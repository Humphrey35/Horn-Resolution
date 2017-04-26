%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "hornres_data_strct.h"
	#include "hornres_code.h"

	extern int yyerror(char * err);
	extern int yylex(void);
	extern FILE *yyin;

	formularlist* horn_anchor;
	varList_elem* global_vars;
	int var_count = 0;
	int line = 0;
%}

%union {
	char* 		var;
	char*		func;
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
%token VAR
%token FUNC
%token PRED
%token ERROR


%%

list: /* Empty */
	| form NEWLINE list		{//printf("new formlist\n");
							 $<formlist>$ = newFormularList( $<form>1 );
							 //printf("add formlist element\n");
							 horn_anchor = addFormularListElem( horn_anchor, $<formlist>$ );
							 //printf("added formlist\n");
							 //printFormList(horn_anchor);
							 //printf("\n\n");
							}
	| form					{//printf("new formlist\n");
							 $<formlist>$ = newFormularList( $<form>1 );
							 //printf("add formlist element\n");
							 horn_anchor = addFormularListElem( horn_anchor, $<formlist>$ );
							 //printf("added formlist\n");
							 //printFormList(horn_anchor);
							 //printf("\n\n");
							}
	| NEWLINE list			{}
	| error NEWLINE list	{printf("Formular not recognized!\n");}

form: body JUNC head		{//printf("found formular");
							 $<form>$ = newFormular( $<atom>3, $<atomlist>1);
							 //printf("\nnew formular!\n\n");
							 line++;}

head: atom					{/*printf("found atom: ", $<pred>1);
							 printAtom($<atom>$);*/}
	| FALSE					{//printf("found atom: ");
							 $<atom>$ = newAtom( xstrdup("false"), NULL);
							 //printAtom($<atom>$);
						 	 //printf("\nnew atom\n\n");
							}

body: atom					{//printf("atomlist: ");
							 $<atomlist>$ = newAtomList( $<atom>1 );
							 //printAtomList($<atomlist>$);
						 	 //printf("\nnew atomlist\n\n");
							}
	| TRUE					{//printf("atomlist: ");
							 $<atomlist>$ = newAtomList( newAtom( xstrdup("true"), NULL ));
							 //printf("\nnew atomlist\n\n");
							}
	| atom AND body			{//printf("atomlist: ");
							 $<atomlist>$ = newAtomList( $<atom>1 );
						 	 $<atomlist>$ = addAtomListElem( $<atomlist>3, $<atomlist>$ );
						 	 //printAtomList($<atomlist>$);
						 	 //printf("\nadded atomlist\n\n");
							}

atom:
	PRED					{//printf("found atom: %s", $<pred>1);
							 $<atom>$ = newAtom( $<pred>1, NULL);
						 	 //printf("\nnew atom\n\n");
							}
	| PRED OB terms CB		{//printf("found atom(termlist): %s", $<pred>1);
							 $<atom>$ = newAtom( $<pred>1, $<termlist>3 );
						 	 //printf("\nnew atom\n\n");
							}

terms: term					{//printf("termlist: ");
							 $<termlist>$ = newTermList( $<term>1 );
							 //printTermList($<termlist>$);
							 //printf("\nnew termlist\n\n");
							}
	|term COMMA terms		{//printf("term, termlist: ");
							 $<termlist>$ = newTermList( $<term>1 );
							 $<termlist>$ = addTermListElem( $<termlist>3, $<termlist>$ );
						 	 //printTermList($<termlist>$);
						 	 //printf("\nadded termlist\n\n");
							}

term: VAR					{//printf("found var: %s\n", $<var>1);
							 global_vars = newVar($<var>1, global_vars);
							 $<term>$ = newTerm( varRename($<var>1, global_vars), NULL );
						 	 //printf("new term\n\n");
							}
	| FUNC					{//printf("found func: %s\n", $<func>1);
							 $<term>$ = newTerm( $<func>1, NULL );
							 //printf("new term\n\n");
							}
	| FUNC OB terms	CB		{//printf("found func(termlist): %s\n", $<func>1);
							 $<term>$ = newTerm( $<func>1, $<termlist>3 );
						 	 //printf("new term\n\n");
							}


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

	printf("\n\tFinished parsing!\n");
	printVarList(global_vars);

	if (argc > 1)
		fclose(yyin);

	printf("\nComplete formular list: \n");
	printFormList(horn_anchor);

	satisfied = checkSETsatisfiable(horn_anchor);
	if(satisfied == 0) {
		printFormList(horn_anchor);
		printf("\n---> Formular unsatisfiable! <---");
		return 0;
	}

	//freeFormList(horn_anchor);
	printf("\nFinal formular list: \n");
	printFormList(horn_anchor);
	printf("\n+--> Formular satisfiable! <--+");
	return 0;
}

int yyerror(char* err){
	printf("Error: %s\n", err);
	exit(-1);
}
