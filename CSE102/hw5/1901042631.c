#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buffsize 500 //max size of news
void menu();

void read_news(char buffer[500], char file_path[11], int is_Only_Title);
void append_file(char* file_path, char c);

double f_func(int x);
double g_func(double f(int x), int a);
void read_magic_numbers(char buffer_magic[10], char buffer_news[500]);

void main(void){
	menu();
}
/*
I made file paths 11 characters long instead of 10 because after news/1.txt
there is a null character('\0') and in total it has 11 characters.
*/
void menu(){
	char buffer[buffsize];
	char buffer_magic[10];
	char fp1[11] = "news/1.txt"; //fp is file path.
	char fp2[11] = "news/2.txt";
	char fp3[11] = "news/3.txt";
	char fp4[11] = "news/4.txt";
	char fpreaded[19]="readed_news_id.txt";
	char opcont='y',opmain,readed[5]; //opcont is Do you want to contiune option(yes no),opmain is what do you want to do option(a b c), readed array holds the readed values.
	int opa=0,opdecrypt,readagain,flag=0;
	FILE* file_pointer;
	file_pointer=fopen(fpreaded,"r");
	printf("********Daily Press********\n");
	printf("Today's news are listed for you:\n\n");
	
	printf("Title of the 1. news: ");
	read_news(buffer,fp1,1);
	printf("\nTitle of the 2. news: ");
	read_news(buffer,fp2,1);
	printf("\nTitle of the 3. news: ");
	read_news(buffer,fp3,1);
	printf("\nTitle of the 4. news: ");
	read_news(buffer,fp4,1);

	while(opcont=='y'){ //in a loop until user presses something other then y.
		printf("\nWhat do you want to do?\na. Read a new\nb. List the readed news\nc. Get decrypted information from the news\n");
		scanf(" %c",&opmain);
		switch(opmain){
			case 'a':
				printf("Which news text would you like to read?\n");
				scanf("%d",&opa);
				if(opa==1){
					fgets(readed,5,file_pointer);
					for(int i=0;i<4;i++){ //checks the readed_news 4 times if there is a 1. (does the same thing for 2,3 and 4)
						if (readed[i]=='1'){
							printf("This new is readed. Do you want to read it again? Yes(1)/No(0)"); 
							scanf("%d",&readagain);
							if(readagain==1) read_news(buffer,fp1,0);
							else if(readagain==0) i=4; //if the user doesn't wants to read it again breaks the loop. (does the same thing for 2,3 and 4) 
							flag=1;
							i=4;
						}
						else flag=0; //if the news is not readed before (does the same thing for 2,3 and 4) 
					}
					if(flag==0){ //if the news is not readed before  
						read_news(buffer,fp1,0);
						append_file(fpreaded,'1');
					}
				}
				if(opa==2){
					fgets(readed,5,file_pointer);
					for(int i=0;i<4;i++){
						if(readed[i]=='2'){
							printf("This new is readed. Do you want to read it again? Yes(1)/No(0)");
							scanf("%d",&readagain);
							if(readagain==1) read_news(buffer,fp2,0);
							else if(readagain==0) i=4;
							flag=1;
							i=4;
						}
						else flag=0;
					}
					if(flag==0){
						read_news(buffer,fp2,0);
						append_file(fpreaded,'2');
					}
				}
				if(opa==3){
					fgets(readed,5,file_pointer);
					for(int i=0;i<4;i++){
						if(readed[i]=='3'){
							printf("This new is readed. Do you want to read it again? Yes(1)/No(0)");
							scanf("%d",&readagain);
							if(readagain==1) read_news(buffer,fp3,0);
							else if(readagain==0) i=4;
							flag=1;
							i=4;
						}
						else flag=0;
					}
					if(flag==0){
						read_news(buffer,fp3,0);
						append_file(fpreaded,'3');
					}
				}
				if(opa==4){
					fgets(readed,5,file_pointer);
					for(int i=0;i<4;i++){
						if(readed[i]=='4'){
							printf("This new is readed. Do you want to read it again? Yes(1)/No(0)");
							scanf("%d",&readagain);
							if(readagain==1) read_news(buffer,fp4,0);
							else if(readagain==0) i=4;
							flag=1;
							i=4;
						}
						else flag=0;
					}
					if(flag==0){
						read_news(buffer,fp4,0);
						append_file(fpreaded,'4');
					}
				}
				fseek(file_pointer, 0, SEEK_SET); //reset the file pointer.
				break;
			case 'b':
				for(int i=0;i<4;i++){
					fscanf(file_pointer,"%s",readed);
					if(readed[i]=='1') printf("%c. new is readed.\n",readed[i]);
					else if(readed[i]=='2') printf("%c. new is readed.\n",readed[i]);
					else if(readed[i]=='3') printf("%c. new is readed.\n",readed[i]);
					else if(readed[i]=='4') printf("%c. new is readed.\n",readed[i]);
				}
				break;
			case 'c':
				printf("Which news would you like to decrypt?:");
				scanf("%d",&opdecrypt);
				if(opdecrypt==1){
					read_news(buffer,fp1,0);
					printf("Number of tests performed:"); 
					read_magic_numbers(buffer_magic,buffer);
				}
				else if(opdecrypt==2){
					read_news(buffer,fp2,0);
					printf("Number of sick people:"); 
					read_magic_numbers(buffer_magic,buffer);
				}
				else if(opdecrypt==3){
					read_news(buffer,fp3,0);
					printf("Number of deaths:"); 
					read_magic_numbers(buffer_magic,buffer);
				}
				else if(opdecrypt==4){
					read_news(buffer,fp4,0);
					printf("Expected number of sick people:"); 
					read_magic_numbers(buffer_magic,buffer);
				}
		}
		printf("Do you want to continue Yes(y)/No(n)\n");
		scanf(" %c",&opcont);
	}
	printf("Goodbye!\n");
	fclose(file_pointer);
}

void read_news(char buffer[buffsize], char file_path[11], int is_Only_Title){
	FILE *fp;
	fp= fopen(file_path,"r");
	int i=0;
	if(is_Only_Title==1){
		fgets(buffer,500,fp);
		printf("%s\n",buffer);
	}
	else{
		while(!feof(fp)){
			fscanf(fp,"%c",&buffer[i]);
			i++;
		}
		buffer[i-1]='\0'; //end of string.
		fseek(fp, 0, SEEK_SET); //reset the file pointer. //resets the pointer.
		printf("%s\n",buffer);
	}
	fclose(fp);
}

void append_file(char* file_path, char c){
	FILE *fpointer;
	fpointer= fopen(file_path,"a+");
	int flag=0; //standart condition (there are no 1-2-3-4 on the readed news file)
	char buffer;
	switch(c){
		case '1':
			while(!feof(fpointer)){
				fscanf(fpointer,"%c",&buffer);
				if(buffer=='1') flag=1;		//if there is a 1 in the readed news file make the flag 1.
			}
			if(flag!=1) fprintf(fpointer,"%c",'1');
			break;
		case '2':
			while(!feof(fpointer)){
				fscanf(fpointer,"%c",&buffer);
				if(buffer=='2') flag=1;		//if there is a 2 in the readed news file make the flag 1.
			}
			if(flag!=1) fprintf(fpointer,"%c",'2');
			break;
		case '3':
			while(!feof(fpointer)){
				fscanf(fpointer,"%c",&buffer);
				if(buffer=='3') flag=1;		//if there is a 3 in the readed news file make the flag 1.
			}
			if(flag!=1) fprintf(fpointer,"%c",'3');
			break;
		case '4':
			while(!feof(fpointer)){
				fscanf(fpointer,"%c",&buffer);
				if(buffer=='4') flag=1;		//if there is a 4 in the readed news file make the flag 1.
			}
			if(flag!=1) fprintf(fpointer,"%c",'4');
			break;
	}
	fclose(fpointer);
}

void read_magic_numbers(char buffer_magic[10], char buffer_news[500]){
	int counter=0,num;//counter counts the number of magic numbers.
	double sum=0; 
	for(int i=0;i<500;i++){
		if (buffer_news[i]=='#'){
			buffer_magic[counter]=buffer_news[i+1];
			counter++;
		}
	}
	for(int i=0;i<counter;i++){
			num=buffer_magic[i]-'0'; //casting char to int.
			sum+=g_func(f_func,num);
	}
	printf("%.2lf\n",sum);
}
double f_func(int x){
	return x*x*x-x*x+2;
}
double g_func(double f(int x), int a){
	return f(a)*f(a);
}
