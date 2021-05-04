#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define arraysize 500
#define N 8
#define maxstrsize 200
/*
PART 2 DOESN'T WORK AS EXPECTED. IT ONLY FILLS THE TABLE ONE TIME AND DOESN'T PRINTS THE ORIENTATIONS.
*/
void menu();
void generate_hofstadters_sequence (int *arr, int n);
int find_max (int arr[], int index, int max_value);
int sum_array (int arr[]);
double std_array (int arr[], double *mean, int n, int index);
void install_pipes (int visited[N][N], int x, int y, int orientation,int counter);
char* remove_duplicates (char* str);

void main(void){
	srand(time(NULL));
	int option,x=0,y=0,visited[N][N]={0},orientation=1,counter=0;
	char str[maxstrsize];
	do{
		printf("Welcome to Homework 8. please chose one of the parts to continue\n\n--------------------------\n\n1) Execute Part 1\n2) Execute Part 2\n3) Execute Part 3\n4) Exit\n");
		scanf("%d",&option);
		switch(option){
			case 1:
				menu();
				break;
			case 2:
				install_pipes(visited,x,y,orientation,counter);
				break;
			case 3:
				printf("Please enter a string(ONLY ENGLISH CHARACTERS): ");
				scanf(" %[^\n]",str);
				printf("Successfully removed: %s\n",remove_duplicates(str));
				break;

		}	
	}while(option!=4);
}

void menu(){
	int hofstadter[arraysize];
	int option,n,maxvalue=0,index=0;
	double mean;

	do{
		printf("Please make your choice:\n\n--------------------------\n\n1) Fill Array\n2) Find Biggest Number\n3) Calculate Sum\n4) Calculate Standart Deviation\n5) Exit\n");
		scanf("%d",&option);
		switch(option){
			case 1:
				printf("Please enter the maxiumum index of the Hofstadter's sequence you would like to put in array:");
				scanf("%d",&n);
				generate_hofstadters_sequence(hofstadter,n);
				printf("Array has been successfully filled.");
				break;
			case 2:
				printf("Biggest number in the array is: %d\n",find_max(hofstadter,index,maxvalue));
				break;
			case 3:
				printf("Sum of all indexes of the array is:%d\n",sum_array(hofstadter));
				break;
			case 4:
				mean=0;
				printf("Mean Value of the array: %.5lf\nStandart Deviation of the array: %.5lf\n",mean,std_array(hofstadter,&mean,n,index));
				break;
		}
	}while(option!=5);
}

void generate_hofstadters_sequence (int *arr, int n){
	arr[0]=1;
	arr[1]=1;
	int i;
	for(i=2;i<n;i++){
		arr[i]=arr[i-arr[i-1]]+arr[i-arr[i-2]];
	}
	for(i=n;i<arraysize;i++){
		arr[i]=0;
	}
}

int find_max (int arr[], int index, int max_value){
	if(index<arraysize){
		if(arr[index]>max_value) max_value=arr[index];
		index++;
		find_max(arr,index,max_value);
	}
	else return max_value;
}

int sum_array (int arr[]){
	int sum=0;
	if(arr[0]==0) return sum;
	else sum=arr[0]+sum_array(&arr[1]);
}

double std_array (int arr[], double *mean, int n, int index){
	double temp=0;
	double sum=0;
	if(*mean!=0){
		if(index==n){
			return 0;
		}
		else{
			temp=pow(arr[index]-*mean,2)+std_array(arr,mean,n,index+1);
		}
	}
	else{
		if(arr[index]!=0){
			sum=arr[index]+std_array(arr,mean,n,index+1);
			temp=sum;
			if(index==0){
				*mean=sum/n;
				temp=sqrt(std_array(arr,mean,n,0)/(n-1));
			}
		}
		//else return 0;
	}
	return temp;
}
void install_pipes (int visited[N][N], int x, int y, int orientation,int counter){
	int oldx=x,oldy=y,flag=0;
	//printf("O%d,C%d\n",orientation,counter);
	if(counter<63){
		if(counter==0) visited[0][0]=1;
		if(visited[x+2][y+1]==0 && (x+2>=0 && y+1>=0 && x+2<=7 && y+1<=7)&&orientation==1){
			x+=2;
			y+=1;
		}
		else if(visited[x+1][y-2]== 0 && (x+1>=0 && y-2>=0 && x+1<=7 && y-2<=7)&&orientation==2){
			x+=1;
			y-=2;
		}
		else if(visited[x-2][y-1]==0 && (x-2>=0 && y-1>=0 && x-2<=7 && y-1<=7)&&orientation==3){
			x-=2;
			y-=1;
		}
		else if(visited[x-1][y+2]==0 && (x-1>=0 && y+2>=0 && x-1<=7 && y+2<=7)&&orientation==4){
			x-=1;
			y+=2;
		}
		else if(visited[x+1][y+2]==0 && (x+1>=0 && y+2>=0 && x+1<=7 && y+2<=7)&&orientation==5){
			x+=1;
			y+=2;
		}
		else if(visited[x+2][y-1]==0 && (x+2>=0 && y-1>=0 && x+2<=7 && y-1<=7)&&orientation==6){
			x+=2;
			y-=1;
		}
		else if(visited[x-1][y-2]==0 && (x-1>=0 && y-2>=0 && x-1<=7 && y-2<=7)&&orientation==7){
			x-=1;
			y-=2;
		}
		else if(visited[x-2][y+1]==0 && (x-2>=0 && y+1>=0 && x-2<=7 && y+1<=7)&&orientation==8){
			x-=2;
			y+=1;
		}
		else if(orientation>8){
			visited[x][y]=0;
			flag=1;
		}
		else{
			install_pipes(visited,x,y,orientation+1,counter);
			flag=1;
		}
		if(counter<63&&flag==0){
			visited[x][y]=1;
			install_pipes(visited,x,y,1,counter+1);
			if(visited[x][y]!=1) install_pipes(visited,oldx,oldy,orientation+1,counter);
		}
	}
}

char* remove_duplicates (char* str){
	if(*(str+1)!='\0'){ //If there is a character after this one.
		if(*str==*(str+1)){ //If the current character and the next character are the same
			*(str+1)=*(str+2); //move the third character to second
			remove_duplicates(str+1);
		}
		remove_duplicates(str+1);
	}
	if(*str==*(str-1)) *(str)=*(str+1); //if there are 4 consecutive duplicates at the beginning this deletes the one extra character.
	return str;
}
