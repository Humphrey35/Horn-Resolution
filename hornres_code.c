//
// Created by Hans Potsch on 18.04.17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hornres.h"

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

atomlist* getQueryAtoms(formularlist* list){
//    printf("0");
    atomlist* query_list = NULL;
	atomlist* tmp = NULL;

    while (list != NULL){
//        printf("1");
        if(strcmp(list->data->head->predicate, "false") == 0){
//			printf("2");
			tmp = list->data->body;
			query_list = addAtomListElem(query_list, tmp);
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

int checkSLDsatisfiable(atomlist* query_list, formularlist* definite_list){

	if(query_list == NULL)
		return 0;

	atomlist* query_anchor = query_list;
	formularlist* definite_anchor = definite_list;

	atomlist* new_query_list = NULL;
	formularlist* unifiable = NULL;

	atomlist* tmp_atmlst = NULL;
	formularlist* tmp_frmlst = NULL;


	while (definite_list != NULL){
		printf("check: %s | %s", query_list->data->predicate, definite_list->data->head->predicate);
		if (strcmp(query_list->data->predicate, definite_list->data->head->predicate) == 0) {
			printf(" <- true\n");
			tmp_frmlst = newFormularList(definite_list->data);
			unifiable = addFormularListElem(unifiable, tmp_frmlst);
		} else {
			printf("\n");
		}
		definite_list = definite_list->next;
	}

	if (unifiable != NULL) {
		printf("\nUnifiable:\n");
		printFormList(unifiable);

		atomlist* body_anchor = NULL;

		if(query_anchor->next != NULL)
			printf("\nquery_anchor->next->data->predicate: %s", query_anchor->next->data->predicate);

		new_query_list = query_anchor->next;

		while (unifiable != NULL) {
			body_anchor = unifiable->data->body;
			while (body_anchor != NULL) {
				if (strcmp(body_anchor->data->predicate, "true") != 0) {
					tmp_atmlst = newAtomList(body_anchor->data);
					new_query_list = addAtomListElem(new_query_list, tmp_atmlst);
				}
				body_anchor = body_anchor->next;
			}
			unifiable = unifiable->next;
		}



		printf("\nNew Query:\n");
		if (new_query_list != NULL)
			printAtomList(new_query_list);

		printf(" --> checkSLDsatisfiable( ");
		if (new_query_list != NULL)
			printAtomList(new_query_list);
		printf(", definite_list )\n");

		if (checkSLDsatisfiable(new_query_list, definite_anchor) != 1)
			return 0;
		else
			return 1;
	}
	return 1;
};

int countAtomListElem(atomlist* list){
	int elements = 0;
	while(list){
		elements++;
		list = list->next;
	}
	return elements;
}

int isAtomListEqual(atomlist* query, atomlist* new_query){
	int query_elem = countAtomListElem(query);
	int new_query_elem = countAtomListElem(new_query);

	int equal_counter = 0;

	if (query_elem != new_query_elem) {
		printf("\nEqual-Counter: %d\n\n", equal_counter);
		return 0;
	} else {
		while (query != NULL){
			while (new_query != NULL){
				if (strcmp(query->data->predicate, new_query->data->predicate) == 0)
					equal_counter++;
				new_query = new_query->next;
			}
			query = query->next;
		}
		printf("Equal-Counter: %d\n", equal_counter);
		if(query_elem != equal_counter)
			return 0;
	}

	return 1;
}