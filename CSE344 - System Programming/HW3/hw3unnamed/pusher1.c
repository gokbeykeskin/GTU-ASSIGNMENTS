#include "chefslib.h"
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int fd_shm,fd_sem;
	struct shared_memory* shmem;
	sem_t *semaphores;
	sem_t *mutex, *sugar,*mw,*mf,*fw;

	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}
    if ((shmem = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	if ((fd_sem = shm_open (SEM_SHMEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}
    if ((semaphores = mmap (NULL, sizeof (sem_t)*12, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_sem, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	mutex=&semaphores[10];
	sugar=&semaphores[3];
	mw=&semaphores[5];
	mf=&semaphores[4];
	fw=&semaphores[7];


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
