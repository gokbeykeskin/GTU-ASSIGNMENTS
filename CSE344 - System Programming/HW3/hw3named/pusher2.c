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
	sem_t *mutex,*flour,*mw,*ms,*ws;
	if((mutex = sem_open(MUTEX_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((flour = sem_open(FLOUR_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((mw = sem_open(MW_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((ms = sem_open(MS_SEM_NAME,0,0600,0))==SEM_FAILED){
		perror("sem_open");exit(EXIT_FAILURE);
	}
	if((ws = sem_open(WS_SEM_NAME,0,0600,0))==SEM_FAILED){
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
		1. pusher:waits for sugar
		2. pusher: waits for flour
		3. pusher: waits for milk
		4. pusher: waits for walnut
	*/
	while(1){

		if(sem_wait(flour)==-1){
			perror("pusher2_semwait_flour");
		}
		if(sem_wait(mutex)==-1){
			perror("pusher2_semwait_mutex");
		}
		if(shmem->isSugar){
			shmem->isSugar=0;
			sem_post(mw);
		}
		else if(shmem->isWalnut){
			shmem->isWalnut=0;
			sem_post(ms);
		}
		else if(shmem->isMilk){
			shmem->isMilk=0;
			sem_post(ws);	
		}
		else{
			shmem->isFlour=1;
		}
		sem_post(mutex);
	}
}