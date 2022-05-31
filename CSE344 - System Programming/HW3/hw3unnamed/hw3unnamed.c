#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include "chefslib.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>


int main(int argc,char* argv[]){
	int wholesaler_pid;
	char* input_file_path;
	int shm_fd,didRead,input_fd,sem_fd;
	struct shared_memory* shmem;
	char readByte;
	int size;
	int bufCap=128;
	char temp[20];
	if(argc != 3){
		printf("There should be 3 arguments.\nTerminating...\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1],"-i")==0){
		input_file_path=argv[2];
	}
	else{
		printf("Invalid/Missing arguments.\nTerminating...\n");
		exit(EXIT_FAILURE);		
	}

	/*
	0->milk,1->flour,2->walnut,3->sugar
	4->mf,5->mw,6->ms,7->fw,8->fs,9->ws
	10->mutex,11->wsaler_sem
	*/
	sem_t *semaphores;

	char **ingr=(char**)malloc(sizeof(char*)*bufCap);
	for(int k=0;k<bufCap;k++){
		ingr[k]=(char*)malloc(sizeof(char)*3);
	}
	int i=0,j=0;
	

	//Create shared memory for variables&ingredients
	if( (shm_fd=shm_open(SHARED_MEM_NAME, O_RDWR | O_CREAT | O_TRUNC ,0600))==-1){
		perror("shared_memory_open");exit(EXIT_FAILURE);
	}
	if(ftruncate(shm_fd,sizeof(struct shared_memory))==-1){
		perror("shm_alloc");exit(EXIT_FAILURE);
	}
	if( (shmem=mmap(NULL,sizeof(struct shared_memory), 
		PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0)) == MAP_FAILED){
		perror("shm_map");exit(EXIT_FAILURE);
	}
	shmem->isMilk=0;
	shmem->isWalnut=0;
	shmem->isFlour=0;
	shmem->isSugar=0;
	strcpy(shmem->ingredients,"--");// ingredients array is initially empty.

	//Create shared memory for storing semaphores
	if( (sem_fd=shm_open(SEM_SHMEM_NAME, O_RDWR | O_CREAT | O_TRUNC ,0600))==-1){
		perror("shared_memory_open");exit(EXIT_FAILURE);
	}
	if(ftruncate(sem_fd,sizeof(sem_t)*12)==-1){
		perror("shm_alloc");exit(EXIT_FAILURE);
	}
	if( (semaphores = mmap(0,sizeof(sem_t)*12,PROT_READ|PROT_WRITE,MAP_SHARED,sem_fd,0))==(void*)-1){
		perror("mmap");exit(EXIT_FAILURE);
	}
	for(int i=0;i<12;i++){
		if(sem_init(&(semaphores[i]),1,0)==-1){
			perror("milk_sem_open");exit(EXIT_FAILURE);
		}
	}

	if(sem_post(&semaphores[10])==-1){
		perror("sem_post_mutex");
		exit(EXIT_FAILURE);
	} //set mutex to 1.

	//Read input file and store it in ingr array
	if( (input_fd = open(input_file_path,O_RDONLY)) == -1){
		perror("open_inputfd");exit(EXIT_FAILURE);
	} 

	while( (didRead=read(input_fd,&readByte,1))>=0){
		if(i==bufCap-2){
			bufCap*=2;
			ingr=realloc(ingr,sizeof(char*)*bufCap);
			for(int i=bufCap/2;i<bufCap;i++){
				ingr[i] = (char*)malloc(sizeof(char)*3+2);
			}
		}
		if(didRead<=0){
			size = ++i;
			break;
		}
		else if(readByte=='\n'){
			ingr[i][j]='\0';
			i+=1;
			j=0;
		}
		else if(didRead>0){
			ingr[i][j++] = readByte;
		}

	}

	//store the size of the ingredients array as the last element of the ingr array
	sprintf(temp,"%d",size);
	strcpy(ingr[i++],temp);
	free(ingr[i]);
	ingr[i]=0;

	//create wholesaler and send ingredients array to it
	wholesaler_pid = fork();
	if(wholesaler_pid==0){
		execve("wholesaler",ingr,NULL);
		perror("execve returned");exit(EXIT_FAILURE);
	}
	else if(wholesaler_pid==-1){
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else{
		for(int k=0;k<bufCap;k++) //free the ingr array after sending it to wholesaler
			free(ingr[k]);
		free(ingr);
	}

	//wait for wholesaler to finish
	int wstatus;
	wait(&wstatus);

	//clean after yourself and exit
	for(int i=0;i<12;i++){
		sem_close(&semaphores[i]);
	}

	munmap(shmem,sizeof(struct shared_memory));
	munmap(semaphores,sizeof(sem_t)*12);

	shm_unlink(SHARED_MEM_NAME);
	shm_unlink(SEM_SHMEM_NAME);
	return 0;
	
}