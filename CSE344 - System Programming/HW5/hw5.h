#ifndef HW5_H
#define HW5_H

struct info{
	int n,m;
	int** matrix1;
	int** matrix2;
	int** resMatrix;
};

struct Timestamp
{
    time_t seconds;
    long milliseconds;
    char timestring[74];
};

void* thread_func(void*);
struct Timestamp getTimestamp();
void sigintHandler(int);
int isPowerOfTwo(int);
#endif