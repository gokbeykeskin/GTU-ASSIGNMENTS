#ifndef SERVER_H
#define SERVER_H

#include "../headers/common.h"

#define MIN_SERVER_THREAD_AMOUNT 5

/*
Holds the information about a servant for later accesses.
*/
struct servant{
    int port;
    int city_amt;
    int pid;
    char** cities;
};


/*
Initializes (create,bind,listen) the server.
Creates the thread pool and starts the threads.
*/
void init_all_sv(int server_port,SA_IN server_addr);

/*
Handles the client and servants requests.
*/
void handle_connection(void* client_socket_ptr);

/*
    Dequeues connection requests and passes it to handle_connection.
*/
void* thread_function(void* arg);

/*
Saves the info of the servant
*/
void set_servant_info(char* buf);

void exit_handler();
void sigint_handler(int signum);
#endif