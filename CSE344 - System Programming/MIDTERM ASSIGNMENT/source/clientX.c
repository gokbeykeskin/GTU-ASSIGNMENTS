#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include "clientX.h"
#include <signal.h>

int sigint_received=0;
int main(int argc, char* argv[]){
	//precaution against interruptions
    sigset_t mask;
    if(sigfillset(&mask)==-1 || sigdelset(&mask,SIGINT)==-1 || sigprocmask(SIG_SETMASK, &mask, NULL)==-1){
    	perror("Failed to block signals (except sigint)");
    	return 1;
    }
	struct sigaction sigint_action;
	char *serverFifoPath,*dataFilePath;
	int dataFD,fifoFD;
	char readByte;
	char* buffer;
	int bufferCapacity=256,bufferSize=0,elementAmt=0;
	char temp[16];//for integer to char*
	int clientFD;
	char pidString[32];
	struct timeval start,end;
	float elapsedTime;
	sprintf(pidString,"%d",getpid());
	char clientFifo[64] = ""; strcat(clientFifo,pidString);
	memset(&sigint_action, 0, sizeof(struct sigaction));
	sigint_action.sa_handler = sigintHandler;
	if(sigaction(SIGINT,&sigint_action,NULL)) perror("sigaction");

	if(argc!=5){
		myPrint(0,"There should be 5 arguments\n");
		exit(EXIT_FAILURE);
	}
	else if(strcmp(argv[1],"-s")==0 && strcmp(argv[3],"-o")==0){
		serverFifoPath = argv[2];
		dataFilePath = argv[4];
	}
	else if(strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-s")==0){
		serverFifoPath = argv[4];
		dataFilePath = argv[2];
	}
	else{
		myPrint(0,"Invalid/Missing arguments\n");
		exit(EXIT_FAILURE);
	}
	buffer = (char*)malloc(sizeof(char)*bufferCapacity);
	dataFD = open(dataFilePath,O_RDONLY);
	if(dataFD==-1){
		perror("open");
		exit(EXIT_FAILURE);
	}
	fifoFD = open(serverFifoPath, O_WRONLY);
	if(fifoFD==-1){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	if(mkfifo(clientFifo,S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}

	gettimeofday(&start,NULL);
	write(fifoFD,clientFifo,strlen(clientFifo));write(fifoFD,"|",1);

	while(read(dataFD,&readByte,1)){

		if(readByte==',' || readByte=='\n' || readByte=='\0') elementAmt++;
		buffer[bufferSize++]=readByte;
		if(bufferSize==bufferCapacity){
			bufferCapacity*=2;
			buffer=(char*)realloc(buffer,sizeof(char)*bufferCapacity);
		}
		if(sigint_received){
			myPrint(1,"SIGINT received, terminating.\n");
			free(buffer);
			close(fifoFD);
			close(dataFD);
			exit(EXIT_SUCCESS);

		}
		
	}
	myPrint(1,"(");
	myPrint(0,dataFilePath);myPrint(0,") is submitting a ");
	sprintf(temp,"%d",(int)sqrt(elementAmt));myPrint(0,temp);myPrint(0,"x");
	myPrint(0,temp);myPrint(0," matrix\n");
	buffer[bufferSize]='\0';
	if(write(fifoFD,buffer,bufferSize)==-1){
		if(sigint_received){
			myPrint(1,"SIGINT received, terminating.\n");
			free(buffer);
			close(fifoFD);
			close(dataFD);
			exit(EXIT_SUCCESS);
		}	
		perror("write");
		exit(EXIT_FAILURE);	

	}
	write(fifoFD,"e",1);


	

	
	clientFD = open(clientFifo, O_RDONLY);
	if(clientFD==-1) perror("open");
	read(clientFD,&readByte,1);

 	if(readByte=='1')
 		myPrint(1," the matrix is invertible, ");
 	else if(readByte=='0'){
 		myPrint(1," the matrix is not invertible, ");
 	}
 	else{
		myPrint(1,"SIGINT received, terminating.\n");
		 free(buffer);
		close(fifoFD);
		close(dataFD);
		close(clientFD);
		unlink(clientFifo);
		exit(EXIT_SUCCESS);

 	}
 	gettimeofday(&end,NULL);
	elapsedTime = (end.tv_sec - start.tv_sec);
	elapsedTime += (end.tv_usec - start.tv_usec)/1000000.0;

 	myPrint(0,"total time ");
 	sprintf(temp,"%f",elapsedTime);
 	myPrint(0,temp);
 	myPrint(0,", goodbye.\n");
 	free(buffer);
	close(fifoFD);
	close(dataFD);
	close(clientFD);
	unlink(clientFifo);

	return 0;
}

void sigintHandler(int sig){
	sigint_received=1;
}

void myPrint(int timeStamp, char* str){
	if(timeStamp){
		time_t rawtime;
		struct tm *timeinfo;
		char timeStr[64];
		char pid[16];
		sprintf(pid,"%d",getpid());
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strcpy(timeStr,asctime(timeinfo));
		timeStr[strlen(timeStr)-1]='|';
		write(1,timeStr,strlen(timeStr));
		write(1,"Client PID#",11);write(1,pid,strlen(pid));
		write(1,":",1);
	}
	
	write(1,str,strlen(str));
}
