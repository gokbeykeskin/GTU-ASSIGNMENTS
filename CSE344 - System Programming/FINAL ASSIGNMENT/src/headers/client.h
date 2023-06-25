#ifndef CLIENT_H
#define CLIENT_H


/*
This size is the initial size of the transaction requests of the client.
Size can grow if the client needs more transactions.
*/
#define INITIAL_REQ_SIZE 128
/*
Sends requests for the client.
There is one thread for each request.
*/
void* thread_function(void* request_ptr);
#endif