#ifndef CHEFSLIB_H
#define CHEFSLIB_H



#define MILK_SEM_NAME "milk-sem"
#define FLOUR_SEM_NAME "flour-sem"
#define WALNUT_SEM_NAME "walnut-sem"
#define SUGAR_SEM_NAME "sugar-sem"

#define MF_SEM_NAME "mf-sem"
#define MW_SEM_NAME "mw-sem"
#define MS_SEM_NAME "ms-sem"
#define FW_SEM_NAME "fw-sem"
#define FS_SEM_NAME "fs-sem"
#define WS_SEM_NAME "ws-sem"

#define MUTEX_SEM_NAME "mutex-sem"

#define SHARED_MEM_NAME "/my-shared-mem"
struct shared_memory{
	char ingredients[3];
	int isMilk;
	int isWalnut;
	int isFlour;
	int isSugar;
};

#endif