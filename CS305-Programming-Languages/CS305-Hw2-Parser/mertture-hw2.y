%{ 
#include <stdio.h>

int yylex();

void yyerror (const char * s) {
	
	printf("ERROR\n");
}
%}

%token tSTRING
%token tNUM
%token tPRINT
%token tGET
%token tSET
%token tFUNCTION
%token tRETURN
%token tIDENT
%token tEQUALITY
%token tIF
%token tGT
%token tLT
%token tGEQ
%token tLEQ
%token tINC
%token tDEC

%% 

prog:  '['stmtlist']' 
    |  '[' ']';

stmtlist:  stmt stmtlist
	|  stmt;

stmt: SET 
    | IF
    | PRINT
    | INC
    | DEC
    | RETURN
    | expr;

SET: '[' tSET ',' tIDENT ',' expr ']';  

expr: tNUM 
    | tSTRING 
    | FUNCTION
    | GET
    | Condition
    | Operator;

exprlist: expr
	| expr ',' exprlist;


FUNCTION: '[' tFUNCTION ',' '[' paramlist ']' ',' '[' stmtlist ']' ']' 
        | '[' tFUNCTION ',' '[' paramlist ']' ',' '[' ']' ']'
	| '[' tFUNCTION ',' '[' ']' ',' '[' stmtlist ']' ']'
	| '[' tFUNCTION ',' '[' ']' ',' '[' ']' ']';

paramlist: tIDENT
	 | tIDENT ',' paramlist;

IF: '[' tIF ','  Condition  ',' '[' stmtlist ']' '[' stmtlist ']' ']'
  | '[' tIF ','  Condition  ',' '[' stmtlist ']' ']' ;


PRINT: '[' tPRINT ',' '[' expr ']' ']' ;


GET: '[' tGET ',' tIDENT ']'
   | '[' tGET ',' tIDENT ',' '[' exprlist ']' ']'
   | '[' tGET ',' tIDENT ',' '[' ']' ']' ;


INC: '[' tINC ',' tIDENT ']';
DEC: '[' tDEC ',' tIDENT ']';

Condition:'[' tGT  ',' expr ',' expr ']'
	 |'[' tEQUALITY ',' expr ',' expr ']'
	 |'[' tLT ',' expr ',' expr ']'
	 |'[' tLEQ ',' expr ',' expr ']'
	 |'[' tGEQ ',' expr ',' expr ']'  ; 

Operator: '[' '"' '+' '"' ',' expr ',' expr ']'
	| '[' '"' '-' '"' ',' expr ',' expr ']'
	| '[' '"' '*' '"' ',' expr ',' expr ']'
	| '[' '"' '/' '"' ',' expr ',' expr ']';

RETURN: '[' tRETURN ']'
      | '[' tRETURN ',' expr ']';

%%









int main () {
	if (yyparse()) {
		return 1;
	}

	else {

	printf("OK\n");
	return 0;
	}
}

