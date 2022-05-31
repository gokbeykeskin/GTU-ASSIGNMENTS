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
	sem_t *mutex,*walnut,*mf,*ms,*fs;
	if((mutex = sem_open(MUTEX_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((walnut = sem_open(WALNUT_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((mf = sem_open(MF_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((ms = sem_open(MS_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((fs = sem_open(FS_SEM_NAME,0,0600,0))==SEM_FAILED){
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

		sem_wait(walnut);
		sem_wait(mutex);

		if(shmem->isSugar){
			shmem->isSugar=0;
			sem_post(mf);
		}
		else if(shmem->isFlour){
			shmem->isFlour=0;
			sem_post(ms);
		}
		else if(shmem->isMilk){
			shmem->isMilk=0;
			sem_post(fs);
		}
		else{
			shmem->isWalnut=1;
		}
		sem_post(mutex);
	}
}