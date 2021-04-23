%{
#include <stdio.h>
#include <string.h>
#include "mertture-hw3.h"
int yylex(void);
void yyerror (const char *s) 
{
	printf("ERROR\n");
}


extern int line;
char ans[10000];
char temp[1000];
%}


%union {

int ivalue;
double value;
char *stvalue;

ExprType exprType;
OprType oprType;
}



%type <exprType> expr
%type <oprType> operation
%type <exprType> returnStmt
%type <exprType> setStmt
%type <exprType> getExpr
%type <exprType> condition
%type <exprType> if

%token <value> tADD
%token <value> tSUB 
%token <value> tMUL  
%token <value> tDIV 
%token tPRINT tGET tSET tFUNCTION tIDENT tEQUALITY tIF tGT tLT tGEQ tLEQ tINC tDEC 
%token tRETURN
%token <ivalue> tNUM
%token <value> tFLOAT
%token <stvalue> tSTRING


%start prog

%%
prog:		'[' stmtlst ']'
;

stmtlst:	stmtlst stmt |
;

stmt:		setStmt {if($1.ifGet == 1) {}
			else if($1.ifStr == 1 && $1.ifNum == 0 && $1.ifFloat == 0) {
			strcat(ans, "Result of expression on ");
			snprintf(temp, sizeof temp, "%i", line);
			strcat(ans, temp);
			strcat(ans, " is (" );
			strcat(ans, $1.st);
			strcat(ans, ")\n" );
			//printf("Result of expression on %d is (%.1f)\n", line, $1.st);}
			}
                        else if ($1.ifNum == 1 && $1.ifStr == 0 && $1.ifFloat == 0) {
				strcat(ans, "Result of expression on ");
                        	snprintf(temp, sizeof ans, "%i", line);
	                      	strcat(ans, temp);
                        	strcat(ans, " is (" );
                       		snprintf(temp, sizeof ans, "%i", $1.inum);
				strcat(ans, temp);
                        	strcat(ans, ")\n" );
				//printf("Result of expression on %d is (%d)\n", line, $1.inum);}
			}
                        else if ($1.ifFloat == 1 && $1.ifNum == 0 && $1.ifStr == 0) {
				
				strcat(ans, "Result of expression on ");
                        	snprintf(temp, sizeof ans, "%i", line);
                        	strcat(ans, temp);
                        	strcat(ans, " is (" );
                        	snprintf(temp, sizeof ans, "%.1f", $1.db);
				strcat(ans, temp);
                        	strcat(ans, ")\n" );
				//printf("Result of expression on %d is (%s)\n", line, $1.db);}} 
				}
}
			| if {if($1.ifGet == 1) {}
                        
                        else if($1.ifStr == 1 && $1.ifNum == 0 && $1.ifFloat == 0) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $1.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $1.st);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 1 && $1.ifFloat == 0) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", line);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $1.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $1.inum);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 0 && $1.ifFloat == 1) {
                                
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", line);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $1.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $1.db);}}
			}
}
		| print 
		| unaryOperation 
		| expr {if($1.ifGet == 1) {}
			
                        else if($1.ifStr == 1 && $1.ifNum == 0 && $1.ifFloat == 0) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $1.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $1.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $1.st);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 1 && $1.ifFloat == 0) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $1.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $1.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $1.inum);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 0 && $1.ifFloat == 1) {
                                
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $1.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $1.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $1.db);}} 
			}
}
		| returnStmt {if($1.ifGet == 1) {}
                        else if($1.ifStr == 1 && $1.ifNum == 0 && $1.ifFloat == 0) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $1.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $1.st);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 1 && $1.ifFloat == 0) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", line);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $1.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $1.inum);}
			}
                        else if ($1.ifStr == 0 && $1.ifNum == 0 && $1.ifFloat == 1) {
                                
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", line);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $1.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $1.db);}}
			}
}
;


getExpr:	'[' tGET ',' tIDENT ',' '[' exprList ']' ']' {$$.ifGet = 1;}
		| '[' tGET ',' tIDENT ',' '[' ']' ']'{$$.ifGet = 1;}
		| '[' tGET ',' tIDENT ']' {$$.ifGet = 1;}
;

setStmt:	'[' tSET ',' tIDENT ',' expr ']' { if ($6.ifStr == 1) {
                                $$.ifStr = 1;
				$$.ifNum = 0;
				$$.ifFloat = 0;
                                $$.st = $6.st;
                            }
                             else if ($6.ifNum == 1) {
                                $$.ifNum = 1;
				$$.ifStr = 0;
				$$.ifFloat = 0;
                                $$.inum = $6.inum;
                            }
                             else if ($6.ifFloat == 1) {
                                $$.ifFloat = 1;
				$$.ifStr = 0;
				$$.ifNum = 0;
                                $$.db = $6.db;
                            }
			}
;

if:		'[' tIF ',' condition ',' '[' stmtlst ']' ']'
		| '[' tIF ',' condition ',' '[' stmtlst ']' '[' stmtlst ']' ']'
;

print:		'[' tPRINT ',' '[' expr ']' ']'
;

operation:	'[' tADD ',' expr ',' expr ']' 
		 { $$.lineNo = $2;
		   if ($4.ifGet == 1 || $6.ifGet == 1) {
                        $$.ifGet = 1;
                   } 
		   else if ($4.ifStr == 1 && $6.ifStr == 1) {
			$$.literal = "str";			
			int size = strlen($4.st) + strlen($6.st);
			$$.strResult = (char*)malloc(size * sizeof(char));
			strcpy($$.strResult, $4.st);
			strcat($$.strResult, $6.st);
			//printf("str + str (%s) (%d)\n", $$.strResult, line);					 
		   }
		   else if ($4.ifStr == 0 && $6.ifStr == 1) {
		   	strcat(ans, "Type mismatch on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, "\n" );
		   }
		   else if ($4.ifStr == 1 && $6.ifStr == 0) {
			strcat(ans, "Type mismatch on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, "\n" );	
		   }
		   else if ($4.ifNum == 1 && $6.ifNum == 1) {
			$$.literal = "int";
			$$.intResult = $4.inum + $6.inum;
			//printf("int + int %d (%d)\n", $$.intResult, line);	
		   }
		   else if ($4.ifFloat == 1 && $6.ifFloat == 1) {
	                $$.literal = "db";        
			$$.dbResult = $4.db + $6.db;
                        //printf("db + db %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifNum == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.inum + $6.db;
                        //printf("int + db %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifNum == 1) {
                        $$.literal = "db";
		        $$.dbResult = $4.db + $6.inum;
                        //printf("db + int %f (%d)\n", $$.dbResult, line);
                   }
		  }
		| '[' tSUB ',' expr ',' expr ']' {$$.lineNo = line; 
		   if ($4.ifNum == 1 && $6.ifNum == 1) {
                        $$.literal = "int";
			$$.intResult = $4.inum - $6.inum;
                        //printf("int - int %d (%d)\n", $$.intResult, line);
                   }
		   else if ($4.ifGet == 1 || $6.ifGet == 1) {
                        $$.ifGet = 1;
                   }
		   else if ($4.ifNum == 1 && $6.ifFloat == 1) {
			$$.literal = "db";
                        $$.dbResult = $4.inum - $6.db;
                        //printf("int - db %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifNum == 1) {
			$$.literal = "db";
                        $$.dbResult = $4.db - $6.inum;
                        //printf("db - int %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.db - $6.db;
                        //printf("db - db %f (%d)\n", $$.dbResult, line);
                   }
                   else {
                        strcat(ans, "Type mismatch on ");
			snprintf(temp, sizeof ans, "%i", line);
			strcat(ans, temp);
			strcat(ans, "\n" );
                   }
                   							
		}
		| '[' tMUL ',' expr ',' expr ']' {$$.lineNo = line; 
		  if ($4.ifNum == 1 && $6.ifNum == 1) {
                        $$.literal = "int";
			$$.intResult = $4.inum * $6.inum;
                        //printf("int * int %d (%d)\n", $$.intResult, line);
                   }
		    else if ($4.ifGet == 1 || $6.ifGet == 1) {
                        $$.ifGet = 1;
                   }
                   else if ($4.ifNum == 1 && $6.ifStr == 1){
			int i;
			$$.literal = "str";
			int x = $4.inum;
			$$.strResult = (char*)malloc(x * sizeof(char));
			strcpy($$.strResult, $6.st);
			for (i = 0; i < (($4.inum)-1); i++) {
				strcat($$.strResult, $6.st);
			}
			//printf("str * int %s (%d)\n", $$.strResult, line);
		   }
	   	   else if ($4.ifStr == 1 && $6.ifNum == 1){
                       	int i;
			$$.literal = "str";
			int x = $6.inum;
			$$.strResult = (char*)malloc(x * sizeof(char));
                        strcpy($$.strResult, $4.st);
                        for (i = 0; i < $6.inum-1; i++) {
                                strcat($$.strResult, $4.st);
                        }
			//printf("str * int %s (%d)\n", $$.strResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifNum == 1) {
	                $$.literal = "db";        
			$$.dbResult = $4.db * $6.inum;
                        //printf("db * int %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.db * $6.db;
                        //printf("db * db %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifNum == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.inum * $6.db;
                        //printf("int * db %f (%d)\n", $$.dbResult, line);
                   }
		   else {
                        strcat(ans, "Type mismatch on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, "\n" );
                   }
		}

		| '[' tDIV ',' expr ',' expr ']' {$$.lineNo = line; 
		   if ($4.ifNum == 1 && $6.ifNum == 1) {
                        $$.literal = "int";
			$$.intResult = $4.inum / $6.inum;
                        //printf("int / int  %d (%d)\n", $$.intResult, line);
                   }
		   else if ($4.ifGet == 1 || $6.ifGet == 1) {
                        $$.ifGet = 1;
                   }
		   else if ($4.ifNum == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.inum / $6.db;
                        //printf("int / db  %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifNum == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.db / $6.inum;
                        //printf("db / int  %f (%d)\n", $$.dbResult, line);
                   }
		   else if ($4.ifFloat == 1 && $6.ifFloat == 1) {
                        $$.literal = "db";
			$$.dbResult = $4.db / $6.db;
                        //printf("db / int  %f (%d)\n", $$.dbResult, line);
                   }
                   else {
                        strcat(ans, "Type mismatch on ");
                        snprintf(temp, sizeof ans, "%i", line);
                        strcat(ans, temp);
                        strcat(ans, "\n" );
                   }
		}
;	

unaryOperation: '[' tINC ',' tIDENT ']'
		| '[' tDEC ',' tIDENT ']'
;

expr:		tNUM	   { $$.ifNum = 1; $$.ifStr = 0; $$.ifFloat = 0; $$.inum = $1;} 
		| tFLOAT   { $$.ifFloat = 1; $$.ifStr = 0; $$.ifNum = 0; $$.db = $1;}
		| tSTRING  { $$.ifNum = 0; $$.ifStr = 1; $$.ifFloat = 0; $$.st = $1;}
	        | getExpr  { $$.ifNum = 0; $$.ifStr = 0; $$.ifFloat = 0; $$.ifGet = $1.ifGet; }
	        | function { $$.ifNum = 0; $$.ifStr = 0; $$.ifFloat = 0;}
		| operation {if ($1.ifGet == 1) {
                                $$.ifGet = 1;
				$$.LineNo = $1.lineNo;
                            }
			    else if ($1.literal == "str") {
				$$.ifStr = 1;
				$$.ifFloat = 0;
				$$.ifNum = 0;
				$$.st = $1.strResult;
				$$.LineNo = $1.lineNo;
			    }
			     else if ($1.literal == "int") {
			        $$.ifNum = 1;
				$$.ifFloat = 0;
				$$.ifStr = 0;
				$$.inum = $1.intResult;
				$$.LineNo = $1.lineNo;
			    }
			     else if ($1.literal == "db") {
				$$.ifFloat = 1;
				$$.ifStr = 0;
				$$.ifNum = 0;
				$$.db = $1.dbResult;
				$$.LineNo = $1.lineNo;
			    }
			    
}
		| condition { $$.ifNum = 0; $$.ifStr = 0; $$.ifFloat = 0;}
;

function:	 '[' tFUNCTION ',' '[' parametersList ']' ',' '[' stmtlst ']' ']'
		| '[' tFUNCTION ',' '[' ']' ',' '[' stmtlst ']' ']'
;

condition:	'[' tEQUALITY ',' expr ',' expr ']' {if($4.ifGet == 1) {}
                        
                        else if($4.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $4.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $4.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $4.st);}
			}
                        else if ($4.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $4.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $4.inum);}
			}
                        else if ($4.ifFloat == 1) {
                                
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $4.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $4.db);}}
			}
			if($6.ifGet == 1) {}

                        else if($6.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $6.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $6.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $6.st);}
			}
                        else if ($6.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $6.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $6.inum);}
			}
                        else if ($6.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $6.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $6.db);}}
                        }}

     		
		| '[' tGT ',' expr ',' expr ']' 
                        {if($4.ifGet == 1) {}

                        else if($4.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $4.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $4.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $4.st);}
			}
                        else if ($4.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $4.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $4.inum);}
			}
                        else if ($4.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $4.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $4.db);}}
                        }
if($6.ifGet == 1) {}

                        else if($6.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $6.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $6.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $6.st);}
}
                        else if ($6.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $6.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $6.inum);}
}
                        else if ($6.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $6.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $6.db);}}
                        }}
		| '[' tLT ',' expr ',' expr ']' {if($4.ifGet == 1) {}

                        else if($4.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $4.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $4.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $4.st);}
}
                        else if ($4.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $4.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $4.inum);}
}
                        else if ($4.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $4.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $4.db);}}
                        }
if($6.ifGet == 1) {}

                        else if($6.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $6.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $6.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $6.st);}
			}
                        else if ($6.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $6.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $6.inum);}
			}
                        else if ($6.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $6.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $6.db);}}
                        }}
		| '[' tGEQ ',' expr ',' expr ']' {if($4.ifGet == 1) {}

                        else if($4.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $4.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $4.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $4.st);}
			}
                        else if ($4.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $4.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $4.inum);}
			}
                        else if ($4.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $4.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $4.db);}}
                        }
if($6.ifGet == 1) {}

                        else if($6.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $6.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $6.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $6.st);}
			}
                        else if ($6.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $6.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $6.inum);}
			}
                        else if ($6.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $6.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $6.db);}}
                        }}
		| '[' tLEQ ',' expr ',' expr ']' {if($4.ifGet == 1) {}

                        else if($4.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $4.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $4.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $4.st);}
			}
                        else if ($4.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $4.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $4.inum);}
			}
                        else if ($4.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $4.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $4.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $4.db);}}
                        }
if($6.ifGet == 1) {}

                        else if($6.ifStr == 1) {
                        strcat(ans, "Result of expression on ");
                        snprintf(temp, sizeof ans, "%i", $6.LineNo);
                        strcat(ans, temp);
                        strcat(ans, " is (" );
                        strcat(ans, $6.st);
                        strcat(ans, ")\n" );
                        //printf("Result of expression on %d is (%.1f)\n", line, $6.st);}
			}
                        else if ($6.ifNum == 1) {
                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%i", $6.inum);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%d)\n", line, $6.inum);}
			}
                        else if ($6.ifFloat == 1) {

                                strcat(ans, "Result of expression on ");
                                snprintf(temp, sizeof ans, "%i", $6.LineNo);
                                strcat(ans, temp);
                                strcat(ans, " is (" );
                                snprintf(temp, sizeof ans, "%.1f", $6.db);
                                strcat(ans, temp);
                                strcat(ans, ")\n" );
                                //printf("Result of expression on %d is (%s)\n", line, $6.db);}}
                        }}
;

returnStmt:	'[' tRETURN ',' expr ']' { 
			     if ($4.ifStr == 1) {
                                $$.ifStr = 1;
				$$.ifNum = 0;
				$$.ifFloat = 0;
                                $$.st = $4.st;
                            }
                             else if ($4.ifNum == 1) {
                                $$.ifNum = 1;
				$$.ifFloat = 0;
				$$.ifStr = 0;
                                $$.inum = $4.inum;
                            }
                             else if ($4.ifFloat == 1) {
                                $$.ifFloat = 1;
				$$.ifStr = 0;
				$$.ifNum = 0;
                                $$.db = $4.db;
                            }
			}			
		| '[' tRETURN ']'
;

parametersList: parametersList ',' tIDENT | tIDENT
;

exprList:	exprList ',' expr | expr
;

%%
int main () {
	if (yyparse()) {
		// parse error
		return 1;
	}

	else {
		// successful parsing
		printf(ans);
		return 0;
	}
}

