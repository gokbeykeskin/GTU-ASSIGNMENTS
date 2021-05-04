#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define fileName "list.txt"

typedef struct LinkedList{
	int data;
	struct LinkedList *next;
}LL;

typedef struct DiffValues{
	int n1,n2;
}DV;


int read_array(char *filename,int **intArray);     //these 2 functions returns the byte counter. 
int read_Linkedlist(char *filename,LL **root);	 //array and the linked list are call by referance values.
float * stats_array(int *intArray);
float * stats_linkedlist(LL *root);
float sqroot(float num); //math.h wasn't allowed on the assignment sheet.
DV * DifferentValue(LL *LList,int *array);


void main(void){
    clock_t start, end;
    int ArrayBytes,LLBytes;  //my byte counters are 60004 and 2000016 because they have 1 extra index (-1).
    double arrTime,LLTime,arrStatTime,LLStatTime,stats[4];
    int *arr;
    LL *integerList;
    int arrValue,LLValue,i=0,counter=1; //FOR SECOND PART.
    LL *LList,*iter;  //FOR SECOND PART.
    int *array; //FOR SECOND PART.
    start=clock();
    	ArrayBytes=read_array(fileName,&arr);
    end=clock();
    arrTime=((double)(end-start)) / CLOCKS_PER_SEC;
    start=clock();
    	LLBytes=read_Linkedlist(fileName,&integerList);
    end=clock();
    LLTime=((double)(end-start)) / CLOCKS_PER_SEC;
    printf("Memory used for read_array function (in bytes) is: %d\n",ArrayBytes);
    printf("Memory used for read_Linkedlist function (in bytes) is: %d\n",LLBytes);
    printf("----------------------------------------\n");
    printf("Time needed to execute function read_array is: %lf\n",arrTime);	//30 secs(more or less)
    printf("Time needed to execute function read_Linkedlist is: %lf\n",LLTime);//0.03 secs (more or less)
    printf("----------------------------------------\n");
    start=clock();
    	printf("Stats of the array:\nMin:%.0lf\nMax:%.0lf\nMean Value:%lf\nStandart Deviation:%lf\n",stats_array(arr)[0],stats_array(arr)[1],stats_array(arr)[2],stats_array(arr)[3]);
    end=clock();
    printf("----------------------------------------\n");
    arrStatTime=((double)(end-start)) / CLOCKS_PER_SEC;
    start=clock();
    	printf("Stats of the linked list:\nMin:%.0lf\nMax:%.0lf\nMean Value:%lf\nStandart Deviation:%lf\n",stats_linkedlist(integerList)[0],stats_linkedlist(integerList)[1],stats_linkedlist(integerList)[2],stats_linkedlist(integerList)[3]);
    end=clock();
    printf("----------------------------------------\n");
    LLStatTime=((double)(end-start)) / CLOCKS_PER_SEC;
    printf("Time needed to execute function stats_array (4 times) is %lf\n",arrStatTime);
    printf("Time needed to execute function stats_linkedlist (4 times) is %lf\n",LLStatTime);
    printf("----------------------------------------\n");
    printf("Linked list uses %d more bytes then array. Time difference between array and linked list is %lf(Nearly the same time).\nSo all in all using an array is much better in such cases.(Because we don't do shift operations etc.)\n",LLBytes-ArrayBytes,arrTime-LLTime);
    printf("\n***************** PART 2 *******************\n");
	printf("Input the array values(Positive numbers only.) Type -1 to stop.\n");
	do{ //dynamically filling the array with user inputs.
		scanf("%d",&arrValue);
		array=(int*)realloc(array,counter*sizeof(int));
		array[i]=arrValue;
		i++;
		counter++;
	}while(arrValue>0); 
	array[i]=-1;
	printf("Array is succesfully filled. Now Input the Linked List Values(Positive numbers only.) Type -1 to stop.\n");
	printf("Note: You should type exactly the same amount of numbers for linked list and array.\n");
	LList=(LL*)malloc(sizeof(LL));
	iter=LList;
	i=0;
	do{
		scanf("%d",&LLValue);
		iter->data=LLValue;
		iter->next=(LL*)malloc(sizeof(LL));
		iter=iter->next;
	}while(LLValue>0);
	iter->data=-1;
	printf("Linked List is succesfully filled.\n");
	i=0;
	while(DifferentValue(LList,array)[i].n1>0 && DifferentValue(LList,array)[i].n2>0){
		printf("Different values: %d, %d\n",DifferentValue(LList,array)[i].n1,DifferentValue(LList,array)[i].n2);
		i++;
	}
	if(DifferentValue(LList,array)[i].n1<0 && DifferentValue(LList,array)[i].n2<0) printf("All the values are the same.");
}

int read_array(char *filename,int **intArray){ //reads the file and creates an array with the numbers in it.
	int i,counter=0,input,byteCounter=0;
	char comma;
	FILE *fp;
	fp=fopen(filename,"r");
	*intArray=(int *)malloc(sizeof(int));
	do{
		fscanf(fp,"%d",&input);
		fscanf(fp,"%c",&comma);
		counter++;
		*intArray=(int *)realloc(*intArray,counter*sizeof(int));
		(*intArray)[counter-1]=input;
		byteCounter++;
	}while(!feof(fp));
	(*intArray)[counter-1]=-1;
	byteCounter=byteCounter * 4; //size of int is 4
	return byteCounter;
}
int read_Linkedlist(char *filename,LL **root){ //reads the file and creates a linked list with the numbers in it.
	int input,byteCounter=0;
	char comma;
	FILE *fp;
	fp=fopen(filename,"r");
	*root=(LL*)malloc(sizeof(LL));
	LL *iter;
	iter=*root;
	do{
		fscanf(fp,"%d",&input);
		fscanf(fp,"%c",&comma);
		iter->data = input;
		iter->next=(LL*)malloc(sizeof(LL));
		iter=iter->next;
		byteCounter++;
	}while(!feof(fp));
	byteCounter=byteCounter * 8; //size of LL is 8
	//iter=NULL;
	iter->data=-1;
	return byteCounter;
}

float * stats_array(int *intArray){
	int min=9999999,max=0,i=0;
	double mean=0.0,stddev=0.0;
	float *statsArr;
	statsArr=(float *)calloc(4,sizeof(float));
	while(intArray[i]>0){
		mean+=(double)intArray[i];
		if(intArray[i]<min) min=intArray[i];
		if(intArray[i]>max) max=intArray[i];
		i++;
	}
	mean=mean/(i);
	i=0;
	while(intArray[i]>0){
		stddev+=(intArray[i]-mean)*(intArray[i]-mean);
		i++;
	}
	stddev=stddev/i;
	stddev=sqroot(stddev);
	statsArr[0]=min;
	statsArr[1]=max;
	statsArr[2]=mean;
	statsArr[3]=stddev;
	return statsArr;
}

float * stats_linkedlist(LL *root){
	int min=9999999,max=0,i=0,counter=0;
	double mean=0.0,stddev=0.0;
	float *statsLL;
	LL *iter;
	statsLL=(float *)calloc(4,sizeof(float));
	iter=root;
	while(iter->next->data!=-1){
		mean+=(double)(iter->data);
		if(iter->data<min) min=iter->data;
		if(iter->data>max) max=iter->data;
		counter++;
		iter=iter->next;
	}
	mean=mean/(counter);
	while(root->next->data!=-1){
		stddev+=(root->data-mean)*(root->data-mean);
		root=root->next;
		
	}
	stddev=stddev/counter;
	stddev=sqroot(stddev);
	statsLL[0]=min;
	statsLL[1]=max;
	statsLL[2]=mean;
	statsLL[3]=stddev;
	return statsLL;
}

float sqroot(float num){ //math.h wasn't allowed on the assignment sheet so I made a simple square root function.
	float sqrt,temp=0.0;
	sqrt=num/2;
	while(sqrt!=temp){
		temp=sqrt;
		sqrt=(num/temp +temp)/2;
	}
	return sqrt;
}

DV * DifferentValue(LL *LList,int *array){ // finds the different values in an array and a linked list and holds these values in a dynamically created structure.
	int i=0,counter=2,k=0;
	DV *values;
	values=(DV*)malloc(sizeof(int));
	while(array[i]>0){
		if(array[i]!=LList->data){
			values[k].n1=array[i];
			values[k].n2=LList->data;
			values=(DV*)realloc(values,counter*sizeof(DV));
			counter++;
			k++;
		}
		LList=LList->next;
		i++;
	}
	values=(DV*)realloc(values,(counter+1)*sizeof(DV));
	values[k].n1=-1;
	values[k].n2=-1;
	return values;
}
