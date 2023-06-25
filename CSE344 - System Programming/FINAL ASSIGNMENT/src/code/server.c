#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include "../headers/utils.h"
#include "../headers/server.h"
#include "../headers/queue.h"
#include "../headers/common.h"
#include "../headers/networking.h"

int server_socket_fd, client_socket_fd, addr_size;

struct queue *connections;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t servant_set_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
int sigint_received=0;
int num_threads=-1;
pthread_t *thread_pool;
struct servant* my_servants;
int servant_amt = 0;
int servant_cap = 32;
int handled_requests = 0;
int main(int argc, char* argv[]){
    SA_IN server_addr, client_addr;
    char timestamp_buf[32] = "";
    int server_port=-1;
    int *client_fd;
    //Get arguments
    arg_check(5,argc);
    for(int i=1;i<argc-1;i++){
        if(strcmp(argv[i],"-p")==0){
            if(atoi(argv[i+1])<MIN_PORT_NUM){
                fprintf(stderr,"Port number should be greater than %d\n",MIN_PORT_NUM);
                exit(1);
            }
            server_port = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-t")==0){
            if(atoi(argv[i+1])<MIN_SERVER_THREAD_AMOUNT){
                fprintf(stderr,"Number of threads should be greater or equal to 5\n");
                exit(1);
            }
            num_threads = atoi(argv[i+1]);
        }
    }
    if(server_port==-1||num_threads==-1) {
        fprintf(stderr,"Invalid arguments\n");
        exit(1);
    }

    //Establish handlers
    atexit(exit_handler);
    set_sig_handler(sigint_handler);
    //Initialize the server and create the thread pool
    init_all_sv(server_port,server_addr);

    //Accept connections and enqueue them (monitor protected)
    while(1){
        addr_size = sizeof(SA_IN);
        fail_check(client_socket_fd = accept(server_socket_fd, (SA*)&client_addr, (socklen_t*)&addr_size),"Accept Client");
        get_timestamp(timestamp_buf);
        client_fd = malloc(sizeof(int));
        *client_fd = client_socket_fd;
        fail_check_pthread(pthread_mutex_lock(&queue_mutex),"Lock queue mutex");
        enqueue(connections,client_fd);
        fail_check_pthread(pthread_cond_signal(&cond_var),"Signal condition variable");
        fail_check_pthread(pthread_mutex_unlock(&queue_mutex),"Unlock queue mutex");

    }
    return 0;
}

void init_all_sv(int server_port,SA_IN server_addr){
    connections = queue_create();
    my_servants = (struct servant*)malloc(sizeof(struct servant)*servant_cap);
    pthread_attr_t thread_attr;

    thread_pool=(pthread_t*)malloc(sizeof(pthread_t)*num_threads);
    for(int i=0;i<num_threads;i++){
        fail_check_pthread(pthread_attr_init(&thread_attr),"Init thread attribute (Server)");
        fail_check_pthread(pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED),"Set detach state (Server)");
        fail_check_pthread(pthread_create(&thread_pool[i],&thread_attr,thread_function,NULL),"Create thread (Server)");
    }

    server_socket_init(&server_addr,&server_port,&server_socket_fd);
    
}


void exit_handler(){
    char timestamp_buf[32] = "";
    if(sigint_received){
        get_timestamp(timestamp_buf);
        printf("%s|SIGINT has been received. I handled a total of %d requests. Goodbye.\n",timestamp_buf,handled_requests);
        queue_destroy(connections);
        close(server_socket_fd);
        close(client_socket_fd);
        for(int i=0;i<num_threads;i++){
            fail_check_pthread(pthread_cancel(thread_pool[i]),"Cancel thread (Server)");
        }
        free(thread_pool);
        for(int i=0;i<servant_amt;i++){
            kill(my_servants[i].pid,SIGINT);
            for(int j=0;j<my_servants[i].city_amt;j++){
                free(my_servants[i].cities[j]);
            }
            free(my_servants[i].cities);
        }
        free(my_servants);
        fail_check_pthread(pthread_mutex_destroy(&servant_set_mutex),"Destroy servant set mutex");
        fail_check_pthread(pthread_mutex_destroy(&queue_mutex),"Destroy queue mutex");
    }
}

void sigint_handler(int signum){
    sigint_received=1;
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg){
    int *fd_client;
    while(1){
        fail_check_pthread(pthread_mutex_lock(&queue_mutex),"Lock queue mutex");
        while((fd_client = dequeue(connections))==NULL){
            fail_check_pthread(pthread_cond_wait(&cond_var,&queue_mutex),"Wait for condition variable");
        }
        fail_check_pthread(pthread_mutex_unlock(&queue_mutex),"Unlock queue mutex");
        if(fd_client==NULL){
            fprintf(stderr,"NULL REQUEST\n");
            exit(EXIT_FAILURE);
        }
        handle_connection(fd_client);

    }
}

void handle_connection(void* client_socket_fd_ptr){
    char buffer[FIXED_MSG_LEN]="";
    int client_sock = *((int*)client_socket_fd_ptr);
    int servant_sock;
    char request[FIXED_MSG_LEN] = "";
    char response[FIXED_MSG_LEN]="";
    int response_int=0;
    char start_date[16]="",end_date[16]="";
    int start,end;
    char transaction_type[FIXED_INFO_LEN] = "";
    char search_city[FIXED_INFO_LEN]="";
    int city_found = 0;
    char timestamp_buf[32] = "";
    char* temp;
    SA_IN servant_addr;
    free(client_socket_fd_ptr);
    fail_check(read(client_sock,buffer,FIXED_MSG_LEN),"Read from client");
    if(strncmp(buffer,"srvt",4)==0){
        remove_first_n_chars(buffer,5);
        fail_check_pthread(pthread_mutex_lock(&servant_set_mutex),"Lock servant set mutex");
            set_servant_info(buffer);
        fail_check_pthread(pthread_mutex_unlock(&servant_set_mutex),"Unlock servant set mutex");
    }
    else if(strncmp(buffer,"clnt",4)==0){
        start = 0;
        end = 0;
        free(remove_until_delim(buffer,' '));
        get_timestamp(timestamp_buf);
        printf("%s|Request arrived \"%s\"\n",timestamp_buf,buffer);
        sscanf(buffer,"%s %s %s %s",transaction_type,start_date,end_date,search_city);
        //Convert date strings into an integer value
        end_date[strlen(end_date)]='\0';
        temp = remove_until_delim(start_date,'-');
        start += atoi(temp);
        free(temp);
        temp=remove_until_delim(start_date,'-');
        start += atoi(temp)*30;
        free(temp);
        start += atoi(start_date)*365;
        temp = remove_until_delim(end_date,'-');
        end += atoi(temp);
        free(temp);
        temp=remove_until_delim(end_date,'-');
        end += atoi(temp)*30;
        free(temp);
        end += atoi(end_date)*365;
        //------------------------------------------------------------------
        if(search_city!=NULL && strlen(search_city)>0){
            for(int i=0;i<servant_amt;i++){
                for(int j=0;j<my_servants[i].city_amt;j++){
                    if(strcmp(my_servants[i].cities[j],search_city)==0){
                        get_timestamp(timestamp_buf);
                        printf("%s|Contacting servant %d\n",timestamp_buf,my_servants[i].pid);
                            init_socket(&servant_addr,my_servants[i].port);
                            connect_to_socket(&servant_addr,&servant_sock);
                            sprintf(request,"%s|%d|%d|%s",transaction_type,start,end,search_city);
                            fail_check(write(servant_sock,request,FIXED_MSG_LEN),"Write to servant");
                            fail_check(read(servant_sock,response,FIXED_MSG_LEN),"Read from servant");
                            close(servant_sock);
                        city_found = 1;

                        break;
                    }
                }
                if(city_found==1){
                    city_found=0;
                    break;
                }
            }
        }
        else{
            get_timestamp(timestamp_buf);
            printf("%s|Contacting ALL servants.\n",timestamp_buf);
            for(int i=0;i<servant_amt;i++){
                    init_socket(&servant_addr,my_servants[i].port);
                    connect_to_socket(&servant_addr,&servant_sock);
                    sprintf(request,"%s|%d|%d",transaction_type,start,end);
                    fail_check(write(servant_sock,request,FIXED_MSG_LEN),"Write to servant");
                    fail_check(read(servant_sock,response,FIXED_MSG_LEN),"Read from servant");
                    response_int+=atoi(response);
                    close(servant_sock);
                
            }
            sprintf(response,"%d",response_int);
        }
        get_timestamp(timestamp_buf);
        printf("%s|Response received: %s, forwarded to client.\n",timestamp_buf,response);
        fail_check(write(client_sock,response,FIXED_MSG_LEN),"Write to client");
        handled_requests+=1;
        close(client_sock);
    }
    else{
        printf("Invalid sender! Ignored.\n");
    }

}

void set_servant_info(char* buf){
    char timestamp_buf[32] = "";
    char *temp;
    temp = remove_until_delim(buf,'|');
    my_servants[servant_amt].pid = atoi(temp);
    free(temp);
    temp = remove_until_delim(buf,'|');
    my_servants[servant_amt].port = atoi(temp);
    free(temp);
    temp = remove_until_delim(buf,'|');
    my_servants[servant_amt].city_amt = atoi(temp);
    free(temp);
    my_servants[servant_amt].cities = (char**)malloc(sizeof(char*)*(my_servants[servant_amt].city_amt));
    for(int j=0;j<my_servants[servant_amt].city_amt;j++){
        my_servants[servant_amt].cities[j] = (char*)malloc(sizeof(char)*256);
        temp = remove_until_delim(buf,'|');
        strcpy(my_servants[servant_amt].cities[j],temp);
        free(temp);
    }
    get_timestamp(timestamp_buf);
    printf("%s|Servant %d is present at the port %d handling cities %s-%s\n",
        timestamp_buf,my_servants[servant_amt].pid,my_servants[servant_amt].port,my_servants[servant_amt].cities[0],
        my_servants[servant_amt].cities[my_servants[servant_amt].city_amt-1]);
    if((++servant_amt)==servant_cap){
        servant_cap*=2;
        my_servants = (struct servant*)realloc(my_servants,sizeof(struct servant)*servant_cap);
    }

}