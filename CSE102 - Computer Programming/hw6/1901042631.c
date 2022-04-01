/*
NOTES: 
	COMPILER GIVES A WARNING BECAUSE I USED A gets FUNCTION,IDK WHY, PROGRAM WORKS FINE.
	YOU CAN MAKE LINES 277 TO 282 COMMENT TO MAKE GAME EASIER. (DELETES THE RANDOM CHARACTERS.)
*/
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>

void menu();
void randword(char buffer[10][20]); //gets a random word from the words.txt.
void wordgenerate0(char words[10][20],char table[20][20],int a); //LEFT TO RIGHT.
void wordgenerate1(char words[10][20],char table[20][20],int a); //RIGHT TO LEFT.
void wordgenerate2(char words[10][20],char table[20][20],int a); //UP TO DOWN.
void wordgenerate3(char words[10][20],char table[20][20],int a); //DOWN TO UP.
void wordgenerate4(char words[10][20],char table[20][20],int a); //RIGHTDOWN.
void wordgenerate5(char words[10][20],char table[20][20],int a); //RIGHTUP.
void wordgenerate6(char words[10][20],char table[20][20],int a); //LEFTUP.
void wordgenerate7(char words[10][20],char table[20][20],int a); //LEFTDOWN.
void gametable(); //generates the table and puts random words.
void gameplay(char table[20][20],char words[10][20]); //checks the user input, if it's right makes the word uppercase and keeps the gametable updated.

void main(void){
	menu();
}

void menu(){
	char words[10][20];
	randword(words);
	gametable(words);
}

void randword(char buffer[10][20]){  //an array of 10 words, each word can have 20 characters at max.
	int randnum;
	int a,b;
	FILE *fp;
	fp=fopen("words.txt","r");
	srand(time(NULL));
	char wordlist[100][20]; //an array of 100 words, each word can have 20 characters at max
	for(a=0;a<100;a++){
		fscanf(fp,"%19s",wordlist[a]); //wordlist contains every word in words.txt
	}
	for(b=0;b<10;b++){//we need 10 words
		randnum=rand()%101; //generates a random number 1 to 100
		strcpy(buffer[b],wordlist[randnum]); //copies a random word from wordlist and puts it in the buffer.
	}
}

void wordgenerate0(char words[10][20],char table[20][20],int a){ //left to right
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy][randlocx+k]!='*'||(randlocx+strlen(words[a]))>=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b][c+k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate1(char words[10][20],char table[20][20],int a){ //right to left
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy][randlocx-k]!='*'||(randlocx+strlen(words[a]))<=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b][c-k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate2(char words[10][20],char table[20][20],int a){ //up to down
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy+k][randlocx]!='*'||(randlocx+strlen(words[a]))>=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b+k][c]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate3(char words[10][20],char table[20][20],int a){ //down to up
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy-k][randlocx]!='*'||(randlocx+strlen(words[a]))<=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b-k][c]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate4(char words[10][20],char table[20][20],int a){ //right down.
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy+k][randlocx+k]!='*'||(randlocx+strlen(words[a]))>=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b+k][c+k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate5(char words[10][20],char table[20][20],int a){ //right up.
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy-k][randlocx+k]!='*'||(randlocx+strlen(words[a]))>=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b-k][c+k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate6(char words[10][20],char table[20][20],int a){ //left up.
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy-k][randlocx-k]!='*'||(randlocx+strlen(words[a]))<=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b-k][c-k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}
void wordgenerate7(char words[10][20],char table[20][20],int a){ //left down.
	int b,c,k;
	int randlocx,randlocy; //random locations and direction.
	randlocx=rand()%20;
	randlocy=rand()%20;
		for(b=0;b<20;b++){ //y axis
			for(c=0;c<20;c++){ //x axis
				for(k=0;k<strlen(words[a]);k++){
					while(table[randlocy+k][randlocx-k]!='*'||(randlocx+strlen(words[a]))<=20){
							randlocx=rand()%20;
							randlocy=rand()%20;
					}
				}
				if(b==randlocy && c==randlocx){
					for(k=0;k<strlen(words[a]);k++){
							table[b+k][c-k]=words[a][k];
					}
					b=20;
					c=20;
				}
			}
		}
}

void gametable(char words[10][20]){
	int a,b;
	char table[20][20];
	char randomchar;
	int randdir;
	srand(time(NULL));
	
	for(a=0;a<20;a++){ //fills the table with stars to make checkings easier before adding the words.
		for(b=0;b<20;b++){
			table[a][b]='*';
		}
	}

	for(a=0;a<10;a++){ //runs 10 times because there are 10 words.
		randdir=rand()%8; //random direction
		switch(randdir){
			case 0:
				wordgenerate0(words,table,a);
				break;
			case 1:
				wordgenerate1(words,table,a);
				break;
			case 2:
				wordgenerate2(words,table,a);
				break;
			case 3:
				wordgenerate3(words,table,a);
				break;
			case 4:
				wordgenerate4(words,table,a);
				break;
			case 5:
				wordgenerate5(words,table,a);
				break;
			case 6:
				wordgenerate6(words,table,a);
				break;
			case 7:
				wordgenerate7(words,table,a);
				break;
		}
	}
	for(a=0;a<20;a++){ //fills the empty places with random chars.
		for(b=0;b<20;b++){
			randomchar=122-rand()%26;
			if(table[a][b]=='*') table[a][b]=randomchar;
		}
	}

	//below this, the program numerates the table like chess board.
	for(a=0;a<20;a++){
		for(b=0;b<20;b++){
			printf("%c ",table[a][b]);
		}
		printf("|%d  ",a+1); //vertical numbers.
		printf("\n");
	}
	for(a=0;a<20;a++) printf("_ "); //horizontal line.
	printf("\n");
	for(a=65;a<85;a++) printf("%c ",a); //horizontal characters A to T. 
	printf("THE WORDS YOU SHOULD FIND: ");
	for(a=0;a<10;a++){
	printf("%s|",words[a]);
	}
	printf("\n");
	gameplay(table,words);
}

void gameplay(char table[20][20],char words[10][20]){
	char word[20],str[15],exitcondition[15]="exit";
	char xposchar;
	int ypos,xpos,flag1,flag2,flagsuccess,counter=0;
	int i,k;

	while(counter<10){
		flagsuccess=0;
		flag2=0;
		flag1=1;
		printf("Write the position of the word first, then write the word(ex:E6 ryhme):");
		gets(str);
		if(strncmp(str,exitcondition,4)==0){
			printf("Until next time.\n");
			break;
		}
		else sscanf(str,"%c%d %s",&xposchar,&ypos,word);
		xpos= xposchar-65; //A is 0 T is 19 
		for(i=0;i<10;i++){
			if(strcmp(words[i],word)==0) flag2=1; //if the written word is one of the words in random word list.
		}

		//left to right control -----------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){ 
			if(table[ypos-1][xpos+i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1][xpos+i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//right to left control---------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1][xpos-i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1][xpos-i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//up to down control-------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1+i][xpos]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1+i][xpos] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//down to up control----------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1-i][xpos]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1-i][xpos] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//RIGHTDOWN CROSS CONTROL--------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1+i][xpos+i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1+i][xpos+i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//RIGHTUP CROSS CONTROL-----------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1-i][xpos+i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1-i][xpos+i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//LEFTUP CROSS CONTROL------------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1-i][xpos-i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1-i][xpos-i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//LEFTDOWN CROSS CONTROL------------------------------------------------------------------------------------------------------------
		for(i=0;i<strlen(word);i++){
			if(table[ypos-1+i][xpos-i]!=word[i]) flag1=0; //if there is a different letter
		}
		if(flag1==1 && flag2==1){
			for(i=0;i<strlen(word);i++){
				table[ypos-1+i][xpos-i] -=32; //make the word uppercase.
			}
			flagsuccess=1;
			counter++;
		}
		flag1=1;
		//-----------------------------------------------------------------------------------------------------------------------------------
		for(i=0;i<20;i++){
			for(k=0;k<20;k++){
				printf("%c ",table[i][k]);
			}
			printf("|%d  ",i+1); //vertical numbers.
			printf("\n");
		}
		for(i=0;i<20;i++) printf("_ "); //horizontal line.
		printf("\n");
		for(i=65;i<85;i++) printf("%c ",i); //horizontal characters A to T. 
		printf("THE WORDS YOU SHOULD FIND: ");
		for(i=0;i<10;i++){
		printf("%s|",words[i]);
		}
		printf("\n");
		if(flagsuccess!=1) printf("***Wrong position or there is no such a word on the wordlist.***\n");
	}
	printf("Well Done.\n");
}