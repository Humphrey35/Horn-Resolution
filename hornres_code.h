//
// Created by Hans Potsch on 25.04.17.
//

#ifndef HORNRES_HORNRES_CODE_H
#define HORNRES_HORNRES_CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hornres_data_strct.h"

void printVarList(varList_elem* anchor);
char* varRename(char* var, varList_elem* anchor);

void printFormList(formularlist* formlist);
void printAtomList(atomlist* atomlist);
void printAtom(atom* atom);
void printTermList(termlist* termlist);

void freeFormList(formularlist* formlist);
void freeAtomList(atomlist* junk);
void freeTermList(termlist* junk);

formularlist* getQueryFormulars(formularlist* list);
formularlist* getDefiniteFormulars(formularlist *list);

int checkSLDsatisfiable(atomlist* query_list, formularlist* definite_list);
int checkSETsatisfiable(formularlist* horn_anchor);

int checkAtomsEqual(atom *atom1, atom *atom2);
int checkTermListsEqual(termlist* termlist1, termlist* termlist2);
int checkTermsEqual(term* term1, term* term2);

atom* unify(atom *query_body, atom* definite_head);
termlist* unifyTermLists(termlist* termlist1, termlist* termlist2);
int checkSubterm(char *var, termlist* subterm);
int checkVar(char* var);


#endif //HORNRES_HORNRES_CODE_H
