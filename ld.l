%{
	#include<stdio.h>

	int inst=-4, pass=0, addr[1024];
	FILE *fp, *yyin;
%}

%%

L[0-9]+\n {
	   	if (!pass)
	   	{
			fprintf(fp,"%s",yytext);
			++inst;
        }
		else
		{
			int label = atoi(yytext+1);
			fprintf(fp, "%d\n", addr[label]);
		}
	  }
L[0-9]+:\n {
        int label = atoi(yytext+1);
        addr[label] = 2048 + 2*inst;
	   }


[\n]+ {
        fprintf(fp,"%s",yytext);
        ++inst;
	 }

. { fputc(*yytext,fp); }

%%


int yywrap(void)
{
	if (!pass)
	{
        fflush(fp);
        fclose(fp);
        yyin = fopen("temp","r");
        fp = fopen("B190513CS.xsm","w");
        pass = 1;
        return 0;
    }
	fflush(fp);
	fclose(fp);
	return 1;
}


int main()
{
	yyin = fopen("B190513CS.o","r");
	fp = fopen("temp","w");
	yylex();
	return 0;
}