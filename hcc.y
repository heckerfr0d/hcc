%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "hcc.c"
	#include "y.tab.h"

    int yylex();
    void yyerror(char const*);
	extern FILE *yyin;
%}

%union {
	struct  node *node_ptr;
}

%token FUN IF ELSE DO WHILE READ WRITE PLUS MINUS MUL ASGN LE GE LT GT EQ NE VAR NUM ENDOFFILE
%type <node_ptr> Program Stmts Stmt E VAR NUM
%left PLUS MINUS MUL
%nonassoc LE GE LT GT EQ NE

%%

Program: FUN'('')' '{' Stmts '}' ENDOFFILE	{
        fp = fopen("B190513CS.o", "w");
		fprintf(fp, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP,4095\nMOV BP,4096\nPUSH R0\nCALL 2066\nINT 10\n");
		code_gen($5);
		fprintf(fp, "RET\n");
        fflush(fp);
		fclose(fp);

		return 0;
	}
	;

Stmts: Stmts Stmt	{ $$ = createNode(STMTS_NODE, -1, NULL, $1, NULL, $2); }
	| Stmt	        { $$ = $1; }
	;


Stmt: VAR ASGN E ';'	{
            if (!lookup($1->NAME))
                append($1->NAME);
			$$ = createNode(ASGN, -1, NULL, $1, NULL, $3);
		}

		| IF '(' E ')' '{' Stmts '}' {
			$$ = createNode(IF, -1, NULL, $3, $6, NULL);
		}

		| IF '(' E ')' '{' Stmts '}' ELSE '{' Stmts '}' {
			$$ = createNode(IF, -1, NULL, $3, $6, $10);
		}

		| DO '{' Stmts '}' WHILE '(' E ')' ';' {
			$$ = createNode(WHILE, -1, NULL, $3, $7, NULL);
		}

		| READ '(' VAR ')' ';'	{
            if (!lookup($3->NAME))
                append($3->NAME);
			$$ = createNode(READ, -1, NULL, $3, NULL, NULL);
		}

		| WRITE '(' E ')' ';' {
			$$ = createNode(WRITE, -1, NULL, $3, NULL, NULL);
		}
		;

E:  E PLUS E	{ $$ = createNode(PLUS, -1, NULL, $1, $3, NULL); }

    | E MINUS E	{ $$ = createNode(MINUS, -1, NULL, $1, $3, NULL); }

    | E MUL E	{ $$ = createNode(MUL, -1, NULL, $1, $3, NULL); }

    | '(' E ')'	{ $$ = $2; }

    | E LE E    { $$ = createNode(LE, -1, NULL, $1, $3, NULL); }

    | E GE E    { $$ = createNode(GE, -1, NULL, $1, $3, NULL); }

    | E LT E    { $$ = createNode(LT, -1, NULL, $1, $3, NULL); }

    | E GT E    { $$ = createNode(GT, -1, NULL, $1, $3, NULL); }

    | E EQ E    { $$ = createNode(EQ, -1, NULL, $1, $3, NULL); }

    | E NE E    { $$ = createNode(NE, -1, NULL, $1, $3, NULL); }

    | NUM	    { $$ = $1; }

    | VAR       { $$ = $1; }
    ;

%%

void yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(int argc, char *argv[]) {
	if(argc > 1)
    	yyin = fopen(argv[1], "r");
	yyparse();

	return 0;
}