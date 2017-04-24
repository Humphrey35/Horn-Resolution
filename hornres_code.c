//
// Created by Hans Potsch on 18.04.17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hornres.h"

extern int var_count;
extern int line;

int yyerror(char* err){
    printf("Error: %s\n", err);
    exit(-1);
}

void* xmalloc(size_t size){
    void *mem = malloc(size);
    if(!mem)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}
char* xstrdup(char* str){
    void* newstr = strdup(str);
    if(!newstr)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return newstr;
}

term* newTerm(char* payload, termlist* arguments){
    term* new_term = (term *)xmalloc(sizeof(term));

    new_term->varorfunc = xstrdup(payload);
    new_term->arguments = arguments;

    return new_term;
}
termlist* newTermList(term* data){
    termlist* new_termlist = xmalloc(sizeof(termlist));

    new_termlist->data = data;
    new_termlist->next = NULL;

    return new_termlist;
}

atom* newAtom(char* payload, termlist* arguments){
    atom* new_atom = xmalloc(sizeof(atom));

    new_atom->predicate = strdup(payload);
    new_atom->arguments = arguments;

    return new_atom;
}
atomlist* newAtomList(atom* data){
    atomlist* new_atomlist = xmalloc(sizeof(atomlist));

    new_atomlist->data = data;
    new_atomlist->next = NULL;

    return new_atomlist;
}

formular* newFormular(atom* head, atomlist* body){
    formular* new_formular = xmalloc(sizeof(formular));

    new_formular->head = head;
    new_formular->body = body;

    return new_formular;
}
formularlist* newFormularList(formular* data){
    formularlist* new_formularlist = xmalloc(sizeof(formularlist));

    new_formularlist->data = data;
    new_formularlist->next = NULL;

    return new_formularlist;
}

termlist* addTermListElem(termlist* anchor, termlist* new_termlist){
    new_termlist->next = anchor;
    anchor = new_termlist;

    return new_termlist;
}
atomlist* addAtomListElem(atomlist* anchor, atomlist* new_atomlist){
    new_atomlist->next = anchor;
    anchor = new_atomlist;

    return new_atomlist;
}
formularlist* addFormularListElem(formularlist* anchor, formularlist* new_formularlist){
    new_formularlist->next = anchor;
    anchor = new_formularlist;

    return new_formularlist;
}

void printFormList(formularlist* formlist){
    if(formlist->data->body != NULL && formlist->data->head != NULL) {
        printAtomList(formlist->data->body);
        printf(" -> ");
        printAtom(formlist->data->head);
        printf("\n");
    }
    else
    {
        printf("No valid Formular or Pointer went wrong!\n");
    }

    if(formlist->next != NULL) {
        printFormList(formlist->next);
    }

}
void printAtomList(atomlist* atomlist){
    printf("%s", atomlist->data->predicate);
    if(atomlist->data->arguments != NULL) {
        printf("(");
        printTermList(atomlist->data->arguments);
        printf(")");

    }

    if (atomlist->next != NULL) {
        printf(" & ");
        printAtomList(atomlist->next);
    }
}
void printAtom(atom* atom){
    printf("%s", atom->predicate );
    if(atom->arguments != NULL)
    {
        printf("(");
        printTermList(atom->arguments);
        printf(")");
    }
}
void printTermList(termlist* termlist){
    printf("%s", termlist->data->varorfunc);
    if(termlist->data->arguments != NULL) {
        printf("(");
        printTermList(termlist->data->arguments);
        printf(")");
    }

    if(termlist->next != NULL) {
        printf(",");
        printTermList(termlist->next);
    }
}

void freeFormList(formularlist* formjunk){

    free(formjunk->data->head->predicate);
    freeTermList(formjunk->data->head->arguments);
    free(formjunk->data->head);
    freeAtomList(formjunk->data->body);
    if (formjunk->next != NULL) {
        freeFormList(formjunk->next);
    }
    free(formjunk);
};
void freeAtomList(atomlist* atomjunk) {
    atomlist* next = NULL;

    while (atomjunk != NULL) {
        next = atomjunk->next;
        free(atomjunk->data->predicate);
        if (atomjunk->data->arguments != NULL) {
            freeTermList(atomjunk->data->arguments);
        }
        free(atomjunk->data);
        free(atomjunk);
        atomjunk = next;
    }
}
void freeTermList(termlist* termjunk) {
    termlist* next = NULL;

    while (termjunk != NULL) {
        next = termjunk->next;
        free(termjunk->data->varorfunc);
        if (termjunk->data->arguments != NULL) {
            freeTermList(termjunk->data->arguments);
        }
        free(termjunk->data);
        free(termjunk);
        termjunk = next;
    }
}

formularlist* getQueryFormulars(formularlist* list){
//    printf("0");
    formularlist* query_list 	= NULL;
	formularlist* tmp 			= NULL;

    while (list != NULL){
//        printf("1");
        if(strcmp(list->data->head->predicate, "false") == 0){
//			printf("2");
			tmp = newFormularList(list->data);
			query_list = addFormularListElem(query_list, tmp);
        }
//        printf("3");
        list = list->next;
    }
//	printf("5");
    return query_list;
}

formularlist* getDefiniteFormulars(formularlist* list){
	formularlist* definite_list = NULL;
	formularlist* tmp = NULL;

	while (list != NULL){
		if (strcmp(list->data->head->predicate, "false") != 0){
			tmp = newFormularList(list->data);
			definite_list = addFormularListElem(definite_list, tmp);
		}
		list = list->next;
	}
	return definite_list;
}

int checkSLDsatisfiable(atomlist* query_body, formularlist* definite_list){

	if(query_body == NULL || strcmp(query_body->data->predicate, "true") == 0)
		return 0;
	else {
		atomlist* query_anchor = query_body;
		formularlist* definite_anchor = definite_list;

		atomlist* new_query_list = NULL;
		formularlist* unifiable = NULL;

		atomlist* tmp_atmlst = NULL;
		formularlist* tmp_frmlst = NULL;


		while (definite_list != NULL){
			printf("check: %s | %s", query_body->data->predicate, definite_list->data->head->predicate);
			if (strcmp(query_body->data->predicate, definite_list->data->head->predicate) == 0) {
				printf(" <- true\n");
				tmp_frmlst = newFormularList(definite_list->data);
				unifiable = addFormularListElem(unifiable, tmp_frmlst);
			} else {
				printf("\n");
			}
			definite_list = definite_list->next;
		}

		printf("\nUnifiable:\n");
		if (unifiable != NULL)
			printFormList(unifiable);
		else
			printf("{}\n");

		atomlist* body_anchor = NULL;

		while (unifiable != NULL){
			if (unifiable != NULL) {
				body_anchor = unifiable->data->body;
				new_query_list = query_anchor->next;
				while (body_anchor != NULL) {
					if (strcmp(body_anchor->data->predicate, "true") != 0) {
						tmp_atmlst = newAtomList(body_anchor->data);
						new_query_list = addAtomListElem(new_query_list, tmp_atmlst);
					}
					body_anchor = body_anchor->next;
				}
			}

			printf("\nNew Query:\n");
			if (new_query_list != NULL)
				printAtomList(new_query_list);
			else
				printf("{}\n");

			printf(" --> checkSLDsatisfiable()""\n+--------------------------------------------------------------------+\n");
			getchar();
			if (checkSLDsatisfiable(new_query_list, definite_anchor) != 1)
				return 0;

			unifiable = unifiable->next;
		}
		return 1;
	}

};

int checkSETsatisfiable(formularlist* horn_anchor){

	formularlist* query_list = NULL;
	formularlist* definite_list = NULL;

	printf("\n\tGetting query formulars\n\n");
	query_list = getQueryFormulars(horn_anchor);
	printf("Query list: \n");
	if (query_list != NULL)
		printFormList(query_list);
	else
		printf("{}");
	printf("\n");

	printf("\n\tGetting definite formulars\n\n");
	definite_list = getDefiniteFormulars(horn_anchor);
	printf("Definite list:\n");
	if(definite_list != NULL)
		printFormList(definite_list);
	else
		printf("{}");
	printf("\n");

	while (query_list != NULL){
		printf("\nChecking query: \n");
		printAtomList(query_list->data->body);
		printf(" -> false\n\n");
		printf("+--------------------------------------------------------------------+\n");
		getchar();
		if (checkSLDsatisfiable(query_list->data->body, definite_list) != 1)
			return 0;
		query_list = query_list->next;
	}

	return 1;
}

varlist* newVar(char* var, varlist* anchor){
	char* buffer;
	varlist* tmp = anchor;

//	printf("Compare Vars:\n");
	while(tmp != NULL ) {
//		printf("%s | %s", tmp->var, var);
		if (strcmp(tmp->var, var) == 0 && tmp->line == line) {
//			printf(" <- true\n");
			return anchor;
		}
//		printf("\n");
		tmp = tmp->next;
	}

	var_count++;
	varlist* new_varlist = xmalloc(sizeof(varlist));
//	printf("\tnewVar %s %p --> %p\n", var, anchor, new_varlist);
	new_varlist->var = xstrdup(var);
	new_varlist->var_count = var_count;
	new_varlist->line = line;

	buffer = xmalloc(sizeof(char)+ sizeof(int));
	sprintf(buffer, "v%d", new_varlist->var_count);

	new_varlist->new_name = xstrdup(buffer);
	free(buffer);

	new_varlist->next = anchor;
	anchor = new_varlist;

	return new_varlist;
}

char* varRename(char* var, varlist* anchor){

	while (anchor != NULL){
		if(strcmp(anchor->var, var) == 0){
			break;
		}
		anchor = anchor->next;
	}

	return anchor->new_name;
}

void printVarList(varlist* anchor){
	char* buffer = xmalloc(sizeof(char)+ sizeof(int));

	printf("\nVar-List:\n");

	while(anchor != NULL){
		printf("%s ---> %s\n", anchor->var, anchor->new_name);
		anchor = anchor->next;
	}
	printf("\n");

	free(buffer);
}