#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "chefslib.h"
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


void createEveryoneElse(int chef_pid[6],int pusher_pid[4]);


int main(int argc, char* argv[]){
	int fd_shm,fd_sem;
	struct shared_memory* shmem;
	sem_t *semaphores;
	sem_t *milk,*flour,*walnut,*sugar; //pusher semaphores
	sem_t *wsaler_sem; //to wake wholesaler
	int chef_pid[6];
	int pusher_pid[4];
	int wstatus;
	char temp[40];
	int size=0;
	int totalDeserts=0;
	createEveryoneElse(chef_pid,pusher_pid);
	char **ingr = (char**)malloc(sizeof(char*)*(argc-1));

	for(int k=0;k<argc-1;k++){
		ingr[k]=(char*)malloc(sizeof(char)*3);
		strcpy(ingr[k],argv[k]);
	}
	size = atoi(argv[argc-1]);
	if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}
	if ((fd_sem = shm_open (SEM_SHMEM_NAME, O_RDWR, 0600)) == -1){
		perror ("2shm_open");exit(EXIT_FAILURE);
	}
    if ((shmem = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}
    if ((semaphores = mmap (NULL, sizeof (sem_t)*12, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_sem, 0)) == MAP_FAILED){
		perror ("shared_mem");exit(EXIT_FAILURE);
	}

	milk = &semaphores[0];
	flour = &semaphores[1];
	walnut  = &semaphores[2];
	sugar = &semaphores[3];
	wsaler_sem = &semaphores[11];
	for(int i=0;i<size;i++){
		if(strcmp(ingr[i],"MS")==0 || strcmp(ingr[i],"SM")==0) strcpy(temp,"milk and sugar");
		else if(strcmp(ingr[i],"MW")==0 || strcmp(ingr[i],"WM")==0) strcpy(temp,"milk and walnuts");
		else if(strcmp(ingr[i],"MF")==0 || strcmp(ingr[i],"FM")==0) strcpy(temp,"milk and flour");
		else if(strcmp(ingr[i],"WF")==0 || strcmp(ingr[i],"FW")==0) strcpy(temp,"walnut and flour");
		else if(strcmp(ingr[i],"WS")==0 || strcmp(ingr[i],"SW")==0) strcpy(temp,"walnut and sugar");
		else if(strcmp(ingr[i],"FS")==0 || strcmp(ingr[i],"SF")==0) strcpy(temp,"flour and sugar");
		else{
			for(int i=0;i<6;i++){
				kill(chef_pid[i],SIGINT);
				waitpid(chef_pid[i],&wstatus,0);
				if(WIFEXITED(wstatus))
					totalDeserts+=WEXITSTATUS(wstatus);
			}
			fprintf(stderr,"WRONG INPUT\n(There might be an extra \\n at the end of the file or other characters than M-S-W-F might be used)\nTerminating... (total deserts %d)\n",totalDeserts );

			for(int i=0;i<argc-2;i++){
				free(ingr[i]);
			}
			free(ingr);
			for(int i=0;i<4;i++){
				kill(pusher_pid[i],SIGINT);
				wait(NULL);
			}
			exit(EXIT_FAILURE);
		}



		printf("the wholesaler (pid %d) delivers %s\n",getpid(),temp);

		if(ingr[i][0]=='W'){
			shmem->ingredients[0]='W';
			if(sem_post(walnut)==-1){
				perror("sem_post_walnut");exit(EXIT_FAILURE);
			}
		}
		if(ingr[i][1]=='W'){
			shmem->ingredients[1]='W';
			if(sem_post(walnut)==-1){
				perror("sem_post_walnut");exit(EXIT_FAILURE);
			}

		}
		if(ingr[i][0]=='S'){
			shmem->ingredients[0]='S';
			if(sem_post(sugar)==-1){
				perror("sem_post_sugar");exit(EXIT_FAILURE);
			}
		}
		if(ingr[i][1]=='S'){
			shmem->ingredients[1]='S';
			if(sem_post(sugar)==-1){
				perror("sem_post_sugar");exit(EXIT_FAILURE);
			}
		}
		if(ingr[i][0]=='F'){
			shmem->ingredients[0]='F';
			if(sem_post(flour)==-1){
				perror("sem_post_flour");exit(EXIT_FAILURE);

			}
		}
		if(ingr[i][1]=='F'){
			shmem->ingredients[1]='F';
			if(sem_post(flour)==-1){
				perror("sem_post_flour");exit(EXIT_FAILURE);
			}
		}
		if(ingr[i][0]=='M'){
			shmem->ingredients[0]='M';
			if(sem_post(milk)==-1){
				perror("sem_post_milk");exit(EXIT_FAILURE);
			}
		}
		if(ingr[i][1]=='M'){
			shmem->ingredients[1]='M';
			if(sem_post(milk)==-1){
				perror("sem_post_milk");exit(EXIT_FAILURE);
			}

		}
		printf("the wholesaler (pid %d) is waiting for the dessert\n",getpid() );
		if(sem_wait(wsaler_sem)==-1){
			perror("sem_wait_ws_sem");exit(EXIT_FAILURE);
		}
		printf("the wholesaler (pid %d) has obtained the dessert and left\n",getpid());
		printf("------------------------------------------------------------\n"); //for clearer output
		shmem->ingredients[2]='\0';
	}

	for(int i=0;i<6;i++){
		kill(chef_pid[i],SIGINT);
		waitpid(chef_pid[i],&wstatus,0);
		totalDeserts+=WEXITSTATUS(wstatus);
	}
	for(int i=0;i<argc-2;i++){
		free(ingr[i]);
	}
	free(ingr);
	for(int i=0;i<4;i++){
		kill(pusher_pid[i],SIGINT);
	}

	printf("the wholesaler (pid %d) is done (total desserts: %d)\n",getpid(),totalDeserts);

}

void createEveryoneElse(int chef_pid[6],int pusher_pid[4]){
	char* pusher_argv[]={0};
	char temp[20];
	char* child_argv[] = {"","",0};
	for(int i=0;i<6;i++){
		chef_pid[i] = fork();
		sprintf(temp,"%d",i);
		child_argv[1] = temp;
		if(i==0){
			sprintf(temp,"%d",0);
			child_argv[0]= temp;
		}
		else if(i==1){
			sprintf(temp,"%d",1);
			child_argv[0]= temp;
		}
		else if(i==2){
			sprintf(temp,"%d",2);
			child_argv[0]= temp;
		}
		else if(i==3){
			sprintf(temp,"%d",3);
			child_argv[0]= temp;
		}
		else if(i==4){
			sprintf(temp,"%d",4);
			child_argv[0]= temp;
		}
		else if(i==5){
			sprintf(temp,"%d",5);
			child_argv[0]= temp;
		}

		switch(chef_pid[i]){
			case 0:
				execve("generic_chef",child_argv,NULL); 
				break;
			case -1:
				perror("execve");
				exit(EXIT_FAILURE);
		}
	}

	
	pusher_pid[0] = fork();
	switch(pusher_pid[0]){
		case 0:
			execve("pusher1",pusher_argv,NULL);
			perror("pusher1");
			break;
		case -1:
			perror("execve");
			exit(EXIT_FAILURE);
	}
	pusher_pid[1] = fork();
	switch(pusher_pid[1]){
		case 0:

			execve("pusher2",pusher_argv,NULL); 
			break;
		case -1:
			perror("execve");
			exit(EXIT_FAILURE);
		}
	pusher_pid[2] = fork();
	switch(pusher_pid[2]){
		case 0:

			execve("pusher3",pusher_argv,NULL); 
			break;
		case -1:
			perror("execve");
			exit(EXIT_FAILURE);
	}
	pusher_pid[3] = fork();
	switch(pusher_pid[3]){
		case 0:

			execve("pusher4",pusher_argv,NULL); 
			break;
		case -1:
			perror("execve");
			exit(EXIT_FAILURE);
	}
}