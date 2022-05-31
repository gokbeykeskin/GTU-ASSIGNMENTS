#include "chefslib.h"
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int fd_shm;
	struct shared_memory* shmem;
	sem_t *mutex,*milk,*ws,*fs,*fw;
	if((mutex = sem_open(MUTEX_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((milk = sem_open(MILK_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((ws = sem_open(WS_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((fs = sem_open(FS_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	
	if((fw = sem_open(FW_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}


	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}

    if ((shmem = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}
	/*
		1. pusher: waits for sugar
		2. pusher: waits for flour
		3. pusher: waits for milk
		4. pusher: waits for walnut
	*/
	while(1){

		sem_wait(milk);

		sem_wait(mutex);
		if(shmem->isFlour){
			shmem->isFlour=0;
			sem_post(ws);
		}
		else if(shmem->isWalnut){
			shmem->isWalnut=0;
			sem_post(fs);
		}
		else if(shmem->isSugar){
			shmem->isSugar=0;

			if(sem_post(fw)==-1){
				perror("flour2_post");
			}
		}
		else{
			shmem->isMilk=1;
		}
		sem_post(mutex);
	}
}