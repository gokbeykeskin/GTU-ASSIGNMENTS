#ifndef CHEFSLIB_H
#define CHEFSLIB_H

#define SHARED_MEM_NAME "/my-shared-mem"
#define SEM_SHMEM_NAME "/sem-shmem"
struct shared_memory{
	char ingredients[3];
	int isMilk;
	int isWalnut;
	int isFlour;
	int isSugar;
};

#endif