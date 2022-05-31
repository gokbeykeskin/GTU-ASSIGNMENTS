#include "serverY.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>


int childReturned=0; //siguser1 received
pid_t availableChild; //pid of the child who sent siguser1

int sigchld_received=0;
int sigint_received=0;
int invertibleAmt=0; //siguser2 received => invertibleAmt+=1

int main(int argc,char* argv[]){
	//precaution against interruptions
    sigset_t mask;
    if(sigfillset(&mask)==-1 || sigdelset(&mask,SIGINT)==-1  || sigdelset(&mask,SIGCHLD)==-1 ||
    	sigdelset(&mask, SIGUSR1)==-1 || sigdelset(&mask, SIGUSR2)==-1 || sigprocmask(SIG_SETMASK, &mask, NULL)==-1){
    	perror("Failed to block signals");
    	exit(EXIT_FAILURE);
    }
	int one_instance = open("serverY.file",O_CREAT|O_EXCL );
	if(one_instance==-1){
		myPrint(0,"One instance of Server Y is already running.\nTerminating...\n");
		exit(EXIT_SUCCESS);
	}
	int i,freeChildFound,j,x;
	char* serverFifoPath="", *logFilePath="";
	int poolSize=-1,poolSize2=-1,sleepTime=-1;
	int serverFd, dummyFd, logFD;
	struct sigaction sigusr_action,sigusr2_action,sigchld_action,sigint_action;
	char temp[20]; //used for int to char* conversions
	int bufferCap=128;
	int busyPoolAmt=0,requestsHandled=0,requestsForwarded=0;//just for output
	char* buffer = (char*)malloc(sizeof(char)*bufferCap);
	char readByte;
	int zPipeFD;
	struct flock lock;
	memset(&lock,0,sizeof(lock));
	
	memset(&sigusr_action, 0, sizeof(struct sigaction));
	memset(&sigusr2_action, 0, sizeof(struct sigaction));
	memset(&sigchld_action, 0, sizeof(struct sigaction));
	memset(&sigint_action, 0, sizeof(struct sigaction));
	/*--------Read arguments--------*/
	if(argc!=11){
		myPrint(0,"There should be 11 arguments...exiting\n");
		exit(EXIT_FAILURE);
	}
	for(int i=1;i<argc-1;i++){
		if(strcmp(argv[i],"-s")==0) serverFifoPath = argv[i+1];
		if(strcmp(argv[i],"-o")==0) logFilePath = argv[i+1];
		if(strcmp(argv[i],"-p")==0) poolSize = atoi(argv[i+1]);
		if(strcmp(argv[i],"-r")==0) poolSize2 = atoi(argv[i+1]);
		if(strcmp(argv[i],"-t")==0) sleepTime = atoi(argv[i+1]);

	}

	if(strcmp(serverFifoPath,"")==0 || strcmp(logFilePath,"")==0 || poolSize<0 ||
		poolSize2<0 || sleepTime<0){
		myPrint(0,"Invalid/Missing arguments...exiting\n");
		exit(EXIT_FAILURE);
	}

	logFD = open(logFilePath,O_WRONLY | O_CREAT | O_APPEND);
	if(logFD == -1){
		perror("open log file");
		exit(EXIT_FAILURE);
	}
	becomeDaemon(logFD);
	myPrint(0,"--------------------------------------------------------------------------------------\n");

	myPrint(1,"Server Y (");myPrint(0,logFilePath),myPrint(0,", p=");
	sprintf(temp,"%d",poolSize),myPrint(0,temp);myPrint(0,", t=");
	sprintf(temp,"%d",poolSize2);myPrint(0,temp);myPrint(0,") started\n");

	if(sigaction(SIGPIPE,NULL,NULL)==-1) perror("sigaction");

	sigusr_action.sa_flags = SA_SIGINFO;
	sigusr_action.sa_sigaction = siguserHandler;
	if(sigaction(SIGUSR1,&sigusr_action,NULL)==-1) perror("sigaction");

	sigusr2_action.sa_handler = siguser2Handler;
	if(sigaction(SIGUSR2,&sigusr2_action,NULL)==-1) perror("sigaction");


	sigchld_action.sa_handler = sigchldHandler;
	if(sigaction(SIGCHLD,&sigchld_action,NULL)) perror("sigaction");

	sigint_action.sa_handler = sigintHandler;
	if(sigaction(SIGINT,&sigint_action,NULL)) perror("sigaction");

	if(mkfifo(serverFifoPath,S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}
	serverFd = open(serverFifoPath, O_RDONLY);
	if(serverFd==-1){
		if(sigint_received){
			myPrint(1,"SIGINT received before initialization, Terminating.\n");
			free(buffer);
			close(serverFd);
			unlink(serverFifoPath);
			exit(EXIT_SUCCESS);
		}
		perror("open");
	}
	dummyFd = open(serverFifoPath,O_WRONLY); //to never see EOF
	if(dummyFd == -1){
		if(sigint_received){
			myPrint(1,"SIGINT received before initialization, Terminating.\n");
			free(buffer);
			close(serverFd);
			unlink(serverFifoPath);
			exit(EXIT_SUCCESS);
		}
		perror("open");
		exit(EXIT_FAILURE);
	}

	int serverZPID;
	zPipeFD=createServerZ(poolSize2,sleepTime,logFD,&serverZPID);

	myPrint(1,"Instantiated server Z\n");
	child children[poolSize];
	createChildren(poolSize,children,sleepTime);
	i=0;freeChildFound=1;

	j=0;
	while(1){


		while( (x=read(serverFd,&readByte,1))) {
			if(sigchld_received){ //to prevent zombie children;
				wait(NULL);
			}
			if(sigint_received){
				myPrint(1,"SIGINT Received, terminating Z and exiting server Y Total requests handled: ");
				sprintf(temp,"%d",requestsHandled);myPrint(0,temp);myPrint(0,", ");
				sprintf(temp,"%d",invertibleAmt);myPrint(0,temp);myPrint(0," invertible, ");
				sprintf(temp,"%d",requestsHandled-invertibleAmt);myPrint(0,temp);myPrint(0," not. ");
				sprintf(temp,"%d",requestsForwarded);myPrint(0,temp);myPrint(0," requests were forwarded.\n");

				for(int i=0;i<poolSize;i++){
					//when write end of the children pipe is closed,
					//while-read loop of the children terminates and children returns.
					close(children[i].writeEnd);
				}
				close(one_instance);
				kill(serverZPID,SIGINT); //send the signal to serverZ
				free(buffer);
				close(serverFd);
				unlink(serverFifoPath);
				close(dummyFd);
				close(serverFd);
				close(zPipeFD);
				exit(0);
			}
			if(x!=-1){
				if(j==bufferCap){
					bufferCap*=2;
					buffer = (char*)realloc(buffer,sizeof(char)*bufferCap);
				}
				buffer[j++]=readByte;
				if(readByte=='e'){

					j=0;
					freeChildFound=0;
					for(i=0;i<poolSize;i++){
						if(children[i].busy==0){
							freeChildFound=1;
							children[i].busy=1; 
							busyPoolAmt++;//didn't used this in algorithm, just for the output
							break;
						}
					}
					break;
				}
			}
			if(childReturned){
				for(int j=0;j<poolSize;j++){
					if(children[j].pid == availableChild){
						children[j].busy=0;
						childReturned=0;
						availableChild=-1;
						busyPoolAmt--; //didn't used this in algorithm, just for the output
						break;
					}
				}
			}
		}
		
		
		if(freeChildFound){
			myPrint(1,"\n\nBUF:");myPrint(0,buffer);
			write(children[i].writeEnd,buffer,strlen(buffer));
			lock.l_type = F_WRLCK;
			fcntl(1,F_SETLKW,&lock);
			myPrint(1,"pool busy ");sprintf(temp,"%d",busyPoolAmt);
			myPrint(0,temp);myPrint(0,"/");sprintf(temp,"%d",poolSize);
			myPrint(0,temp);myPrint(0,", ");
			lock.l_type = F_UNLCK;
			fcntl(1,F_SETLKW,&lock);
			requestsHandled+=1;
		}
		else{
			//OUTPUT
			int k=0,m=0;
			lock.l_type = F_WRLCK;
			fcntl(1,F_SETLKW,&lock);
			myPrint(1,"pool busy ");sprintf(temp,"%d",poolSize);
			myPrint(0,temp);myPrint(0,"/");myPrint(0,temp);
			myPrint(0,", Forwarding request of client PID#");
			for(;buffer[k]!='|';k++){
				temp[k]=buffer[k];
			}
			temp[k]='\0';
			myPrint(0,temp); myPrint(0," to serverZ, ");
			for(int l=k;buffer[l]!='\n';l++){
				if(buffer[l]==',') m+=1;
			}
			m+=1;
			myPrint(0,"matrix size ");sprintf(temp,"%d",m);
			myPrint(0,temp);myPrint(0,"x");myPrint(0,temp);
			myPrint(0,"\n");
			lock.l_type = F_UNLCK;
			fcntl(1,F_SETLKW,&lock);
			//SEND TO Z
			requestsForwarded+=1;
			write(zPipeFD,buffer,strlen(buffer));//ServerZ
		}
		memset(buffer,0,strlen(buffer)); //empty the buffer
	}
	
	return 0;
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

	/*Close all open files*/
	maxfd = sysconf(_SC_OPEN_MAX);
	if (maxfd == -1)
		maxfd = BD_MAX_CLOSE;

	dup2(logFD,STDOUT_FILENO);
	dup2(logFD,STDERR_FILENO);
	close(logFD);
	close(0);
	for (fd = 3; fd < maxfd; fd++)
		close(fd);
	return 0;
}

void siguserHandler(int sig, siginfo_t *info, void *context){
	childReturned=1;
	availableChild=info->si_pid;
}

void siguser2Handler(int sig){
	invertibleAmt+=1;
}

void sigchldHandler(int sig){
	sigchld_received=1;
}

void sigintHandler(int sig){
	unlink("serverY.file");
	sigint_received=1;
}

int createServerZ(int poolSize2, int sleepTime, int logFD,int * serverZPID){
	pid_t pid;
	int filedes[2];
	char filedes_str[8],poolSize2_str[8],sleepTime_str[8];
    if(pipe(filedes) == -1)
    {
        perror( "pipe Failed" );
        exit(EXIT_FAILURE);
    }
    sprintf(filedes_str,"%d",filedes[0]);
    sprintf(poolSize2_str,"%d",poolSize2);
    sprintf(sleepTime_str,"%d",sleepTime);

	char* args[] = {filedes_str,poolSize2_str,sleepTime_str,(char*)0};

    pid = fork();

    if(pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if(pid == 0)
    {

        close(filedes[1]);
        execve("serverZ",args,NULL);
        perror("execve");
        exit(0);
    }

    else{//parent
        close(filedes[0]);
        (*serverZPID) = pid;
    }
    return filedes[1];
}

int childFunc(int filedes,int sleepTime){
	char readByte;
	char temp[10];
	char clientFifo[32];
	int clientFD;
	int n = 16;
	int i=0,j=0,k=0;
	int newArrival = 1;
	int inv=0;//1 if invertible
	struct flock lock;
	memset(&lock,0,sizeof(lock));
	int **matrix = (int**)malloc(sizeof(int*)*n);
	for(int i=0;i<n;i++){
		matrix[i]=(int*)malloc(sizeof(int)*n);
	}


	while(read(filedes,&readByte,1)>0){
		if(newArrival){
			memset(clientFifo,0,strlen(clientFifo));
			clientFifo[i++] = readByte;

			while(read(filedes,&readByte,1)>0){
				if(readByte=='|'){
					read(filedes,&readByte,1);//empty read to pass '|'
					clientFifo[i]='\0';
					break;
				}
				clientFifo[i++] = readByte;
			}
			newArrival=0;
			i=0;

		}
		
		clientFD = open(clientFifo, O_WRONLY);
		if(readByte!=',' && readByte!='\n' && readByte!='e'){
			temp[k++]=readByte;
		}
		else{
			temp[k]='\0';
			k=0;
			matrix[i][j++]=atoi(temp);

			if(readByte=='\n'){
				i+=1;j=0;
			}
			if(readByte=='e'){
				lock.l_type = F_WRLCK;
				fcntl(1,F_SETLKW,&lock);
				myPrint(2," is handling client PID#");
				myPrint(0,clientFifo);myPrint(0,", matrix size ");
				sprintf(temp,"%d",i);myPrint(0,temp);myPrint(0,"x");
				myPrint(0,temp);myPrint(0,"\n");
				lock.l_type = F_UNLCK;
				fcntl(1,F_SETLKW,&lock);
				sleep(sleepTime);
				inv = isInvertible(matrix,i);
				if(inv) kill(getppid(),SIGUSR2);
				sprintf(temp,"%d",inv);
				write(clientFD,temp,strlen(temp));
				kill(getppid(),SIGUSR1);
				newArrival=1;
				inv=0;
				i=0;k=0;j=0;
			}
		}
		
		if(j==n-1){
			char **matrix = (char**)realloc(matrix,sizeof(char*)*n);
			for(int k=0;k<n;k++)
				matrix[k]=(char*)realloc(matrix[k],sizeof(char)*n);
		}

	}
	for(int i=0;i<n;i++){
		free(matrix[i]);
	}

	free(matrix);
	close(clientFD);
	
	return 0;
}

void createChildren(int amt,child* children,int sleepTime){
	pid_t pid;
	int filedes[2];

	 for(int num_process = 0; num_process < amt; num_process++){
        if(pipe(filedes) == -1)
        {
            perror( "pipe Failed" );
            exit(EXIT_FAILURE);
        }

        pid = fork();

        if(pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if(pid == 0)
        {

            close(filedes[1]);
            childFunc(filedes[0],sleepTime);
            exit(0);
        }

        else{//parent
            close(filedes[0]);
	    	children[num_process].pid = pid;
	    	children[num_process].writeEnd = filedes[1];
	    	children[num_process].busy = 0;
        }
    }
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
    int determinant = 0; 
 
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
		write(1,"Y:",2);
		time_t rawtime;
		struct tm *timeinfo;
		char timeStr[64];
		

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strcpy(timeStr,asctime(timeinfo));
		timeStr[strlen(timeStr)-1]='|';
		write(1,timeStr,strlen(timeStr));
		
	}
	if(timeStamp==2){
		char pid[16];
		sprintf(pid,"%d",getpid());
		write(1,"Worker PID#",11);write(1,pid,strlen(pid));
	}
	
	write(1,str,strlen(str));
}