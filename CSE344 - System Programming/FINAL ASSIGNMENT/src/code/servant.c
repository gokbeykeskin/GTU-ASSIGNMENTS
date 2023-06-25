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
#include <pthread.h>
#include "../headers/networking.h"
#include "../headers/utils.h"
#include "../headers/common.h"
#include "../headers/servant.h"


int servant_socket_fd, server_socket_fd, addr_size;
int sigint_received=0;
int handled_req_amt = 0;
struct cities_root cities;
int directory_range[2];
int day_amt;
int total_city_amt;

struct dirent **city_files;
struct dirent **day_files;
int server_port=-1;
int server_sock;
int servant_pid;
int main(int argc, char* argv[]){
    SA_IN servant_addr, server_addr;
    char servants_info[FIXED_MSG_LEN]="";
    int servants_port=-1;
    char directory_path[256]="x";
    char server_ip[16]="x";

    pthread_t thread;
    pthread_attr_t thread_attr;
    int *thread_arg;
    /*Get arguments*/
    arg_check(REQUIRED_ARG_AMT,argc);
    for(int i=1;i<argc-1;i++){
        if(strcmp(argv[i],"-p")==0){
            if(atoi(argv[i+1])<MIN_PORT_NUM){
                fprintf(stderr,"Port number should be greater than %d\n",MIN_PORT_NUM);
                exit(1);
            }
            server_port = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i],"-d")==0){
            strcpy(directory_path,argv[i+1]);
            strcat(directory_path,"/");
        }
        else if(strcmp(argv[i],"-r")==0){
            strcpy(server_ip,argv[i+1]);
        }
        else if(strcmp(argv[i],"-c")==0){
            directory_range[0] = atoi(strtok(argv[i+1],"-"))+2; //+2 to avoid . and ..
            directory_range[1] = atoi(strtok(NULL,"-"))+2;
            servants_port = MIN_PORT_NUM + directory_range[0];
        }
    }
    total_city_amt = scandir(directory_path,&city_files,0,alphasort);

    if(server_port==-1||strcmp(directory_path,"x")==0||strcmp(server_ip,"x")==0 || 
    directory_range[0]>directory_range[1] || directory_range[0]<=0 || directory_range[1]>total_city_amt || servants_port ==-1) {
        fprintf(stderr,"Invalid arguments\n");
        exit(EXIT_FAILURE);
    }
    //-------------------------------------------------------------------------------------
    //Set handlers
    set_sig_handler(servant_sigint_handler);
    atexit(servant_exit_handler);
    servant_pid = get_pid();

    //Get the contents of cities directory
    if(directory_range[0]<0 || directory_range[1]>total_city_amt){
        fprintf(stderr,"Invalid Range\n");
        exit(EXIT_FAILURE);
    }


    cities.city_amt=directory_range[1]-directory_range[0]+1;
    sprintf(servants_info,"srvt|%d|%d|%d|",servant_pid,servants_port,cities.city_amt);
    load_cities(directory_path,&cities,servants_info);
    printf("Servant %d: loaded dataset, cities %s-%s\n",servant_pid,cities.cities[0].city_name,cities.cities[cities.city_amt-1].city_name);
    if(total_city_amt<0){
        perror("scan dir");
        exit(EXIT_FAILURE);
    }
    //Set socket
    init_socket(&server_addr,server_port);
    connect_to_socket(&server_addr,&server_sock);

    fail_check(write(server_sock,servants_info,FIXED_MSG_LEN),"write");
    close(server_sock);
    printf("Servant %d: Dataset Information is sent to Server\n",servant_pid);
    server_socket_init(&servant_addr,&servants_port,&servant_socket_fd);
    printf("Servant %d: listening at port %d\n",servant_pid,servants_port);
    while(1){
        fail_check_pthread(pthread_attr_init(&thread_attr),"pthread_attr_init");
        fail_check_pthread(pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED),"pthread_attr_setdetachstate");
        fail_check(server_socket_fd = accept(servant_socket_fd, (SA*)&server_addr, (socklen_t*)&addr_size),"Accept Server");

       
        thread_arg = (int*)malloc(sizeof(int));
        *thread_arg = server_socket_fd;
        fail_check(pthread_create(&thread,&thread_attr,servant_thread_function,(void*)thread_arg),"Create Servant Thread");

    }

    return 0;
}


void* servant_thread_function(void* arg){
    int server_socket_fd = *(int*)arg;
    free(arg);
    char request[FIXED_MSG_LEN] = "";
    char response[FIXED_MSG_LEN] = "";
    int start,end;
    char transaction_type[FIXED_INFO_LEN] = "";
    char city[FIXED_INFO_LEN] = "";
    long int transaction_amt;
    char* temp;
    fail_check(read(server_socket_fd,request,FIXED_MSG_LEN),"Read Request");
    printf("Servant %d: Request Received from Server:%s\n",servant_pid,request);
    temp = remove_until_delim(request,'|');
    strcpy(transaction_type,temp);
    free(temp);
    sscanf(request,"%d|%d|%s",&start,&end,city);
    transaction_amt = search_transaction(city,start,end,transaction_type);
    sprintf(response,"%ld",transaction_amt);
    fail_check(write(server_socket_fd,response,FIXED_MSG_LEN),"Write Servant Response");
    handled_req_amt+=1;
    printf("Servant %d: Sent response to server:%s\n",servant_pid,response);
    pthread_exit(NULL);

}

int search_transaction(char* city,int start,int end,char* transaction_type){
    int transaction_amt=0;
    if(city!=NULL && strlen(city)>0){
        for(int i=0;i<cities.city_amt;i++){
            if(strcmp(city,cities.cities[i].city_name)==0){
                for(int j=0;j<cities.cities[i].day_amt;j++){
                    if(cities.cities[i].days[j].day>=start && cities.cities[i].days[j].day<=end){
                        for(int k=0;k<cities.cities[i].days[j].transaction_amt;k++){
                            if(strcmp(cities.cities[i].days[j].transactions[k].real_estate_type,transaction_type)==0){
                                transaction_amt += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        for(int i=0;i<cities.city_amt;i++){
            for(int j=0;j<cities.cities[i].day_amt;j++){
                if(cities.cities[i].days[j].day>=start && cities.cities[i].days[j].day<=end){
                    for(int k=0;k<cities.cities[i].days[j].transaction_amt;k++){
                        if(strcmp(cities.cities[i].days[j].transactions[k].real_estate_type,transaction_type)==0){
                            transaction_amt += 1;
                        }
                    }
                }
            }
        }
    }
    return transaction_amt;
}

void load_cities(char* directory_path,struct cities_root* cities,char* servants_info){
    int j=0;
    char temp[1024];
    char temp_backup[1024];
    char* temp_str;
    char *transaction = NULL;
    cities->cities = (struct city*)malloc((directory_range[1]-directory_range[0]+1)*sizeof(struct city)); //will hold the servants cities
    FILE *fp;
    int transaction_cap = 100;
    char path_day[FIXED_INFO_LEN] = "";
    size_t null_size=0;
    for(int i=directory_range[0]-1;i<directory_range[1];i++){
        sprintf(temp,"%s%s",directory_path,city_files[i]->d_name);
        day_amt = scandir(temp,&day_files,0,alphasort);
        
        free(day_files[0]);
        free(day_files[1]);

        strcpy(cities->cities[j].city_name,city_files[i]->d_name);
        cities->cities[j].day_amt = day_amt-2;
        cities->cities[j].days = (struct data_day*)malloc(sizeof(struct data_day)*day_amt);
        for(int i=2;i<day_amt;i++){
            strcpy(path_day,day_files[i]->d_name);
            cities->cities[j].days[i-2].day = 0;
            temp_str = remove_until_delim(day_files[i]->d_name,'-');
            cities->cities[j].days[i-2].day += atoi(temp_str);
            free(temp_str);
            temp_str = remove_until_delim(day_files[i]->d_name,'-');
            cities->cities[j].days[i-2].day += atoi(temp_str)*30;
            free(temp_str);
            cities->cities[j].days[i-2].day += atoi(day_files[i]->d_name)*365;
            strcpy(temp_backup,temp);
            strcat(temp,"/");strcat(temp,path_day);
            cities->cities[j].days[i-2].transactions = (struct transaction*)malloc(sizeof(struct transaction)*transaction_cap);
            if( (fp = fopen(temp,"r"))==NULL){
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            free(day_files[i]);
            cities->cities[j].days[i-2].transaction_amt = 0;
            while (getline(&transaction,&null_size,fp) != -1) {
                temp_str = remove_until_delim(transaction,' ');
                cities->cities[j].days[i-2].transactions[cities->cities[j].days[i-2].transaction_amt].transaction_id = atoi(temp_str);
                free(temp_str);
                temp_str = remove_until_delim(transaction,' ');
                strcpy(cities->cities[j].days[i-2].transactions[cities->cities[j].days[i-2].transaction_amt].real_estate_type,temp_str);
                free(temp_str);
                temp_str = remove_until_delim(transaction,' ');
                strcpy(cities->cities[j].days[i-2].transactions[cities->cities[j].days[i-2].transaction_amt].street,temp_str);
                free(temp_str);
                temp_str = remove_until_delim(transaction,' ');
                cities->cities[j].days[i-2].transactions[cities->cities[j].days[i-2].transaction_amt].surface_area = atoi(temp_str);
                free(temp_str);
                cities->cities[j].days[i-2].transactions[cities->cities[j].days[i-2].transaction_amt].price = atoi(transaction);
                cities->cities[j].days[i-2].transaction_amt++;

                if(cities->cities[j].days[i-2].transaction_amt == transaction_cap){
                    transaction_cap *= 2;
                    cities->cities[j].days[i-2].transactions = (struct transaction*)realloc(cities->cities[j].days[i-2].transactions,sizeof(struct transaction)*transaction_cap);
                }

            }
            fclose(fp);
            strcpy (temp,temp_backup);
        }
        free(day_files);
        strcat(servants_info,city_files[i]->d_name);
        strcat(servants_info,"|");
        j++;
    }
}


void servant_exit_handler(){
    if(sigint_received){
        printf("Servant %d: termination message received, handled %d requests in total.\n",servant_pid,handled_req_amt);
        for(int i=0;i<total_city_amt;i++){
            free(city_files[i]);
        }
        free(city_files);

        for(int i=0;i<cities.city_amt;i++){ 
            for(int j=0;j<day_amt-2;j++){ //-2 since  there are my_city_amt directories (. and ..)
                free(cities.cities[i].days[j].transactions);
            }
            free(cities.cities[i].days);
        }
        free(cities.cities);
        close(server_sock);
        close(servant_socket_fd);
        close(server_socket_fd);
    }
}

void servant_sigint_handler(int signum){
    sigint_received=1;
    exit(EXIT_SUCCESS);
}