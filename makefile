LEX= flex
YACC= bison
CC= gcc
CFLAGS= -g

all: lex.yy.c hornres.tab.c hornres.tab.h lex.yy.o hornres.tab.o hornres_code.o HornRes clean

lex.yy.c: hornres.l hornres.tab.h hornres.h
	$(LEX) hornres.l

hornres.tab.c hornres.tab.h: hornres.y hornres.h
	$(YACC) -d hornres.y

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c -o lex.yy.o lex.yy.c

hornres.tab.o: hornres.tab.c
	$(CC) $(CFLAGS) -c -o hornres.tab.o hornres.tab.c

hornres_code.o: hornres_code.c hornres.h
	$(CC) $(CFLAGS) -c -o hornres_code.o hornres_code.c

HornRes: lex.yy.o hornres.tab.o hornres_code.o
	$(CC) $(CFLAGS) hornres_code.o lex.yy.o hornres.tab.o  -o HornRes

clean:
	rm hornres.tab.*
	rm lex.yy.*
	rm *.o
