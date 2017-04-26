LEX= flex
YACC= bison
CC= gcc
CFLAGS= -g

all: hornres.tab.c hornres.tab.h lex.yy.c lex.yy.o hornres.tab.o hornres_data_strct.c hornres_code.o HornRes

hornres.tab.c hornres.tab.h: hornres.y
	$(YACC) -d hornres.y

lex.yy.c: hornres.l hornres.tab.h
	$(LEX) hornres.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c -o lex.yy.o lex.yy.c

hornres.tab.o: hornres.tab.c
	$(CC) $(CFLAGS) -c -o hornres.tab.o hornres.tab.c

hornres_data_strct.o: hornres_data_strct.c hornres_data_strct.h
	$(CC) $(CFLAGS) -c -o hornres_data_strct.o hornres_data_strct.c

hornres_code.o: hornres_code.c hornres_code.h
	$(CC) $(CFLAGS) -c -o hornres_code.o hornres_code.c

HornRes: lex.yy.o hornres.tab.o hornres_data_strct.o hornres_code.o
	$(CC) $(CFLAGS) hornres_code.o hornres_data_strct.o lex.yy.o hornres.tab.o -o HornRes

clean:
	rm hornres.tab.*
	rm lex.yy.*
	rm *.o
