#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include "../headers/utils.h"
#include "../headers/common.h"
#include "../headers/servant.h"
#include "../headers/client.h"
#include "../headers/networking.h"

int thread_amt=0;
int arrived=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
SA_IN server_addr;
char** requests;
int main(int argc, char *argv[]){
    char request_file_path[256]="x",server_ip[16]="x";
    int server_port=-1;
    size_t request_size_cap = INITIAL_REQ_SIZE;
    char *buffer=NULL,*req_type;
    size_t buf_size=0;
    int char_amt;
    pthread_t *threads;
    int* thread_nums;

    //Get the arguments
    arg_check(7,argc);
    for(int i=1;i<argc-1;i++){
        if(strcmp(argv[i],"-r")==0){
            strcpy(request_file_path,argv[i+1]);
        }
        else if(strcmp(argv[i],"-q")==0){
            if(atoi(argv[i+1])<MIN_PORT_NUM){
                fprintf(stderr,"Port number should be greater than 5000\n");
                exit(EXIT_FAILURE);
            }
            server_port = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-s")==0){
            strcpy(server_ip,argv[i+1]);
        }
    }
    if(server_port==-1||strcmp(request_file_path,"x")==0||strcmp(server_ip,"x")==0) {
        fprintf(stderr,"Invalid arguments\n");
        exit(EXIT_FAILURE);
    }
    //open request file
    FILE* fp = fopen(request_file_path,"r");
    if(!fp){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    init_socket(&server_addr,server_port);
    requests = calloc(request_size_cap,sizeof(char*));
    //read the request file
    while( (char_amt = getline(&buffer,&buf_size,fp))!=-1 ){
        req_type = remove_until_delim(buffer,' ');
        if(char_amt<16){
            free(req_type);
            break;
        }
        else if(strncmp(req_type,"transactionCount",16)!=0){
            free(req_type);
            fprintf(stderr,"Unknown Command.\n");
            exit(EXIT_FAILURE);
        }
        free(req_type);

        requests[thread_amt] = (char*)calloc(FIXED_MSG_LEN,sizeof(char));
        sprintf(requests[thread_amt],"%s %s","clnt",buffer);
        requests[thread_amt][strlen(requests[thread_amt])-1]='\0';
       
        thread_amt++;
        if(thread_amt==request_size_cap){
            request_size_cap*=2;
            requests = (char**)realloc(requests,sizeof(char*)*request_size_cap);
        }
    }
    printf("Client: I have loaded %d requests and I'm creating %d threads.\n",thread_amt,thread_amt);
    //create threads
    thread_nums = (int*)malloc(sizeof(int)*thread_amt);
    threads = (pthread_t*)malloc(sizeof(pthread_t)*thread_amt);
    for(int j=0;j<thread_amt;j++){
        thread_nums[j]=j;
        fail_check_pthread(pthread_create(&threads[j],NULL,thread_function,(void*)(&thread_nums[j])),"pthread_create_client");
    }
    //wait for threads to end
    for(int j=0;j<thread_amt;j++){
        fail_check_pthread(pthread_join(threads[j],NULL),"pthread_join_client");
    }
    //clean up and exit
    printf("Client: All threads have terminated, goodbye.\n");
    fclose(fp);
    free(requests);
    free(thread_nums);
    free(threads);
    free(buffer);
    fail_check_pthread(pthread_mutex_destroy(&lock),"pthread_mutex_destroy_client");
    return 0;
}

void* thread_function(void* arg){
    int thread_num = *(int*)arg;
    char response[FIXED_MSG_LEN];
    int sv_socket_fd;
    printf("Client-Thread-%d: Thread-%d has been created\n",thread_num,thread_num);
    fail_check_pthread(pthread_mutex_lock(&lock),"pthread_mutex_lock_client");
    //Synch Barrier
    ++arrived;
    if(arrived<thread_amt){
		fail_check_pthread(pthread_cond_wait(&condvar,&lock),"pthread_cond_wait_client");
	}
    fail_check_pthread(pthread_cond_broadcast(&condvar),"pthread_cond_broadcast_client");
	fail_check_pthread(pthread_mutex_unlock(&lock),"pthread_mutex_unlock_client");
    //End Synch Barrier
    printf("Client-Thread-%d: I am requesting \"%s\"\n",thread_num,requests[thread_num]);
    //establish connection and send request
    connect_to_socket(&server_addr,&sv_socket_fd);
    fail_check(write(sv_socket_fd,requests[thread_num],FIXED_MSG_LEN),"write_client");
    //receive response
    fail_check(read(sv_socket_fd,response,FIXED_MSG_LEN),"read_client");
    printf("Client-Thread-%d: The server's response to \"%s\" is %s\n",thread_num,requests[thread_num],response);
    //close connection and clean up
    free(requests[thread_num]);
    close(sv_socket_fd);
    printf("Client-Thread-%d: Terminating.\n",thread_num);
    pthread_exit(NULL);
}