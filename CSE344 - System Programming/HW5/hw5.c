#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <complex.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "hw5.h"


struct info *global_info;
pthread_mutex_t lock;
pthread_cond_t condvar;
int arrived=0;
int sigint_received=0;
int main(int argc, char* argv[]){
	struct timeval start,end;
	float elapsedTime;
	char temp[30]; //for float -> string conversion 
	char* filePath1="x",*filePath2="x",*outputPath="x";

	int file1_fd, file2_fd, out_fd;
	
	struct sigaction sigint_action;
	memset(&sigint_action, 0, sizeof(struct sigaction));
	sigint_action.sa_handler = sigintHandler;
	if(sigaction(SIGINT,&sigint_action,NULL)){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	global_info = (struct info*)malloc(sizeof(struct info));
	global_info->n=-1;
	global_info->m=-1;

	pthread_t *threads;
	char readByte;
	if(argc<11){
		printf("%s\n","Missing Arguments.\nTerminating...");
		exit(EXIT_FAILURE);
	}

	for(int i=1;i<argc-1;i++){
		if(strcmp(argv[i],"-i")==0){
			filePath1=argv[i+1];
		}
		else if(strcmp(argv[i],"-j")==0){
			filePath2 = argv[i+1];
		}
		else if(strcmp(argv[i],"-o")==0){
			outputPath=argv[i+1];
		}
		else if(strcmp(argv[i],"-n")==0){
			global_info->n=pow(2,atoi(argv[i+1]));
		}
		else if(strcmp(argv[i],"-m")==0){
			global_info->m=atoi(argv[i+1]);
		}
	}
	if(strcmp(filePath1,"x")==0 || strcmp(filePath2,"x")==0 || strcmp(outputPath,"x")==0 || global_info->n==-1 || global_info->m==-1){
		printf("Invalid/Missing Arguments\nTerminating\n");
		exit(EXIT_FAILURE);
	}
	else if(global_info->m%2!=0 || global_info->n<=4 ){
		printf("%s\n","Wrong m or n\nTerminating...");
		exit(EXIT_FAILURE);
	}
	else if(argc>11){
		printf("%s\n","There are non-used arguments.\nIgnoring them and proceeding...\n" );
	}

	if (pthread_mutex_init(&lock, NULL) != 0) {
		perror("mutex_init");
        exit(EXIT_FAILURE);
    }

    if(pthread_cond_init(&condvar, NULL)!= 0){
    	perror("cond_init");
    	exit(EXIT_FAILURE);
    }

	if( (file1_fd = open(filePath1,O_RDONLY))==-1){
		perror("open_file1");
		exit(EXIT_FAILURE);
	}
	if( (file2_fd = open(filePath2,O_RDONLY))==-1){
		perror("open_file2");
		exit(EXIT_FAILURE);
	}


	global_info->matrix1=(int**)malloc(sizeof(int*)*global_info->n);
	global_info->matrix2=(int**)malloc(sizeof(int*)*global_info->n);
	global_info->resMatrix=(int**)malloc(sizeof(int*)*global_info->n);
	double complex** fourier_matrix=(double complex**)malloc(sizeof(double complex*)*global_info->n);;


	for(int i=0;i<global_info->n;i++){
		global_info->matrix1[i]=(int*)malloc(sizeof(int)*global_info->n);
		fourier_matrix[i]=(double complex*)malloc(sizeof(double complex)*global_info->n);

		global_info->matrix2[i]=(int*)malloc(sizeof(int)*global_info->n);
		global_info->resMatrix[i]=(int*)malloc(sizeof(int)*global_info->n);
		memset(global_info->resMatrix[i],0,sizeof(int)*global_info->n);
		for(int j=0;j<global_info->n;j++){
			if(read(file1_fd,&readByte,1)>0)
				global_info->matrix1[i][j]=(int)readByte;

			else{
				printf("%s\n","File 1 has insufficient content.\nFatal Error!\nTerminating...");
				exit(EXIT_FAILURE);
			}
			if(read(file2_fd,&readByte,1)>0)
				global_info->matrix2[i][j]=(int)readByte;
			else{
				printf("%s\n","File 2 has insufficient content.\nFatal Error!\nTerminating...");
				exit(EXIT_FAILURE);
			}


		}

	}
	gettimeofday(&start,NULL);

	printf("%s|Two matrices of size %dx%d have been read. The number of threads is %d\n",
		getTimestamp().timestring,global_info->n,global_info->n,global_info->m );


	threads= (pthread_t*)malloc(sizeof(pthread_t)*global_info->m);
	int* thread_id=(int*)malloc(sizeof(int)*global_info->m);
	for(int i=0;i<global_info->m;i++){
		thread_id[i]=i;
		pthread_create(&threads[i],NULL,thread_func,(void*)(&thread_id[i]));

	}

	double complex** temp_complex;
	int index=0;

	

	for(int i=0;i<global_info->m;i++){
		pthread_join(threads[i],(void*)(&temp_complex));
		index=0;
		for(int j=i;j<global_info->n;j+=global_info->m){
			for(int k=0;k<global_info->n;k++){
				fourier_matrix[j][k] = temp_complex[index][k];
			}
			free(temp_complex[index]);
			index++;
		}
		free(temp_complex[index]);
		free(temp_complex);
	}
	if(sigint_received){
			for(int i=0;i<global_info->n;i++){
				free(global_info->matrix1[i]);
				free(global_info->matrix2[i]);
				free(global_info->resMatrix[i]);
				free(fourier_matrix[i]);

			}
			free(global_info->matrix1);
			free(global_info->matrix2);
			free(global_info->resMatrix);
			free(global_info);
			free(fourier_matrix);
			free(threads);
			free(thread_id);
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&condvar);
			close(file1_fd);
			close(file2_fd);
			exit(EXIT_SUCCESS);
		}

	if( (out_fd = open(outputPath,O_WRONLY|O_CREAT|O_TRUNC,0666))<0 ){
		perror("open");
		exit(EXIT_FAILURE);
	}

	for(int i=0;i<global_info->n;i++){
		for(int j=0;j<global_info->n;j++){

			if(sigint_received){
				for(int i=0;i<global_info->n;i++){
					free(global_info->matrix1[i]);
					free(global_info->matrix2[i]);
					free(global_info->resMatrix[i]);
					free(fourier_matrix[i]);

				}
				free(global_info->matrix1);
				free(global_info->matrix2);
				free(global_info->resMatrix);
				free(global_info);
				free(fourier_matrix);
				free(threads);
				free(thread_id);
				for(int i=0;i<global_info->m;i++) pthread_join(threads[i],NULL);
				pthread_mutex_destroy(&lock);
				pthread_cond_destroy(&condvar);
				close(out_fd);
				close(file1_fd);
				close(file2_fd);
				exit(EXIT_SUCCESS);
			}

			sprintf(temp,"%.3f",creal(fourier_matrix[i][j]));
			write(out_fd,temp,strlen(temp));
			sprintf(temp,"%+.3f",cimag(fourier_matrix[i][j]));
			write(out_fd,temp,strlen(temp));
			write(out_fd,"i,",2);
		}
		write(out_fd,"\n",1);
	}
	gettimeofday(&end,NULL);
	elapsedTime = (end.tv_sec - start.tv_sec);
	elapsedTime += (end.tv_usec - start.tv_usec)/1000000.0;
	printf("%s|The process has written the output file. The total time spent is %.4f seconds.\n",
		getTimestamp().timestring,elapsedTime);

	for(int i=0;i<(global_info->n);i++){
		free(global_info->matrix1[i]);
		free(global_info->matrix2[i]);
		free(global_info->resMatrix[i]);
		free(fourier_matrix[i]);

	}
	free(global_info->matrix1);
	free(global_info->matrix2);
	free(global_info->resMatrix);
	free(global_info);
	free(threads);
	free(thread_id);
	free(fourier_matrix);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&condvar);
	close(out_fd);
	close(file1_fd);
	close(file2_fd);
	return 0;
}


void* thread_func(void* thread_num){
	struct timeval start,end;
	float elapsedTime;
	int tnum=*((int*)(thread_num));
	gettimeofday(&start,NULL);
	double complex** fourierMatrix = (double complex**)malloc(sizeof(double complex*)* ((global_info->n/global_info->m)+1) );
	for(int i=0;i<(global_info->n/global_info->m)+1;i++){
		fourierMatrix[i]=(double complex*)malloc(sizeof(double complex)*global_info->n);
	}


	//FIRST PART
	for(int i=tnum;i<global_info->n;i+=global_info->m){
		for(int j=0;j<global_info->n;j++){
			for(int k=0;k<global_info->n;k++){
				global_info->resMatrix[i][j]+=global_info->matrix1[i][k]*global_info->matrix2[k][j];
				if(sigint_received){
					pthread_exit((void*)fourierMatrix );
				}

			}

		}
	}

	//SYNCH BARRIER
	pthread_mutex_lock(&lock);
		gettimeofday(&end,NULL);
		++arrived;
		elapsedTime = (end.tv_sec - start.tv_sec);
		elapsedTime += (end.tv_usec - start.tv_usec)/1000000.0;
		
		printf("%s|Thread %d has reached the rendezvous point in %.4f seconds.\n",getTimestamp().timestring,tnum,elapsedTime);
		while(arrived<global_info->m){
			pthread_cond_wait(&condvar,&lock);
		}
		pthread_cond_broadcast(&condvar);
	pthread_mutex_unlock(&lock);
	printf("%s|Thread %d is advancing to the second part.\n",getTimestamp().timestring,tnum);
	gettimeofday(&start,NULL);

	int ind=0;
	double complex temp;

	//SECOND PART
	for(int i=tnum;i<global_info->n;i+=global_info->m){
		for(int j=0;j<global_info->n;j++){
			temp=0;
			for(int k=0;k<global_info->n;k++){
				for(int l=0;l<global_info->n;l++){
					temp+=global_info->resMatrix[k][l]*cpow(M_E,-I*( (2*M_PI)/global_info->n*i*k ))*cpow(M_E,-I*( (2*M_PI)/global_info->n )*j*l);
					if(sigint_received){
						pthread_exit((void*)fourierMatrix );
					}
				}

			}
			fourierMatrix[ind][j]=temp;


		}
		ind++;
	}
	gettimeofday(&end,NULL);
	elapsedTime = (end.tv_sec - start.tv_sec);
	elapsedTime += (end.tv_usec - start.tv_usec)/1000000.0;
	printf("%s|Thread %d has finished the second part in %.4f seconds.\n",getTimestamp().timestring,tnum,elapsedTime);

	pthread_exit((void*)fourierMatrix );
}

struct Timestamp getTimestamp()
{
    char   timebuffer[53]     = {0};
    struct tm      *tmval     = NULL;
    struct tm       gmtval    = {0};
    struct timespec curtime   = {0};

    struct Timestamp timestamp;


    // Get current time
    clock_gettime(CLOCK_REALTIME, &curtime);


    // Set the fields
    timestamp.seconds      = curtime.tv_sec;
    timestamp.milliseconds = curtime.tv_nsec/1.0e6;

    if((tmval = localtime_r(&timestamp.seconds, &gmtval)) != NULL)
    {
        // Build the first part of the time
        strftime(timebuffer, sizeof timebuffer, "%Y-%m-%d %H:%M:%S", &gmtval);
        // Add the milliseconds part and build the time string
        snprintf(timestamp.timestring, sizeof timestamp.timestring, "%s:%ld", timebuffer, timestamp.milliseconds); 
    }

    return timestamp;
}

void sigintHandler(int sig){
	sigint_received=1;
}

int isPowerOfTwo(int n){
    if (n <= 1) //1 is not accepted as in assignment pdf
        return 0;
    while (n != 1) {
        if (n % 2 != 0)
            return 0;
        n = n / 2;
    }
    return 1;
}