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
    #include<stdio.h> 
    #include "gpp_parser.tab.h"
    #include <string.h>
%}

%%

"and"		return KW_AND;		
"or"		return KW_OR;		
"not"		return KW_NOT; 	
"equal"		return KW_EQUAL; 	
"less"		return KW_LESS; 	
"nil"		return KW_NIL; 	
"list"		return KW_LIST; 	
"append"	return KW_APPEND; 	
"concat"	return KW_CONCAT; 	
"set"		return KW_SET; 	
"deffun"	return KW_DEFFUN; 	
"defvar"    return KW_DEFVAR; //lispe ekle
"for"		return KW_FOR;
"while"		return KW_WHILE; //lispe ekle	
"if"		return KW_IF;		
"exit"		return KW_EXIT;return 0; 	
"load"		return KW_LOAD; 	
"disp"		return KW_DISP; 	
"true"		return KW_TRUE; 	
"false"		return KW_FALSE;	
"+"			return OP_PLUS; 	
"-"			return OP_MINUS;	
"/"			return OP_DIV; 
"*"			return OP_MULT;
"**"		return OP_DBLMULT;  	
"("			return OP_OP;		
")" 		return OP_CP;		
","			return OP_COMMA;
"\""        return OP_DBLQUOTE;
"'("        return OP_LIST;


[;;].* 					return 0;				/*regular expression for comment token*/
[0][a-zA-Z][_a-zA-Z0-9]* 			printf("INVALID EXPRESSION(LEADING DIGIT)"); 
[0]|[0][.][0-9]*|[1-9][0-9]*[.]?[0-9]*	{yylval.value=atoi(yytext); return NUMBER;}			/*regular expression for NUMBER*/
[a-zA-Z][_a-zA-Z0-9]* 				{strcpy(yylval.id,yytext); return IDENTIFIER;}		/*regular expression for IDENTIFIER*/
" "|"\t" { }
"\n"|"\r"|"\r\n" return 0;
[0-9][0-9]* 					printf("INVALID EXPRESSION(LEADING 0)");			/*regular expression for invalid int */
.        { printf("Error");}
%%


int yywrap() 
{ 
return 1; 
} 
