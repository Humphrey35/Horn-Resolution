CC= gcc
CFLAGS= -g

all: hornres.tab.c hornres.tab.h lex.yy.c lex.yy.o hornres.tab.o HornRes clean

hornres.tab.c hornres.tab.h: hornres.y hornres.h
	bison -d hornres.y

lex.yy.c: hornres.l hornres.tab.h hornres.h
	flex hornres.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

hornres.tab.o: hornres.tab.c
	$(CC) $(CFLAGS) -c hornres.tab.c

HornRes: lex.yy.o hornres.tab.o
	$(CC) $(CFLAGS) lex.yy.o hornres.tab.o -o HornRes

clean:
	rm hornres.tab.*
	rm lex.yy.*
