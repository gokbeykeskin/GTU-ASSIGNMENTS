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
	char* input_file_path;
	char* wsaler_sem_name;

	if(argc != 5){
		printf("There should be 5 arguments.\nTerminating...\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-n")==0){
		input_file_path=argv[2];
		wsaler_sem_name = argv[4];
	}
	else if(strcmp(argv[3],"-i")==0 && strcmp(argv[1],"-n")==0){
		input_file_path=argv[4];
		wsaler_sem_name = argv[2];
	}
	else{
		printf("Invalid/Missing arguments.\nTerminating...\n");
		exit(EXIT_FAILURE);		
	}
	int wholesaler_pid;
	
	int shm_fd,didRead,input_fd;
	struct shared_memory* shmem;
	char readByte;
	int size;
	char temp[20];
	int bufCap = 128;
	char **ingr=(char**)malloc(sizeof(char*)*bufCap);
	for(int k=0;k<bufCap;k++){
		ingr[k]=(char*)malloc(sizeof(char)*3);
	}
	int i=0,j=0;

	sem_t *milk,*flour,*walnut,*sugar; //wholesaler semaphores
	sem_t *mf,*mw,*ms,*fw,*fs,*ws; //pusher semaphores
	sem_t *mutex; //mutual exclusion semaphore
	sem_t *wsaler_sem; //to wake wholesaler


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

	//Create semaphores
	if( (milk = sem_open(MILK_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("milk_sem_open");exit(EXIT_FAILURE);
	}
	if((flour = sem_open(FLOUR_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("flour_sem_open");exit(EXIT_FAILURE);
	}
	if((walnut = sem_open(WALNUT_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("walnuts_sem_open");exit(EXIT_FAILURE);
	}
	if((sugar = sem_open(SUGAR_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("sugar_sem_open");exit(EXIT_FAILURE);
	}
	if( (mf = sem_open(MF_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("milk_sem_open");exit(EXIT_FAILURE);
	}
	if( (mw = sem_open(MW_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("flour_sem_open");exit(EXIT_FAILURE);
	}
	if( (ms = sem_open(MS_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("walnuts_sem_open");exit(EXIT_FAILURE);
	}
	if((fw = sem_open(FW_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("sugar_sem_open");exit(EXIT_FAILURE);
	}
	if((fs = sem_open(FS_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("walnuts_sem_open");exit(EXIT_FAILURE);
	}
	if((ws = sem_open(WS_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("sugar_sem_open");exit(EXIT_FAILURE);
	}
	if((mutex = sem_open(MUTEX_SEM_NAME,O_CREAT,0600,0))==SEM_FAILED){
		perror("walnuts_sem_open");exit(EXIT_FAILURE);
	}
	if((wsaler_sem = sem_open(wsaler_sem_name,O_CREAT,0600,0))==SEM_FAILED){
		perror("wsaler_sem_open");exit(EXIT_FAILURE);
	}

	if(sem_post(mutex)==-1){
		perror("sem_post_mutex");
		exit(EXIT_FAILURE);
	} //set mutex to 1.

	//Read the input file and store it in ingr array
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

	//store the size and wholesaler semaphores name as the last two elements of the ingr array
	sprintf(temp,"%d",size);
	strcpy(ingr[i++],temp);
	strcpy(ingr[i++],wsaler_sem_name);
	free(ingr[i]);
	ingr[i]=0;

	wholesaler_pid = fork(); //wholesaler
	if(wholesaler_pid==0){
		execve("wholesaler",ingr,NULL);//create wholesaler and send ingr array to it
		perror("execve returned");exit(EXIT_FAILURE);
	}
	else if(wholesaler_pid==-1){
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else{
		for(int k=0;k<bufCap;k++)
			free(ingr[k]); //free ingr array after sending it
		free(ingr);
	}

	int wstatus;
	wait(&wstatus);//wait for wholesaler to finish up

	//Clean after yourself
	sem_close(milk);
	sem_close(flour);
	sem_close(walnut);
	sem_close(sugar);
	sem_close(mutex);
	sem_close(wsaler_sem);
	sem_close(mf);
	sem_close(mw);
	sem_close(ms);
	sem_close(fw);
	sem_close(fs);
	sem_close(ws);

	sem_unlink(MILK_SEM_NAME);
	sem_unlink(FLOUR_SEM_NAME);
	sem_unlink(WALNUT_SEM_NAME);
	sem_unlink(SUGAR_SEM_NAME);
	sem_unlink(MUTEX_SEM_NAME);
	sem_unlink(wsaler_sem_name);
	sem_unlink(MF_SEM_NAME);
	sem_unlink(MW_SEM_NAME);
	sem_unlink(MS_SEM_NAME);
	sem_unlink(FW_SEM_NAME);
	sem_unlink(FS_SEM_NAME);
	sem_unlink(WS_SEM_NAME);
	shm_unlink(SHARED_MEM_NAME);

	return 0;
	
}