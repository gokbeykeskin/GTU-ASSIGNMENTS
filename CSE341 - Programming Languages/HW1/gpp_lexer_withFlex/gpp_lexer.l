/*
COMPILATION
$flex gpp_lexer.l
$gcc lex.yy.c -lfl -o gppLexer

EXECUTION
READING FROM FILE: ./gppLexer < input.gpp
INPUT FROM TERMINAL: ./gppLexer

Normally, all the printf statements except 
the error cases should be return statements 
but I made them printf for easier testing on terminal.

*/

%{
#include "definitions.h"
%}

%%

"and"		printf("KW_AND\n");		
"or"		printf("KW_OR\n");		
"not"		printf("KW_NOT\n"); 	
"equal"		printf("KW_EQUAL\n"); 	
"less"		printf("KW_LESS\n"); 	
"nil"		printf("KW_NIL\n"); 	
"list"		printf("KW_LIST\n"); 	
"append"	printf("KW_APPEND\n"); 	
"concat"	printf("KW_CONCAT\n"); 	
"set"		printf("KW_SET\n"); 	
"deffun"	printf("KW_DEFFUN\n"); 	
"for"		printf("KW_FOR\n");		
"if"		printf("KW_IF\n");		
"exit"		printf("KW_EXIT\n");return 0; 	
"load"		printf("KW_LOAD\n"); 	
"disp"		printf("KW_DISP\n"); 	
"true"		printf("KW_TRUE\n"); 	
"false"		printf("KW_FALSE\n");	
"+"			printf("OP_PLUS\n"); 	
"-"			printf("OP_MINUS\n");	
"/"			printf("OP_DIV\n"); 
"*"			printf("OP_MULT\n"); 	
"("			printf("OP_OP\n");		
")"			printf("OP_CP\n");		
"**"		printf("OP_DBLMULT\n"); 
","			printf("OP_COMMA\n"); 	


[\"].*[\"] printf("STRING\n");				//regular expression for "STRING"
[;;].* 					printf("COMMENT\n");				/*regular expression for comment token*/
[0][a-zA-Z][_a-zA-Z0-9]* 			printf("INVALID EXPRESSION(LEADING DIGIT)\n"); 
[0]|[0][.][0-9]*|[1-9][0-9]*[.]?[0-9]*	printf("VALUE\n");			/*regular expression for value (both int and real num) */
[a-zA-Z][_a-zA-Z0-9]* 				printf("IDENTIFIER\n");			/*regular expression for identifier*/
[ \t\n]+				;							/*regular exprssion for ignored characters*/
[0-9][0-9]* 					printf("INVALID EXPRESSION(LEADING 0)\n");			/*regular expression for invalid int */
.					printf("INVALID EXPRESSION\n");          /*EVERYTHING ELSE*/

%%
