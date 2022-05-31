#include "serverZ.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

int sigchld_received=0,sigint_received=0;
int invertibleAmt=0; //sigusr received => invertibleAmt+=1;
int main(int argc, char* argv[]){
	//precaution against interruptions
	sigset_t mask;
    if(sigfillset(&mask)==-1 || sigdelset(&mask,SIGINT)==-1  || sigdelset(&mask,SIGCHLD)==-1 ||
    	sigdelset(&mask, SIGUSR1)==-1 || sigprocmask(SIG_SETMASK, &mask, NULL)==-1){
    	perror("Failed to block signals");
    	exit(EXIT_FAILURE);
    }
	int zPipeFD = atoi(argv[0]);
	int poolSize = atoi(argv[1]);
	int sleepTime = atoi(argv[2]);
	struct Queue * shared_mem_ptr;
	sem_t *mutex_sem, *empty_sem, *full_sem;
	int fd_shm;
	char *buffer;
	int buffSize=0,buffCap=128;
	char readChar;
	int childrenPid[100];
	struct sigaction sigchld_action,sigint_action,sigusr_action;
	int requestsHandled=0;
	char temp[20]; //for int->char* transformation
	memset(&sigint_action, 0, sizeof(struct sigaction));
	memset(&sigchld_action, 0, sizeof(struct sigaction));
	memset(&sigusr_action, 0, sizeof(struct sigaction));

	sigchld_action.sa_handler = sigchldHandler;
	if(sigaction(SIGCHLD,&sigchld_action,NULL)) perror("sigaction");

	sigint_action.sa_handler = sigintHandler;
	if(sigaction(SIGINT,&sigint_action,NULL)) perror("sigaction");

	sigusr_action.sa_handler = sigusrHandler;
	if(sigaction(SIGUSR1,&sigusr_action,NULL)) perror("sigaction");

	/*Initialization of shared memory and semaphores*/
	if( (mutex_sem = sem_open(SEM_MUTEX_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("mutex");exit(EXIT_FAILURE);
	}
	if( (fd_shm = shm_open(SHARED_MEM_NAME, O_RDWR | O_CREAT | O_TRUNC ,0600)) ==-1){
		perror("shared_memory");exit(EXIT_FAILURE);
	}

	if(ftruncate(fd_shm,sizeof(struct Queue))==-1){
		perror("shm_alloc");exit(EXIT_FAILURE);
	}


	if( (shared_mem_ptr=mmap(NULL,sizeof(struct Queue), 
		PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm,0)) == MAP_FAILED){
		perror("shm_map");exit(EXIT_FAILURE);
	}

	shared_mem_ptr->busyChild = 0;
	shared_mem_ptr->size = 0;
	shared_mem_ptr->capacity = QUEUE_CAP;
    shared_mem_ptr->front = 0;
    shared_mem_ptr->rear = QUEUE_CAP-1;



	if((empty_sem = sem_open(SEM_EMPTY_NAME,O_CREAT,0600,poolSize))==SEM_FAILED){
		perror("1sem_open");exit(EXIT_FAILURE);
	}
	if((full_sem = sem_open(SEM_FULL_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("1sem_open");exit(EXIT_FAILURE);
	}
	buffer = (char*)malloc(sizeof(buffer)*buffCap);
	if(sem_post(mutex_sem)==-1){//Set mutex to 1
		perror("sem_post");exit(EXIT_FAILURE);
	}
	if(sem_init(empty_sem,1,poolSize)==-1){
		perror("sem_post");exit(EXIT_FAILURE);
	}
	if(sem_init(full_sem,1,0)==-1){
		perror("sem_post");exit(EXIT_FAILURE);		
	}

	createChildren(poolSize, sleepTime,childrenPid,poolSize);
	while(1){
		while(read(zPipeFD,&readChar,1)>0){
			if(buffSize==buffCap){
				buffCap*=2;
				buffer=(char*)realloc(buffer,sizeof(char)*buffCap);
			}
			buffer[buffSize++]=readChar;
			if(readChar=='e'){
				buffer[buffSize]='\0';
				buffSize=0;
				if(sem_wait(empty_sem)==-1){
					perror("wait_sem_empty");exit(EXIT_FAILURE);
				}
				if(sem_wait(mutex_sem)==-1){
					perror("wait_sem_mutex");exit(EXIT_FAILURE);
				}
				requestsHandled+=1;
				enqueue(shared_mem_ptr,buffer);
				if(sem_post(mutex_sem)==-1){
					perror("post_sem_mutex");exit(EXIT_FAILURE);
				}
				if(sem_post(full_sem)==-1){
					perror("post_sem_full");exit(EXIT_FAILURE);
				}
			}
		}
		if(sigchld_received){
			wait(NULL);
		}
		if(sigint_received){
			break;
		}

	}

	myPrint(1,"SIGINT Received, exiting server Z. Total requests handled ");
	sprintf(temp,"%d",requestsHandled);myPrint(0,temp);myPrint(0,", ");
	sprintf(temp,"%d",invertibleAmt);myPrint(0,temp);myPrint(0," invertible, ");
	sprintf(temp,"%d",requestsHandled-invertibleAmt);myPrint(0,temp);myPrint(0," not.\n");

	for(int i=0;i<poolSize;i++){
		kill(childrenPid[i],SIGINT);
	}
	sem_close(mutex_sem);
	sem_close(empty_sem);
	sem_close(full_sem);
	sem_unlink(SEM_MUTEX_NAME);
	sem_unlink(SEM_FULL_NAME);
	sem_unlink(SEM_EMPTY_NAME);
	shm_unlink(SHARED_MEM_NAME);
	close(zPipeFD);
	close(fd_shm);
	if (munmap (shared_mem_ptr, sizeof (struct Queue)) == -1){
		perror("munmap");exit(EXIT_FAILURE);
	}
	free(buffer);

	return 0;
}

void sigchldHandler(int sig){
	sigchld_received=1;
}

void sigintHandler(int sig){
	sigint_received=1;
}

void sigusrHandler(int sig){
	invertibleAmt+=1;
}

int becomeDaemon(int logFD)
{
	int maxfd, fd;
	/* Returns 0 on success, -1 on error */
	switch (fork()) {
		case -1: return -1;
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	}

	if (setsid() == -1) return -1;

	switch (fork()) {
		case -1: return -1;
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	}

	umask(0);


	maxfd = sysconf(_SC_OPEN_MAX);
	if (maxfd == -1)
		maxfd = BD_MAX_CLOSE;

	/*Close all unused open files*/
	close(0);
	for (fd = 3; fd < maxfd; fd++)
		close(fd);
	return 0;
}

void createChildren(int amt,int sleepTime, int childrenPid[100], int poolSize){
	pid_t pid;
	 for(int num_process = 0; num_process < amt; num_process++){

        pid = fork();

        if(pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if(pid == 0)
        {	
            childFunc(sleepTime,poolSize);
            exit(0);
        }
        else {
        	childrenPid[num_process]=pid;
        }
    }
}

int childFunc(int sleepTime,int poolSize){
	char temp[10];
	char clientFifo[32];
	int clientFD;
	int n = 16;
	int i=0,j=0,k=0,l=0;
	char buffer[512];
	char* request;
	struct Queue* shared_mem_ptr;
	sem_t *mutex_sem, *empty_sem, *full_sem;
	int fd_shm;
	int inv=0;

	int **matrix = (int**)malloc(sizeof(int*)*n);
	for(int i=0;i<n;i++){
		matrix[i]=(int*)malloc(sizeof(int)*n);
	}
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, 0, 0600, 0)) == SEM_FAILED){
        perror ("2sem_open");exit(EXIT_FAILURE);
    }
	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}

    if ((shared_mem_ptr = mmap (NULL, sizeof (struct Queue), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("2shared_mem");exit(EXIT_FAILURE);
	}
    if ((empty_sem = sem_open (SEM_EMPTY_NAME, 0, 0600, 0)) == SEM_FAILED){
		perror ("sem_open");exit(EXIT_FAILURE);
	}

    if ((full_sem = sem_open (SEM_FULL_NAME, 0, 0600, 0)) == SEM_FAILED){
		perror ("sem_open");exit(EXIT_FAILURE);
	}

	while(1){
		if(sigint_received){
			break;		
		}
		memset(buffer,0,strlen(buffer)); //empty the buffer
		i=0;
		if(sem_wait(full_sem)==-1){
			if(sigint_received){
				break;
			}
			perror("wait_sem_full");
			exit(EXIT_FAILURE);
		}
		if(sem_wait(mutex_sem)==-1){
			perror("wait_sem_mutex");
			exit(EXIT_FAILURE);
		}
		shared_mem_ptr->busyChild+=1;
		request = dequeue(shared_mem_ptr);
		strcpy(buffer,request);
		free(request);
		if(sem_post(mutex_sem)==-1){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(empty_sem)==-1){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		while(buffer[i]!='|'){
			clientFifo[i]=buffer[i];
			i+=1;
		}
		clientFifo[i]='\0';
		i++;

		while(buffer[i]!='\0'){
			if(buffer[i]!=',' && buffer[i]!='\n' && buffer[i]!='e')
				temp[l++]=buffer[i++];
			else{
				temp[l]='\0';
				matrix[j][k] = atoi(temp);
				k++;
				l=0;

				if(buffer[i]=='\n'){
					j+=1;
					k=0;
				}
				if(buffer[i]=='e') break;
				i++;

			}

		}
		myPrint(1,"pool busy ");
		sprintf(temp,"%d",shared_mem_ptr->busyChild);
		myPrint(0,temp);myPrint(0,"/");
		sprintf(temp,"%d",poolSize);myPrint(0,temp);
		myPrint(2,"is handling client #PID");
		myPrint(0,clientFifo);myPrint(0,", matrix size ");
		sprintf(temp,"%d",j);myPrint(0,temp);myPrint(0,"x");
		myPrint(0,temp);myPrint(0,"\n");
		sleep(sleepTime);
		clientFD = open(clientFifo, O_WRONLY);
		inv = isInvertible(matrix,j);
		if(inv){
			kill(getppid(),SIGUSR1);
		}
		sprintf(temp,"%d",inv);
		write(clientFD,temp,strlen(temp));
		inv = 0;
		i=0;k=0;j=0;
		shared_mem_ptr->busyChild-=1;
	}
	sem_post(mutex_sem);//so the parent can exit;
	sem_post(empty_sem);//so the parent can exit;
	sem_close(mutex_sem);
	sem_close(empty_sem);
	sem_close(full_sem);
	shm_unlink(SHARED_MEM_NAME);
	if (munmap (shared_mem_ptr, sizeof (struct Queue)) == -1){
		perror("munmap");exit(EXIT_FAILURE);
	}
	free(shared_mem_ptr);
	for(int i=0;i<n;i++){
		free(matrix[i]);
	}
	free(matrix);
	exit(0);
	return 0;
}
 
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
void enqueue(struct Queue* queue, char* item)
{	
    if (isFull(queue)){
    	myPrint(1,"Queue is full, request ignored\n");
        return;
    }
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    strcpy(queue->array[queue->rear],item);
    queue->size = queue->size + 1;
}

char* dequeue(struct Queue* queue)
{	
    if (isEmpty(queue)){
        return NULL;
    }
    char *item = (char*)malloc(sizeof(char)*1000); //freed at main
    strcpy(item,queue->array[queue->front]);
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void getCofactor(int **mat, int** temp, int p, int q, int n)
{
    int i = 0, j = 0;
 
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {

            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];

                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
 
int determinantOfMatrix(int **mat, int n)
{
    int determinant = 0; // Initialize result
 
    // Base case : if matrix contains single element
    if (n == 1)
        return mat[0][0];
 
    int **temp = (int**)malloc(sizeof(int*)*n);
	for(int i=0;i<n;i++){
		temp[i]=(int*)malloc(sizeof(int)*n);
	}
 
    int sign = 1; 
 
    for (int f = 0; f < n; f++) {
        getCofactor(mat, temp, 0, f, n);
        determinant += sign * mat[0][f] * determinantOfMatrix(temp, n - 1);
 
        sign = -sign;
    }
	for(int i=0;i<n;i++){
		free(temp[i]);
	}
	free(temp);
    return determinant;
}
 
int isInvertible(int **mat, int n)
{
    if (determinantOfMatrix(mat, n) != 0)
        return 1;
    else
        return 0;
}


void myPrint(int timeStamp, char* str){
	if(timeStamp==1){
		write(1,"Z:",2);
		time_t rawtime;
		struct tm *timeinfo;
		char timeStr[64];
		

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strcpy(timeStr,asctime(timeinfo));
		timeStr[strlen(timeStr)-1]='|';
		write(1,timeStr,strlen(timeStr));
	}
	else if(timeStamp==2){
		char pid[16];
		sprintf(pid,"%d",getpid());
		write(1,", Worker PID#",13);write(1,pid,strlen(pid));
	}
	
	write(1,str,strlen(str));
}
