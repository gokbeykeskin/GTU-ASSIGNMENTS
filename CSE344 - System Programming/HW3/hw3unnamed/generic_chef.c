#include "chefslib.h"
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


void sigint_handler(int sig);
int sigint_received=0;
int main(int argc,char* argv[]){
	int fd_shm,fd_sem;
	struct sigaction sigint_action;
	struct shared_memory* shmem;
	struct flock lock;
	int totalDeserts=0;
	char myIngredients[2][20],otherIngredients[2][20];
	int chefNum = atoi(argv[1]);
	sem_t *semaphores;
	sem_t *have_sem,*wsaler_sem;

	if ((fd_sem = shm_open (SEM_SHMEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}
	if ((semaphores = mmap (NULL, sizeof (sem_t)*12, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_sem, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	memset(&sigint_action, 0, sizeof(struct sigaction));
	wsaler_sem=&semaphores[11];
	if(chefNum==0){
		have_sem=&semaphores[4];
		strcpy(myIngredients[0],"Milk");
		strcpy(myIngredients[1],"Flour");
		strcpy(otherIngredients[0],"Sugar");
		strcpy(otherIngredients[1],"Walnuts");
	}
	if(chefNum==1){
		have_sem=&semaphores[6];
		strcpy(myIngredients[0],"Milk");
		strcpy(myIngredients[1],"Sugar");
		strcpy(otherIngredients[0],"Flour");
		strcpy(otherIngredients[1],"Walnuts");
	}
	if(chefNum==2){
		have_sem=&semaphores[5];
		strcpy(myIngredients[0],"Milk");
		strcpy(myIngredients[1],"Walnuts");
		strcpy(otherIngredients[0],"Sugar");
		strcpy(otherIngredients[1],"Flour");
	}
	if(chefNum==3){
		have_sem=&semaphores[9];
		strcpy(myIngredients[0],"Sugar");
		strcpy(myIngredients[1],"Walnuts");
		strcpy(otherIngredients[0],"Milk");
		strcpy(otherIngredients[1],"Flour");
	}
	if(chefNum==4){
		have_sem=&semaphores[8];
		strcpy(myIngredients[0],"Sugar");
		strcpy(myIngredients[1],"Flour");
		strcpy(otherIngredients[0],"Milk");
		strcpy(otherIngredients[1],"Walnuts");
	}
	if(chefNum==5){
		have_sem=&semaphores[7];
		strcpy(myIngredients[0],"Flour");
		strcpy(myIngredients[1],"Walnuts");
		strcpy(otherIngredients[0],"Milk");
		strcpy(otherIngredients[1],"Sugar");
	}
	memset(&lock,0,sizeof(lock));
	sigint_action.sa_handler = sigint_handler;
	if(sigaction(SIGINT,&sigint_action,NULL)) perror("sigaction");
	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}

    if ((shmem = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	while(1){
		printf("Chef%d (pid %d) is waiting for %s and %s|Ingredients arr:%s\n",chefNum,getpid(),otherIngredients[0],otherIngredients[1],shmem->ingredients);
		if(sem_wait(have_sem)==-1){
			if(sigint_received){
				return totalDeserts;
			}
			perror("have_sem");exit(EXIT_FAILURE);
		}

		printf("Chef%d (pid %d) has taken the %s|Ingredients arr:%s\n",chefNum,getpid(),otherIngredients[0],shmem->ingredients);
		if(shmem->ingredients[0]==otherIngredients[0][0]) shmem->ingredients[0]='-';
		if(shmem->ingredients[1]==otherIngredients[0][0]) shmem->ingredients[1]='-';
		printf("Chef%d (pid %d) has taken the %s|Ingredients arr:%s\n",chefNum,getpid(),otherIngredients[1],shmem->ingredients);
		if(shmem->ingredients[0]==otherIngredients[1][0]) shmem->ingredients[0]='-';
		if(shmem->ingredients[1]==otherIngredients[1][0]) shmem->ingredients[1]='-';
		printf("Chef%d (pid %d) is preparing the dessert|Ingredients arr:%s\n",chefNum,getpid(),shmem->ingredients);
		totalDeserts+=1;
		if(sem_post(wsaler_sem)==-1){
				perror("sem_post_wsaler");exit(EXIT_FAILURE);
		}
		printf("Chef%d (pid %d) has delivered the desert|Ingredients arr:%s\n",chefNum,getpid(),shmem->ingredients);
		if(sigint_received){
			return totalDeserts;
		}
	}
	return totalDeserts;

}

void sigint_handler(int sig){
	sigint_received=1;
}