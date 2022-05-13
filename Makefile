all: ld.l temp
	lex -o ld.yy.c ld.l
	gcc ld.yy.c -o ld
	./ld
	rm temp*

temp: $(input) hcc
	./hcc $(input)

hcc: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o hcc

y.tab.c y.tab.h: hcc.y
	yacc -d hcc.y

lex.yy.c: hcc.l
	lex hcc.l

clean:
	rm -f *.yy.c *.xsm hcc ld *.tab.* temp* *.o