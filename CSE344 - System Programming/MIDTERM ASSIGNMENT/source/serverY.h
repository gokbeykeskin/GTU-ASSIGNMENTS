#ifndef SERVER_Y_H
#define SERVER_Y_H

#include <sys/types.h>
#include <signal.h>

/* Maximum file descriptors to close if
   sysconf(_SC_OPEN_MAX) is indeterminate */
#define BD_MAX_CLOSE 8192

typedef struct childProcess{
   pid_t pid;
   int busy;
   int writeEnd;
} child;

void siguserHandler(int, siginfo_t *, void *);
void siguser2Handler(int);
void sigchldHandler(int);
void sigintHandler(int);
int becomeDaemon(int);
int createServerZ(int,int,int,int*);
void createChildren(int, child*,int);
int childFunc(int,int);
void getCofactor(int **, int**, int, int, int);
int determinantOfMatrix(int **, int );
int isInvertible(int **, int);
void myPrint(int , char* );



#endif
