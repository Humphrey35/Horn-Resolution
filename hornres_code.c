//
// Created by Hans Potsch on 18.04.17.
//

#include "hornres_code.h"

extern int var_count;
extern int line;

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
    formularlist* query_list 	= NULL;
	formularlist* tmp 			= NULL;

    while (list != NULL){
        if(strcmp(list->data->head->predicate, "false") == 0){
			tmp = newFormularList(list->data);
			query_list = addFormularListElem(query_list, tmp);
        }
        list = list->next;
    }
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

char* varRename(char* var, varList_elem* anchor){

	while (anchor != NULL){
		if(strcmp(anchor->var, var) == 0){
			break;
		}
		anchor = anchor->next;
	}

	return anchor->new_name;
}

void printVarList(varList_elem* anchor){
	char* buffer = xmalloc(sizeof(char)+ sizeof(int));

	printf("\nVar-List:\n");

	while(anchor != NULL){
		printf("%s ---> %s\n", anchor->var, anchor->new_name);
		anchor = anchor->next;
	}
	printf("\n");

	free(buffer);
}

atom* unify(atom* body_atom, atom* head){
		return head;
};

void* checkAtomsEqual(atom* one, atom* two){
	while (one != NULL && two != NULL){
		if (strcmp(one->predicate, two->predicate) == 0){

		} else {
			return one->predicate;
		}
	}
}

uniList_elem* newUnification(char* var, term* unif){
	uniList_elem* new_elem = xmalloc(sizeof(uniList_elem));

	new_elem->var = xstrdup(var);
	new_elem->unif = unif;

	return new_elem;
}

uniList addUniElem(uniList* anchor, uniList* next){

};