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
	sem_t *mutex, *sugar,*mw,*mf,*fw;

	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}

    if ((shmem = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	if((mutex = sem_open(MUTEX_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}

	if((sugar = sem_open(SUGAR_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((mw = sem_open(MW_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((mf = sem_open(MF_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((fw = sem_open(FW_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}

	/*
	1. pusher:waits for sugar
	2. pusher: waits for flour
	3. pusher: waits for milk
	4. pusher: waits for walnut
*/
	while(1){

		if(sem_wait(sugar)==-1){
			perror("sem_wait");exit(EXIT_FAILURE);
		}
		if(sem_wait(mutex)==-1){
			perror("sem_wait");exit(EXIT_FAILURE);
		}

		if(shmem->isFlour){
			shmem->isFlour=0;
			sem_post(mw);

			
		}
		else if(shmem->isWalnut){
			shmem->isWalnut=0;
			sem_post(mf);
		}
		else if(shmem->isMilk){
			shmem->isMilk=0;
			sem_post(fw);		
		}
		else{
			shmem->isSugar=1;
		}
		sem_post(mutex);
	}
}
