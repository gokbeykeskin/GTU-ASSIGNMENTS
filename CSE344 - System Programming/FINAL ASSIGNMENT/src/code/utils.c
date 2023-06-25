#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "../headers/utils.h"

void arg_check(int necessary_argc,int argc){
    if(argc<necessary_argc){
        fprintf(stderr,"There should be %d arguments\n",necessary_argc);
        exit(EXIT_FAILURE);
    }
    else if(argc>necessary_argc) printf("There are more than %d arguments\nIgnoring extra arguments...\n",necessary_argc);

}

void exit_on_error(int errnum,char* msg){
    fprintf(stderr,"%s: %s\n",msg,strerror(errnum));
    exit(EXIT_FAILURE);
}

void fail_check(int return_val,char* msg){
    if(return_val<0){
        exit_on_error(errno, msg);
    }
}

void fail_check_pthread(int return_val, char* msg){
    if(return_val!=0){
        exit_on_error(return_val, msg);
    }
}

void remove_first_n_chars(char* str,int n){
    int i=0;
    while(str[i+n]!='\0'){
        str[i]=str[i+n];
        i++;
    }
    str[i]='\0';
}

char* remove_until_delim(char* str, char delim){
    int i=0;
    char *retval=(char*)malloc(sizeof(char)*64);
    while(str[i]!=delim){
        retval[i]=str[i];
        i++;
    }
    retval[i] = '\0';
    remove_first_n_chars(str,i+1);
    return retval;
}

void set_sig_handler(void func ( int )){
    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(struct sigaction));
    sigint_action.sa_handler = *func;
    if(sigaction(SIGINT,&sigint_action,NULL)){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void get_timestamp(char *buffer){
   time_t rawtime;
   struct tm * timeinfo;
   time (&rawtime);
   timeinfo = localtime (&rawtime);
   strftime (buffer,32,"%G/%m/%d|%H:%M:%S",timeinfo);
}

pid_t get_pid(){
    pid_t process_id;
    char buffer[32] = "";
    readlink("/proc/self",buffer,sizeof(char)*32);
    process_id = atoi(buffer);
    return process_id;
}