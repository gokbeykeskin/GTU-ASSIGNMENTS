#ifndef HW4_H
#define HW4_H



#define SEM_KEY "/tmp/"

struct consumer_args{
	int n;
	int cons_id;
};

struct Timestamp
{
    time_t seconds;
    long milliseconds;
    char timestring[74];
};

struct Timestamp getTimestamp();
void sigintHandler(int);
void* consumer(void*);
void* supplier(void*);
#endif