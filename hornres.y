%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "hornres.h"

	extern int yyerror(char * err);
	extern int yylex(void);
	extern FILE *yyin;

	formularlist* horn_anchor;
%}

%union {
	char* 		vorf;
	char*		pred;

	term*		term;
	atom*		atom;
	formular*	form;

	termlist*		termlist;
	atomlist*		atomlist;
	formularlist*	formlist;
}


%start list

%left  AND
%left  JUNC
%token COMMA
%token OB
%token CB
%token NEWLINE
%token TRUE
%token FALSE
%token VORF
%token PRED
%token ERROR


%%

list: /* Empty */
	| form NEWLINE list		{printf("new formlist\n");
							 $<formlist>$ = newFormularList( $<form>1 );
							 printf("add formlist element\n");
							 horn_anchor = addFormularListElem( horn_anchor, $<formlist>$ );
							 printf("added formlist\n");
							 printFormList(horn_anchor);
							 printf("\n\n");}
	| NEWLINE list			{}
	| error NEWLINE list	{printf("Formular not regognized!\n");}

form: body JUNC head		{printf("found formular");
							 $<form>$ = newFormular( $<atom>3, $<atomlist>1);
							 printf("\nnew formular!\n\n");}

head: atom					{/*printf("found atom: ", $<pred>1);
							 printAtom($<atom>$);*/}
	| FALSE					{printf("found atom: ");
							 $<atom>$ = newAtom( xstrdup("false"), NULL);
							 printAtom($<atom>$);
						 	 printf("\nnew atom\n\n");}

body: atom					{printf("atomlist: ");
							 $<atomlist>$ = newAtomList( $<atom>1 );
							 printAtomList($<atomlist>$);
						 	 printf("\nnew atomlist\n\n");}
	| TRUE					{printf("atomlist: ");
							 $<atomlist>$ = newAtomList( newAtom( xstrdup("true"), NULL ));
							 printf("\nnew atomlist\n\n");}
	| atom AND body			{printf("atomlist: ");
							 $<atomlist>$ = newAtomList( $<atom>1 );
						 	 $<atomlist>$ = addAtomListElem( $<atomlist>3, $<atomlist>$ );
						 	 printAtomList($<atomlist>$);
						 	 printf("\nadded atomlist\n\n");}

atom:
	PRED					{printf("found atom: %s", $<pred>1);
							 $<atom>$ = newAtom( $<pred>1, NULL);
						 	 printf("\nnew atom\n\n");}
	| PRED OB terms CB		{printf("found atom(termlist): %s", $<pred>1);
							 $<atom>$ = newAtom( $<pred>1, $<termlist>3 );
						 	 printf("\nnew atom\n\n");}

terms: term					{printf("termlist: ");
							 $<termlist>$ = newTermList( $<term>1 );
							 printTermList($<termlist>$);
							 printf("\nnew termlist\n\n");}
	|term COMMA terms		{printf("term, termlist: ");
							 $<termlist>$ = newTermList( $<term>1 );
							 $<termlist>$ = addTermListElem( $<termlist>3, $<termlist>$ );
						 	 printTermList($<termlist>$);
						 	 printf("\nadded termlist\n\n");}

term: VORF					{printf("found term: %s\n", $<vorf>1);
							 $<term>$ = newTerm( $<vorf>1, NULL );
						 	 printf("new term\n\n");}
	| VORF OB terms	CB		{printf("found term(termlist): %s\n", $<vorf>1);
							 $<term>$ = newTerm( $<vorf>1, $<termlist>3 );
						 	 printf("new term\n\n");}


%%

int main (int argc, char* argv[]){
	int satisfied;

	if(argc > 1)
		yyin = fopen(argv[1], "r");
	else
		yyin = stdin;


	do{
		yyparse();
	} while (!feof(yyin));

	if (argc > 1)
		fclose(yyin);

	printf("\nComplete formular list: \n");
	printFormList(horn_anchor);

	satisfied = checkSETsatisfiable(horn_anchor);
	if(satisfied == 0) {
		printf("\n---> Formular unsatisfiable! <---");
		return 0;
	}

	//freeFormList(horn_anchor);

	printf("\n+--> Formular satisfiable! <--+");
	return 0;
}