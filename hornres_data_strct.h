//
// Created by Hans Potsch on 25.04.17.
//

#ifndef HORNRES_DATA_STRCT_H
#define HORNRES_DATA_STRCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct term {
	char* varorfunc;
	struct termlist* arguments;
} term;

typedef struct termlist {
	term* data;
	struct termlist* next;
} termlist;

typedef struct atom {
	char* predicate;
	struct termlist* arguments;
} atom;

typedef struct atomlist {
	atom* data;
	struct atomlist* next;
} atomlist;

typedef struct formular {
	atom* head;
	struct atomlist* body;
} formular;

typedef struct formularlist {
	formular* data;
	struct formularlist* next;
} formularlist;

typedef struct varList_elem {
	char* var;
	char* new_name;
	int var_count;
	int line;
	struct varList_elem* next;
} varList_elem;

typedef struct varList {
	varList_elem* data;
	struct varList* next;
} varList;

typedef struct uniList_elem {
	char *var;
	term *unif;
} uniList_elem;

typedef struct uniList {
	uniList_elem* data;
	struct unificator* next;
} uniList;


void* xmalloc(size_t size);
char* xstrdup(char* str);

term* newTerm(char* payload, termlist* arguments);
termlist* newTermList(term* data);

atom* newAtom(char* payload, termlist* arguments);
atomlist* newAtomList(atom* data);

formular* newFormular(atom* head, atomlist* body);
formularlist* newFormularList(formular* data);

termlist* addTermListElem(termlist* anchor, termlist* next);
atomlist* addAtomListElem(atomlist* anchor, atomlist* next);
formularlist* addFormularListElem(formularlist* anchor, formularlist* next);

varList_elem* newVar(char* var, varList_elem* anchor);

uniList_elem* newUnification(char* var, term* unif);
uniList addUniElem(uniList* anchor, uniList* next);

#endif //HORNRES_DATA_STRCT_H
