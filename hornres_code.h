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

atom* unify(atom* body, atom* head);
void* checkAtomsEqual(atom* one, atom* two);

#endif //HORNRES_HORNRES_CODE_H
