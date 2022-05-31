#ifndef SERVER_Z_H
#define SERVER_Z_H

#define BD_MAX_CLOSE 8192

#define SHARED_MEM_NAME "/my-shared-mem"
#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_EMPTY_NAME "/sem-empty"
#define SEM_FULL_NAME "/sem-full"
#define QUEUE_CAP 100

struct Queue {
    int front, rear, size;
    unsigned capacity;
    char array[1000][1000];
    int busyChild;
};

int isFull(struct Queue*);
int isEmpty(struct Queue*);
void enqueue(struct Queue*, char*);
char* dequeue(struct Queue*);

void sigchldHandler(int);
void sigintHandler(int);
void sigusrHandler(int);
int becomeDaemon(int);
void myPrint(int, char*);
void getCofactor(int **, int** , int, int, int);
int determinantOfMatrix(int **, int);
int isInvertible(int **, int);
void createChildren(int,int,int[100],int);
int childFunc(int,int);

#endif