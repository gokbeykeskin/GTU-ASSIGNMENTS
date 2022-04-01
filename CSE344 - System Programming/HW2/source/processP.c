#include <fcntl.h>
#include <stdio.h> //used for perror and sprintf(double to char*)
#include <unistd.h>
#include <stdlib.h> //used for malloc-realloc
#include <string.h> //used for memset and strcpy
#include<sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>
#include "processP.h"
int sigint_catched=0,sigchld_catched=0;
int *childPids,pidsIndex = 0;
int childpids_size=64;

int main(int argc, char* argv[]){
	sig_atomic_t status; //exit status of the last terminated child process
	char **envp; //environmental variables which will be sent to child
    char **childargv; //arguments which will be sent to child
    unsigned int buf_size = 64;
	char* inputPath,*outputPath;//input file inputPath
    //block all signals except sigint (sigint behaves differently)
    sigset_t mask;

    if(sigfillset(&mask)==-1 || sigdelset(&mask,SIGINT)==-1  || sigdelset(&mask,SIGCHLD) || sigprocmask(SIG_SETMASK, &mask, NULL)==-1){
    	perror("Failed to block signals (except sigint)");
    	return 1;
    }

	if(argc!=5){
		write(1,"Missing/Invalid command line arguments\n",39);
		exit(0);
	}
	else if(strcmp(argv[1],"-i")==0){
		inputPath=argv[2];
		if(strcmp(argv[3],"-o")==0)
			outputPath=argv[4];
		else{
			write(1,"Missing/Invalid command line arguments\n",39);
			exit(0);
		}
	}
	else if(strcmp(argv[3],"-i")==0){
		inputPath=argv[4];
		if(strcmp(argv[1],"-o")==0)
			outputPath=argv[2];
		else{
			write(1,"Missing/Invalid command line arguments\n",39);
			exit(0);
		}
	}
	else{
		write(1,"Missing/Invalid command line arguments\n",39);
		exit(0);
	}

	struct sigaction sigint_action,sigchld_action;
	memset(&sigint_action, 0, sizeof(sigint_action));
	memset(&sigchld_action, 0, sizeof(sigchld_action));
	sigint_action.sa_handler = &sigintHandler;
	sigchld_action.sa_handler = &sigchldHandler;
	sigaction(SIGCHLD, &sigchld_action,NULL);
	sigaction(SIGINT, &sigint_action,NULL);
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	int fd = open(inputPath, O_RDONLY, mode);

	if(fd==-1){
		perror("open");
		return -1;
	}
	
	childPids = (int*)malloc(sizeof(int)*childpids_size);
	char* buffer=(char*)malloc(sizeof(char)*buf_size); //buffer which holds the 10 coordinates
	double* frob_norms = (double*)malloc(sizeof(double)*childpids_size);

	unsigned int byteAmt; //1 if next char is read succesfully
	unsigned char readByte; //next char


	int i,j=0;
	char temp[50]; //used for converting doubles to char* before calling write function

	//if the user created a file with name outputhPath, remove it. 
	if(access(outputPath, F_OK)) unlink(outputPath); 

	write(1,"Process P reading ",18);
	write(1,inputPath,length(inputPath));
	write(1,"\n",1);
	childargv = (char**)malloc(sizeof(char*));
    childargv[0] = (char*)malloc(sizeof(char)*length(outputPath)+1);
	envp = (char**)malloc(sizeof(char*)*10);
	
	for(int i=0;i<10;i++){
		envp[i] = (char*)malloc(sizeof(char)*50);
	}
    do {
		if(sigint_catched){ //clear resources and pass the signal to children
			for(int i=0;i<pidsIndex;i++){
				kill(SIGINT,childPids[i]);
			}
			free(childPids);
			free(buffer);
	    	close(fd);
	    	for(int i=0;i<10;i++){
	    		free(envp[i]);
	    	}
			free(envp);
			free(frob_norms);
			free(childargv[0]);
			free(childargv);

			unlink(outputPath);
			exit(0);
		}

		strcpy(envp[0],"Coord0=");
		strcpy(envp[1],"Coord1=");
		strcpy(envp[2],"Coord2=");
		strcpy(envp[3],"Coord3=");
		strcpy(envp[4],"Coord4=");
		strcpy(envp[5],"Coord5=");
		strcpy(envp[6],"Coord6=");
		strcpy(envp[7],"Coord7=");
		strcpy(envp[8],"Coord8=");
		strcpy(envp[9],"Coord9=");

		i=0;
		j=0;
    	do{
    		byteAmt = read(fd, &readByte, 1);
		    if(byteAmt>0){
    			if(i!=10)envp[i][7+j++]=readByte;
    			if(j==3){
    				j=0;
    				i++;
    			}

		    }

    	}while(i<10 && byteAmt!=0);
    	if(i!=10) break;
    	
    	strcpy(childargv[0],outputPath);

    	write(1, "Created R_",10);
		sprintf(temp,"%d",pidsIndex);
		write(1,temp,length(temp));
		write(1," with ",6);

		for(int k=0;k<i;k++){
			write(1,"(",1);
			for(int j=0;j<3;j++){
				sprintf(temp,"%d",getAscii(envp[k][j+7]));
				write(1,temp,length(temp));
				if(j!=2)write(1,",",1);
			}
			write(1,")",1);
			if(i!=9) write(1,",",1);
		}

		write(1,"\n",1);
		if(pidsIndex==childpids_size-1){
			childpids_size*=2;
			childPids=(int*)realloc(childPids,sizeof(int)*childpids_size);
			frob_norms=(double*)realloc(frob_norms,sizeof(double)*childpids_size);
		}

		spawnChild("processR",childargv,envp);
	   	if(sigchld_catched==1){//to prevent zombie children
	   		wait(&status);
	   		sigchld_catched=0;
	   	} 

    }while (byteAmt!=0);

    wait(&status);
	if(WIFEXITED(status)){
		calcFrobeniusNorm(outputPath);
	}
	else{
		free(buffer);
		free(childPids);
		free(frob_norms);
		free(childargv[0]);
		free(childargv);
		for(int i=0;i<10;i++){
			free(envp[i]);
		}
		free(envp);
		close(fd);
		unlink(outputPath);

		perror("a child is aborted unexpectedly");
		exit(0);

	}

	free(buffer);
	free(childPids);
	free(frob_norms);
	free(childargv[0]);
	free(childargv);
	for(int i=0;i<10;i++){
		free(envp[i]);
    }
	free(envp);
	close(fd);
	sigemptyset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL); //unblock signals
	unlink(outputPath);

    return 0;

}

void sigintHandler(int signal_number){
	sigint_catched = 1;
}
void sigchldHandler(int signal_number){
	sigchld_catched = 1;
}


void spawnChild(char* program, char** arguments, char** matrix){
	pid_t child_pid = fork();

	switch(child_pid){
		case -1:
			perror ("fork");
			exit (0);
		case 0:
			execve(program,arguments,matrix);
    		perror("Execve"); //execve returns only on error case.
			exit(1);
		default:
			childPids[pidsIndex++]=child_pid;
	}
}

void calcFrobeniusNorm(char* outputPath){
	if(pidsIndex<=1){ //there are less then 20 coordinates (2 set). Nothing to compare
		write(1,"There should be at least 2 sets (20 coordinates) in order to calculate closest 2.\n",82);
		return;
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	unsigned int byteAmt; //1 if next char is read succesfully
	unsigned char readByte; //next char
	int fd = open(outputPath, O_RDONLY, mode);
	unsigned int buf_size=64;
	double frob_norm=0;
	int frob_norms_size=64;
	double* frob_norms = (double*)malloc(sizeof(double)*frob_norms_size);
	double (*matrices)[3][3] = malloc(sizeof(double)*frob_norms_size*3*3); 

	double matrix[3][3];
	
	int i=0,j=0,k=0,m=0;
	char* buffer=(char*)malloc(sizeof(char)*buf_size);
	write(1,"Reached EOF, collecting outputs from ",37);
	write(1,outputPath,length(outputPath));
	write(1,"\n",1);
	do{
		i=0;j=0;k=0,frob_norm=0;
		while(i<3){
    		byteAmt = read(fd, &readByte, 1);
    		if(byteAmt==0) break;
    		if(readByte!=',' && readByte!='\n' && readByte!='\0'){
    			buffer[k++] = readByte;
    		}
    		else{
    			buffer[k]='\0';
    			matrix[i][j]=atof(buffer);
				memset(buffer,0,sizeof(buf_size));
    			j++;
    			k=0;
    			if(readByte=='\n'){
    				j=0;
    				i++;
    			}
    		}
    	}		
	    if(byteAmt==0) break;

	    if(m==frob_norms_size-1){
	    	frob_norms_size*=2;
	    	frob_norms=(double*)realloc(frob_norms,sizeof(double)*frob_norms_size);

	    	matrices = realloc(matrices,sizeof(double)*frob_norms_size*3*3);

	    }
	    for(int i=0;i<3;i++){
	    	for(int j=0;j<3;j++){
	    		frob_norm+=square(matrix[i][j]);
				matrices[m][i][j] = matrix[i][j];
	    	}
	    }
	    frob_norm=sqrt(frob_norm);

	    frob_norms[m++] = frob_norm;

    }while(byteAmt>0);


    free(buffer);
    findClosestPair(frob_norms,m,matrices);
    free(matrices);
    free(frob_norms);
    close(fd);
}

double square(double num){
	return num*num;
}

void findClosestPair(double* frob_norms,int frobNormAmt,double (*matrices)[3][3]){
	double min = 9999999;
	double temp;
	int matrix1Ind;
	int matrix2Ind;
	char tempStr[32];
	for(int i=0;i<frobNormAmt;i++){
		for(int j=i+1;j<frobNormAmt;j++){
			temp = fabs(frob_norms[i]-frob_norms[j]);
			if(temp<min){
				min = temp;
				matrix1Ind = i;
				matrix2Ind = j;

			}
		}
	}
	write(1,"The closest 2 matrices are\n",27);
	write(1,"------------------------------------------------------------------------------------\n",85);
	for(int k=0;k<3;k++){
		for(int l=0;l<3;l++){
			sprintf(tempStr,"%lf",matrices[matrix1Ind][k][l]);
			write(1,tempStr,length(tempStr));
			write(1,"  ",2);
		}
		write(1,"\t and \t",7);
		for(int l=0;l<3;l++){
			sprintf(tempStr,"%lf",matrices[matrix2Ind][k][l]);
			write(1,tempStr,length(tempStr));
			write(1,"  ",2);
		}
		write(1,"\n",1);
	}
	write(1,"------------------------------------------------------------------------------------\n",85);

	write(1,"and their distance is\n",22);
	sprintf(tempStr,"%lf",min);
	write(1,tempStr,length(tempStr));
	write(1,"\n",1);
}

int length(char* str){
	int i=0;
	do{
		i++;
	}while(str[i]!='\0');
	return i;
}

int getAscii(char x){
	return x;
}