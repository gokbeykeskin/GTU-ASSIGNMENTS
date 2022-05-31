#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include "hw4.h"


int sems; //semaphores set
int sigint_received=0;


int main(int argc, char* argv[]){
	//sem[0] ->'1', sem[1] ->'2'.
	int c=-1,n=-1;

	struct sigaction sigint_action;
	memset(&sigint_action, 0, sizeof(struct sigaction));
	sigint_action.sa_handler = sigintHandler;
	if(sigaction(SIGINT,&sigint_action,NULL)) perror("sigaction");


	/*Semaphores*/
	key_t s_key;
    union semun  
    {
        int val;
        struct semid_ds *buf;
        ushort array [2];
    } sem_attr;


	char inputfilePath[32];


    /*Threads*/
	pthread_t supplier_thread,*consumer_thread;
	pthread_attr_t supplier_attr;
	struct consumer_args* consArg;

	if ((s_key = ftok (SEM_KEY, 'a')) == -1) {
        perror ("ftok"); exit (1);
    }

     if ((sems = semget (s_key, 2, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }

	sem_attr.val = 0;
    if (semctl (sems, 0, SETVAL, sem_attr) == -1) {
        perror ("semctl SETVAL"); exit (1);
    }
    if (semctl (sems, 1, SETVAL, sem_attr) == -1) {
        perror ("semctl SETVAL"); exit (1);
    }


    /*Set stdout as non-buffering*/
	setvbuf(stdout, NULL, _IONBF, 0);



	for(int i=1;i<argc-1;i++){
		if(strcmp(argv[i],"-C")==0) c=atoi(argv[i+1]);
		else if(strcmp(argv[i],"-N")==0) n=atoi(argv[i+1]);
		else if(strcmp(argv[i],"-F")==0) strcpy(inputfilePath,argv[i+1]);
	}

	if(c==-1||n==-1||strcmp(inputfilePath,"x")==0){
		printf("%s\n","Missing/Invalid Arguments.\nTerminating..\n" );
		exit(EXIT_FAILURE);
	}
	else if(argc<7){
		printf("%s\n","Missing Arguments.\nTerminating..\n" );
		exit(EXIT_FAILURE);
	}
	else if(argc>7){
		printf("There are more arguments than needed. Ignoring them and proceeding\n");
	}
	else if(n<=1 || c<=4){
		printf("%s\n","N should be greater than 1 and C should be greater than 4.\nTerminating.." );
		exit(EXIT_FAILURE);
	}

	/*Set supplier thread as detached*/
	if(pthread_attr_init(&supplier_attr)!=0){
		perror("pthread_attr_init");exit(EXIT_FAILURE);
	}
	if(pthread_attr_setdetachstate(&supplier_attr,PTHREAD_CREATE_DETACHED)!=0){
		perror("pthread_attr_setdetachstate");exit(EXIT_FAILURE);
	}

	pthread_create(&supplier_thread,&supplier_attr,supplier,(void*)(inputfilePath));



	consumer_thread = (pthread_t*)malloc(sizeof(pthread_t)*c);
	consArg=(struct consumer_args*)malloc(sizeof(struct consumer_args)*c);
	
	for(int i=0;i<c;i++){
		consArg[i].n = n;
		consArg[i].cons_id = i;
		pthread_create(&consumer_thread[i],NULL,consumer,(void*)(&consArg[i]));	
	}
	
	for(int i=0;i<c;i++){
		pthread_join(consumer_thread[i],NULL);
	}

	if(semctl(sems,0,IPC_RMID)==-1){
		perror("semctl_IPC_RMID");
		exit(EXIT_FAILURE);
	}
	free(consumer_thread);
	free(consArg);
	return 0;
}


void sigintHandler(int sig){
	sigint_received=1;
}

void* supplier(void* args){

	struct sembuf asem[1];
	char* inputfilePath = (char*)args;
	int input_fd;
	char readByte;

	asem[0].sem_op = 1;
	asem[0].sem_flg = 0;
	if( (input_fd = open(inputfilePath,O_RDONLY)) == -1){
		perror("open_inputfd");exit(EXIT_FAILURE);
	} 
	while( read(input_fd,&readByte,1)>0){
		if(readByte=='1'){
			printf("%s|Supplier: read from input a '1'. Current amounts: %d x '1', %d x '2'.\n",
				getTimestamp().timestring,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));
			asem[0].sem_num = 0;
			if(semop(sems,asem,1)==-1){ //sem_post
				perror("semop");exit(EXIT_FAILURE);
			}
			printf("%s|Supplier: delivered a '1'. Post-delivery amounts: %d x '1', %d x '2'.\n",
				getTimestamp().timestring,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));
		}
		else if(readByte=='2'){
			printf("%s|Supplier: read from input a '2'. Current amounts: %d x '1', %d x '2'.\n",
				getTimestamp().timestring,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));
			asem[0].sem_num = 1;
			if(semop(sems,asem,1)==-1){
				perror("semop");exit(EXIT_FAILURE);
			}
			printf("%s|Supplier: delivered a '2'. Post-delivery amounts: %d x '1', %d x '2'.\n",
				getTimestamp().timestring,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));

		}
		if(sigint_received){
			close(input_fd);
			pthread_exit(NULL);
		}

	}
	printf("%s|The supplier has left.\n",getTimestamp().timestring);

	close(input_fd);
	pthread_exit(NULL);
	return 0;
}


void* consumer(void* args){

	struct consumer_args consArg = *((struct consumer_args*)args);
	struct sembuf asem[2];
	
	//set both semaphores operations as -1 (decrement)
	asem[0].sem_num = 0;
	asem[0].sem_op = -1;
	asem[0].sem_flg = 0;

	asem[1].sem_num = 1;
	asem[1].sem_op = -1;
	asem[1].sem_flg = 0;

	for(int i=0;i<consArg.n;i++){
		printf("%s|Consumer-%d at iteration %d (waiting). Current amounts: %d x '1', %d x '2'.\n",
			getTimestamp().timestring,consArg.cons_id,i,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));
		if(semop(sems,asem,2)==-1){ //sem_wait
			perror("semop");exit(EXIT_FAILURE);
		}
		printf("%s|Consumer-%d at iteration %d (consumed). Post-consumption amounts: %d x '1', %d x '2'.\n",
			getTimestamp().timestring,consArg.cons_id,i,semctl(sems,0,GETVAL),semctl(sems,1,GETVAL));

		if(sigint_received){
			pthread_exit(NULL);
		}

	}
	printf("%s|Consumer-%d has left.\n",getTimestamp().timestring,consArg.cons_id);
	pthread_exit(NULL);

	return 0;
}

struct Timestamp getTimestamp()
{
    char   timebuffer[53]     = {0};
    struct tm      *tmval     = NULL;
    struct tm       gmtval    = {0};
    struct timespec curtime   = {0};

    struct Timestamp timestamp;


    // Get current time
    clock_gettime(CLOCK_REALTIME, &curtime);


    // Set the fields
    timestamp.seconds      = curtime.tv_sec;
    timestamp.milliseconds = curtime.tv_nsec/1.0e6;

    if((tmval = localtime_r(&timestamp.seconds, &gmtval)) != NULL)
    {
        // Build the first part of the time
        strftime(timebuffer, sizeof timebuffer, "%Y-%m-%d %H:%M:%S", &gmtval);
        // Add the milliseconds part and build the time string
        snprintf(timestamp.timestring, sizeof timestamp.timestring, "%s:%ld", timebuffer, timestamp.milliseconds); 
    }

    return timestamp;
}