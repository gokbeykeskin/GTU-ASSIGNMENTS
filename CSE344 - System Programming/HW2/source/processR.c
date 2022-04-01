#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>
#include "processR.h"
#define COORDAMT 10

int sigint_received=0;

int main(int argc, char* argv[]){
	struct sigaction sigint_action;
	memset(&sigint_action, 0, sizeof(sigint_action));
	sigint_action.sa_handler = &sigintHandler;
	sigaction(SIGINT, &sigint_action,NULL);


	double covarianceMatrix[3][3];
	double coord[10][3];
	char* outputPath = argv[0];
	tokenizer(coord);
	covMatrix(covarianceMatrix,coord);
	writeToFile(covarianceMatrix,outputPath);
	return 1;
}

void tokenizer(double coordinates[10][3]){
	
	char c_coord[10][50];
	strcpy(c_coord[0],getenv("Coord0"));
	strcpy(c_coord[1],getenv("Coord1"));
	strcpy(c_coord[2],getenv("Coord2"));
	strcpy(c_coord[3],getenv("Coord3"));
	strcpy(c_coord[4],getenv("Coord4"));
	strcpy(c_coord[5],getenv("Coord5"));
	strcpy(c_coord[6],getenv("Coord6"));
	strcpy(c_coord[7],getenv("Coord7"));
	strcpy(c_coord[8],getenv("Coord8"));
	strcpy(c_coord[9],getenv("Coord9"));
	for(int i=0;i<COORDAMT;i++){
		for(int j=0;j<3;j++){
			coordinates[i][j]=(double)c_coord[i][j];
		}
	}

}


void covMatrix(double covMatrix[3][3], double coord[10][3]){


	double avgX=0,avgY=0,avgZ=0; //average
	double varX=0,varY=0,varZ=0; //variance
	double covarXY=0,covarYZ=0,covarXZ=0; //co-variance
	for(int i=0;i<COORDAMT;i++){
		avgX+=coord[i][0];
		avgY+=coord[i][1];
		avgZ+=coord[i][2];
	}
	avgX/=COORDAMT;
	avgY/=COORDAMT;
	avgZ/=COORDAMT;
	for(int i=0;i<COORDAMT;i++){
		varX+=pow(coord[i][0]-avgX,2);
		varY+=pow(coord[i][1]-avgY,2);
		varZ+=pow(coord[i][2]-avgZ,2);
	}
	varX/=COORDAMT;
	varY/=COORDAMT;
	varZ/=COORDAMT;
	for(int i=0;i<COORDAMT;i++){
		covarXY+=(coord[i][0]-avgX)*(coord[i][1]-avgY);
		covarYZ+=(coord[i][1]-avgY)*(coord[i][2]-avgY);
		covarXZ+=(coord[i][0]-avgX)*(coord[i][2]-avgZ);
	}
	covarXY/=COORDAMT;
	covarXZ/=COORDAMT;
	covarYZ/=COORDAMT;

	covMatrix[0][0] = varX;
	covMatrix[1][1] = varY;
	covMatrix[2][2] = varZ;
	covMatrix[0][1] = covarXY;
	covMatrix[1][0] = covarXY;
	covMatrix[2][0] = covarXZ;
	covMatrix[0][2] = covarXZ;
	covMatrix[2][1] = covarYZ;
	covMatrix[1][2] = covarYZ;

}

void writeToFile(double covMatrix[3][3],char* outputPath){

	if(sigint_received){
		exit(0); //if sigint received, don't open or lock the file and exit.	
	}

	char temp[50];
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd = open(outputPath, O_RDWR | O_APPEND | O_CREAT, mode);
	struct flock lock;
	memset(&lock,0,sizeof(lock));
	lock.l_type = F_WRLCK;
	// locking file to prevent multiple processes write to file at the same time.
	fcntl(fd,F_SETLKW,&lock);
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			sprintf(temp,"%lf",covMatrix[i][j]);
			write(fd,temp,strlength(temp));
			if(j!=2) write(fd,",",1);
		}
		if(i!=2) write(fd,"\n",1);
	}
	write(fd,"\n",1);
	lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLKW,&lock);
	if(close(fd)==-1){
		perror("close");
	}
}

void sigintHandler(int signal_number){
	sigint_received=1;
}

int strlength(char* str){
	int i=0;
	do{
		i++;
	}while(str[i]!='\0');
	return i;
}