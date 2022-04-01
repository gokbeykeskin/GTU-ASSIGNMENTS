#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
void menu ();
char decrypt_numbers (int number);
void decrypt_and_print (char* file_path);
void deep_decrypt_and_print (char* file_path);
void track_machine (); //WHEN YOU INPUT R TOO OFTEN, IT SOMETIMES CANT CHANGE THE ENEMY LOCATION BUT STILL WORKS FINE.
void refresh_position (int *X, int *Y, double *D, double *R);
void encrypt_messages (char* file_path);

void main(void){
	menu();
}

void menu(){
	int option=0;
	char *fpath1= "encrypted_p1.img"; // DECRYPT FUNCTION FILE PATH
	char *fpath2= "encrypted_p2.img"; // DEEP DECRYPT FUNCTION FILE PATH
	char *fpath3="decrypted_p4.img"; // ENCRYPT FUNCTION FILE PATH
	while(option!=5){
		printf("\n1-) Decrypt and print encrypted_p1.img\n2-) Decrypt and print enrypted_p2.img\n3-)Switch on the tracking machine\n4-) Encrypt the message\n5-) Switch off\n\nPlease make your choice:\n");
		scanf("%d",&option);
		switch(option){
		case 1:
			decrypt_and_print(fpath1);
			option=0;
			break;
		case 2:
			deep_decrypt_and_print(fpath2);
			option=0;
			break;
		case 3:
			track_machine(); //WHEN YOU INPUT R TOO OFTEN, IT SOMETIMES CANT CHANGE THE ENEMY LOCATION BUT IF YOU WAIT A FAIR SECONDS OF TIME BEFORE INPUTING IT WORKS FINE.
			option=0;
			break;
		case 4:
			encrypt_messages(fpath3);
			option=0;
			break;
		case 5:
			option=5;
			break;
		}
	}
}

char decrypt_numbers (int number){
	char result;
	switch(number){
		case 0:
			result=' ';
			break;
		case 1:
			result='-';
			break;
		case 2:
			result='_';
			break;
		case 3:
			result='|';
			break;
		case 4:
			result='/';
			break;
		case 5:
			result='\\';
			break;
		case 6:
			result='O';
			break;
		default:
			result=' ';
			break;
	}
	return result;
}

void decrypt_and_print (char* file_path){
	char num;
	FILE *fp;
	fp = fopen(file_path,"r");
	do{
		fscanf(fp,"%c",&num);
		if(num!='\n')
			printf("%c",decrypt_numbers(num-'0')); //I substract 0 (do an arithmetic operation) from num to make it an integer.
		else
			printf("\n");
	}while(!feof(fp));
	fclose(fp);
}

void deep_decrypt_and_print(char* file_path){
	char num1,num2,num3;
	int inum1,inum2,inum3,temp=0,result; //inum is integer versions of the scanned characters.
	FILE *fp;

	fp = fopen(file_path,"r");
	
		fscanf(fp,"%c",&num1);
		fscanf(fp,"%c",&num2);
		while(!feof(fp)){
			fscanf(fp,"%c",&num3);
			inum1=(num1-'0'); //I USED THIS METHOD TO MAKE INTEGERS FROM CHARACTERS INSTEAD OF TYPECASTING.
			inum2=(num2-'0');
			inum3=(num3-'0');

			if(num1!='\n'){ //when its not end of the line
				printf("%c",decrypt_numbers(result));
				temp=inum1+inum2+inum3;
				result=temp%7;
				temp=0;
			}
			else{
				printf("\n");
			}
			num1=num2;
			num2=num3;
		}
	fclose(fp);
}

void track_machine (){
	int xpos,ypos=1,enx=1,eny=1;
	double d=0.0,c=8.485281; // 8.485281 IS THE INITIAL DISTANCE BETWEEN ENEMY CAMP TO OUR CAMP.
	char command='0';
		while(command!='E'){
			while((enx==6 && eny==6) || (enx==0) || (eny==0)) //IF ONE OF THE RANDOMLY GENERATED NUMBERS IS 0 (OUTSIDE OF THE SYSTEM)
				refresh_position(&enx,&eny,&d,&c);            // OR BOTH OF THEM ARE 6 (SAME POSITION WITH OUR CAMP) REFRESH IT UNTIL FINDING A VALID COORDINATE SET.
			for(ypos=1;ypos<=11;ypos++){ //rows
				for(xpos=1;xpos<=11;xpos++){//columns
					if(xpos==6 && ypos==6)
						printf("O\t");
					else if(xpos==enx && ypos==eny)
						printf("E\t");
					else
						printf(".\t");
				}
				printf("\n");
			}
		printf("Enemies X POSITION: %d, Y POSITION: %d,Displacement: %lf, Distance to our camp:%lf\n",enx,eny,d,c);
		printf("Command waiting...:");
		if(command=='R')
			refresh_position(&enx,&eny,&d,&c);
		scanf(" %c",&command);
		printf("\n");
		}
}

void refresh_position (int *X, int *Y, double *D, double *R){
	int prevx,prevy;
	srand(time(NULL));
	prevx= *X;
	prevy= *Y;

	*X=rand()%11; // GENERATES A NUMBER IN RADIUS 0 TO 11.
	*Y=rand()%11;
	
	*D=sqrt(pow(*X-prevx,2)+pow(*Y-prevy,2)); //DISTANCE BETWEEN TWO POINTS FORMULA.
	*R=sqrt(pow(*X-6,2)+pow(*Y-6,2));		  //DISTANCE BETWEEN TWO POINTS FORMULA.
}

void encrypt_messages (char* file_path){
	char sym; //symbols
	int temp=0,num1=0,num2=0,num3=0,result;
	FILE *fp1,*fp2;
	fp1=fopen(file_path,"r");
	fp2=fopen("encrypted_p4.img","w");
	while(!feof(fp1)){
		fscanf(fp1,"%c",&sym);
		switch(sym){
			case ' ':
				temp= 0;
				break;
			case '-':
				temp= 1;
				break;
			case '_':
				temp= 2;
				break;
			case '|':
				temp= 3;
				break;
			case '/':
				temp= 4;
				break;
			case '\\':
				temp= 5;
				break;
			case 'O':
				temp= 6;
				break;
			case '\n':
				temp=-1;
				break;
			default:
				temp=0;
				break;
		}	
		if(temp!=-1){
			num1=num2;
			num2=num3;
			num3=temp;
			result=(num1+num2+num3)%7;
			fprintf(fp2,"%d",result);

		}
		else{
			fprintf(fp2,"%c",'\n');
			num1=0;
			num2=0;
			num3=0;
		}
	}
	fclose(fp1);
	fclose(fp2);
}
