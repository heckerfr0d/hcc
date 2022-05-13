%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

    #include "hcc.h"
	#include "y.tab.h"
%}


%%

<<EOF>>    	{ return ENDOFFILE; }
[ \t\n]    	{;}
"//".*?\n	{;}
"/*".*?"*/"	{;}

[0-9]+  {
	yylval.node_ptr = createNode(NUM, atoi(yytext), NULL, NULL, NULL, NULL);
	return NUM;
}

[fF][uU][nN]   { return FUN; }
[rR][eE][aA][dD]   { return READ;}
[wW][rR][iI][tT][eE] { return WRITE; }
[iI][fF] { return IF; }
[eE][lL][sS][eE] { return ELSE; }
[wW][hH][iI][lL][eE] { return WHILE; }
[dD][oO] { return DO; }

[a-z][a-zA-Z0-9]* {

	char *variable_name = (char *)malloc(strlen(yytext));
	strcpy(variable_name, yytext);

	yylval.node_ptr = createNode(VAR, -1, variable_name, NULL, NULL, NULL);

	return VAR;
}

"+"	{ return PLUS; }
"-"	{ return MINUS; }
"*"	{ return MUL; }
"<=" { return LE; }
">=" { return GE; }
"<" { return LT; }
">" { return GT; }
"==" { return EQ; }
"!=" { return NE; }
[=]  { return ASGN; }
[{}();]	{ return *yytext; }

.	{ printf("Unknown character: %s\n", yytext); exit(1); }

%%

int yywrap(void)
{
	return 1;
}