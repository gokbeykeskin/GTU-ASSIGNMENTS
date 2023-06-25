

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../headers/common.h"
#include "../headers/networking.h"
#include "../headers/utils.h"

void init_socket(SA_IN *server_addr,int server_port){
    (*server_addr).sin_family = AF_INET;
    (*server_addr).sin_addr.s_addr = inet_addr("127.0.0.1");
    (*server_addr).sin_port = htons(server_port);
}

void connect_to_socket(SA_IN *server_addr, int *server_socket){
    fail_check(*server_socket = socket(AF_INET, SOCK_STREAM, 0),"Initialize Socket");
    fail_check(connect(*server_socket, (struct sockaddr*)server_addr, sizeof(SA_IN)),"Connect to Server Socket");
}

void server_socket_init(SA_IN *server_addr,int *server_port,int *server_socket){
    init_socket(server_addr,*server_port);
    fail_check(*server_socket = socket(AF_INET, SOCK_STREAM, 0),"Initialize Server Socket");
    setsockopt(*server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    while(bind(*server_socket, (struct sockaddr*)server_addr, sizeof(SA_IN))==-1){
        close(*server_socket);
        init_socket(server_addr,*server_port);
        fail_check(*server_socket = socket(AF_INET, SOCK_STREAM, 0),"Initialize Server Socket");
        fail_check(setsockopt(*server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)),"Set Socket Option");
        *server_port+=1;
    }
    fail_check(listen(*server_socket, SERVER_BACKLOG),"Bind Socket");
}


