#ifndef NETWORKING_H
#define NETWORKING_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "../headers/server.h"
/*
Initializes socket information for a client  (IP etc.)
*/
void init_socket(SA_IN *server_addr,int server_port);
/*
Calls init_socket and then socket,bind,listen
*/
void server_socket_init(SA_IN *server_addr,int *server_port,int *server_socket);
void connect_to_socket(SA_IN *server_addr, int *server_socket);
#endif // NETWORKING_H