%{
#include <stdio.h>
#include <math.h>
#include <string.h>

int yyerror(const char * ch);
extern FILE *yyin;

int error=0;
int exit_prog=0;

int numFlag=0;
int printFlag=0;
int listFlag=0;

int arr[1000]; 
int idx=0;
int indexArr=0;

typedef struct{
int val;  //key part
char id[30];  // value part
}pair;

pair symbolTable[100];
int indexST=0;

void appendToSymbolTable(int value, char iden[30]){
    pair x;
    x.val = value;
    strcpy(x.id,iden);
    for(int i=0;i<indexST;i++){
        if(strcmp(x.id,symbolTable[i].id)==0){
            symbolTable[i].val = x.val;
            return;
        }
    }
    symbolTable[indexST++] = x;
}

int getValue(char iden[30]){
    for(int i=0;i<indexST;i++){
        if(strcmp(iden,symbolTable[i].id)==0){
            return symbolTable[i].val;
        }
    }
    printf("ERROR: %s is not defined.",iden);
    error=1;
}

void printArr(int arr[],int length){
    int i;
    for(i=0;i<length;i++){
        if(i!= length -1)
            printf("%d,",arr[i]);
        else    
             printf("%d",arr[i]);    

    }

}
int* append(int arr[],int length,int val){
    int i;
    for (i = length-1; i>=0; i--){
        arr[i+1]=arr[i];


    }
    arr[0]=val;
    return arr;

}

int yylex();
%}


%start START
%token OP_PLUS OP_MINUS OP_MULT OP_DIV  OP_OP OP_CP OP_DBLQUOTE OP_COMMA
%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL  KW_APPEND KW_CONCAT KW_SET KW_DEFFUN OP_LIST OP_DBLMULT
%token KW_FOR KW_WHILE KW_DEFVAR KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE KW_LIST
%token COMMENT


%union{
    int value;
    void* values;
    char id[50];
}

%token <value> NUMBER
%token <id>IDENTIFIER
%type <value> INPUT
%type <value> EXPI
%type <value> EXPB
%type <value> EXP
%type <value> BinaryValue
%type <values> EXPLISTI
%type <values> LISTVALUE
%type <values> VALUES


%% 

START: | INPUT;

INPUT: EXPI { 
    if(!error){
        printf("\nSYNTAX OK") ;
        if(printFlag==1){
            if(listFlag==1){ 
                printf ("Result: (" );
                printArr(arr,indexArr);
                printf (")\n");
                indexArr=0;
                listFlag=0;
            }
            else if(numFlag==1){
                printf("\nResult:%d\n",$$);
                numFlag=0;
            }
            else{ 
                printf("Result: Nil\n"); 
                listFlag=0;
                numFlag=0;
            }
            printFlag=0;
        }
    } 
}        

| EXPLISTI {
    if(printFlag==1 &&listFlag==1 &&!error){ 
        printf("\nSYNTAX OK.\n");
        printf ("Result: (" );
        printArr(arr,indexArr);
        printf (")\n");
        indexArr=0;
        printFlag=0;
        listFlag=0;
        idx=0;
    }    
}
| EXPB {
    if (!error)
        printf("\nSYNTAX OK."); 
    if(printFlag==1 &&!error){
        printf("\nResult: ");
        if($$ == 1){
            printf ("true\n");
        }
        else
            printf("false\n");  
        printFlag=0; 
        listFlag=0;
        numFlag=0; 
        idx=0;
    }      
}

LISTVALUE: OP_LIST VALUES OP_CP {  if (idx==0 && !error) { idx=indexArr; }}
| OP_LIST OP_CP {$$=0}|  OP_OP KW_LIST VALUES OP_CP {printFlag=1; listFlag=1; $$=$3;}


VALUES: VALUES NUMBER { arr[indexArr]=$2; indexArr=indexArr+1; }
| NUMBER  { arr[indexArr]=$1 ; indexArr=indexArr+1; }


EXPI:OP_OP OP_PLUS EXPI EXPI OP_CP {$$=$3+$4; printFlag=1; numFlag=1;}

| OP_OP OP_MINUS EXPI EXPI OP_CP {$$=$3-$4; printFlag=1; numFlag=1;}

| OP_OP OP_MULT EXPI EXPI OP_CP {$$=$3*$4; printFlag=1; numFlag=1;}

| OP_OP OP_DIV EXPI EXPI OP_CP {$$=$3/$4; printFlag=1; numFlag=1;}

| OP_OP OP_DBLMULT EXPI EXPI OP_CP {$$=$3*$4*$4;}

|OP_OP IDENTIFIER EXP OP_CP { $$= $3; listFlag=1; printFlag=1;  }

| OP_OP KW_SET IDENTIFIER EXPI OP_CP { $$ = $4; appendToSymbolTable($4,$3); numFlag=1; printFlag=1;}

| OP_OP KW_DEFFUN IDENTIFIER IDENTIFIERLIST EXP OP_CP {$$ = 0;}

| IDENTIFIER {$$ = getValue($1); numFlag=1; printFlag=1;}

| OP_OP KW_LOAD STRING OP_CP {$$=0;}

| OP_OP KW_DISP EXPI OP_CP {$$ = $3; numFlag=1; printFlag=1;}

| NUMBER  {$$=$1;}

| STRING {$$ = 0;}

| OP_OP KW_IF EXPB EXP OP_CP 
{if($3==1)
    $$ = $4;
}

| OP_OP KW_IF EXPB EXP EXP OP_CP 
{
    if($3==1)
        $$ = $4;
    else
        $$ = $5;

}
| OP_OP KW_WHILE EXPB EXP OP_CP {$$ = 0}

| OP_OP KW_FOR OP_OP IDENTIFIER EXPI EXPI OP_CP EXP OP_CP{$$ = 0}

| OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP {  printFlag=1; numFlag=1; $$=$4; }

| OP_OP KW_EXIT OP_CP { exit_prog=1; return 0;}

EXPB: OP_OP KW_AND EXPB EXPB OP_CP  {$$=($3 && $4);  printFlag=1;} 

|OP_OP KW_OR EXPB EXPB OP_CP   {$$=($3 || $4); printFlag=1; } 

| OP_OP KW_NOT EXPB  OP_CP  {$$=!$3;  printFlag=1;} 

| OP_OP KW_LESS EXPI EXPI OP_CP  { $$=($3 < $4); printFlag=1; }

| OP_OP KW_EQUAL EXPB EXPB OP_CP  {$$=($3==$4);  printFlag=1; } 

|OP_OP KW_EQUAL EXPI EXPI OP_CP  {$$=($3==$4);  printFlag=1; } 

| BinaryValue


BinaryValue: KW_TRUE { $$=1;} | KW_FALSE {$$=0; }


IDList: IDENTIFIER | IDENTIFIER IDList
IDENTIFIERLIST : OP_OP IDList OP_CP


EXPLISTI: OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP  {$$=0;}

| OP_OP KW_APPEND EXPI EXPLISTI OP_CP { append(arr,indexArr,$3); indexArr=indexArr+1; }

| LISTVALUE { listFlag=1; printFlag=1;}  


EXP: EXPLISTI {$$ = $1;} | EXPI {$$ = $1;}| EXPB {$$ = $1;}

STRING: OP_DBLQUOTE IDENTIFIER OP_DBLQUOTE

%% 

int load(char* fileName){

}
int main(){ 
    FILE *fp; 
    char input[50];
    char inputFile[50];

    printf ("Write g++ in order to start the interpreter.\n");
    printf ("Write g++ <filename> in order to interprete a file.\n");
    printf("$ "); 

    
    fgets(input,sizeof input,stdin); //whole input
    if (strncmp(input,"g++",3) !=0 ){      //if the first 3 letters are not g++
        printf("Wrong input!");
        
    }      
    else{

        if(strlen(input) == 4){
            printf("Interpreter Mode:\n\n");   
            yyin = stdin; //Input from terminal
            while (exit_prog==0){
                printf(">");
                yyparse(); 
            }
        }
        else{
            printf("File Mode:\n");
            strncpy(inputFile, input+4,strlen(input)-4);

            inputFile[strlen(input)-5] = '\0';
            printf ("$File name: %s\n",inputFile);
            fp = fopen(inputFile,"r"); 
            if(fp==NULL){
                printf("No such file exists.");
                return 0;
            }
            yyin = fp; //input from file
            while (exit_prog==0 || !feof(yyin)){
                yyparse();
            }   
        }
         
    }

    return 0; 
}
int yyerror(const char * ch) {
    printf("\nSYNTAX_ERROR Expression not recognized\n\n");
    exit_prog=1;
    error=1; 
} 