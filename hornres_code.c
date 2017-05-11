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
			printf("check: ");
			printAtom(query_body->data);
			printf(" | ");
			printAtom(definite_list->data->head);
//			printf("\n");

			atom* sigma = unify(query_body->data, definite_list->data->head);
			if(sigma != NULL){
				query_body->data = sigma;
				definite_list->data->head = sigma;
			}

			if (checkAtomsEqual(query_body->data, definite_list->data->head)) {
				printf("<- true\n");
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

atom* unify(atom *query_body, atom* definite_head) {
//	printf("unify\n");
	atom* sigma = NULL;
	atom* query_anchor = query_body;
	atom* definite_anchor = definite_head;

	while (!checkAtomsEqual(query_body, definite_head)){
		if(strcmp(query_anchor->predicate, definite_anchor->predicate) != 0){
//			printf("Not the same predicate\n");
			return NULL;
		} else {
//			printf("Same predicate\n");
			sigma = newAtom(query_anchor->predicate, NULL);
		}

		if (query_anchor->arguments != NULL || definite_anchor->arguments != NULL){
			sigma->arguments = unifyTermLists(query_anchor->arguments, definite_anchor->arguments);
			if (sigma->arguments == NULL)
				return NULL;
		}
		//query_body = sigma;
		//definite_head = sigma;
	}

	printf(" Unified sigma = ");
	if (sigma != NULL)
		printAtom(sigma);
	else
		printf("NULL");

	return sigma;
};

termlist* unifyTermLists(termlist* termlist1, termlist* termlist2){
	if(termlist1 != NULL || termlist2 != NULL)
		printf("\tunifyTermLists %s | %s - ", termlist1->data->varorfunc, termlist2->data->varorfunc);
	else
		printf("\tunifyTermLists 0 | 0");


	termlist* tmp;

	if (termlist1 == NULL || termlist2 == NULL) {
		return NULL;
	} else if (checkVar(termlist1->data->varorfunc) && checkVar(termlist2->data->varorfunc)){
		printf("both Vars!");
		tmp = newTermList(termlist2->data);
		tmp->next = unifyTermLists(termlist1->next, termlist2->next);
//		return tmp;
	} else if (checkVar(termlist1->data->varorfunc)) {
		printf("1 is Var");
		if (checkSubterm(termlist1->data->varorfunc, termlist2->data->arguments)){
			return NULL;
		} else {
			tmp = newTermList(termlist2->data);
			tmp->next = unifyTermLists(termlist1->next, termlist2->next);
//			return tmp;
		}
	} else if (checkVar(termlist2->data->varorfunc)) {
		printf("2 is Var");
		if (checkSubterm(termlist2->data->varorfunc, termlist1->data->arguments)){
			return NULL;
		} else {
			tmp = newTermList(termlist1->data);
			tmp->next = unifyTermLists(termlist1->next, termlist2->next);
//			return tmp;
		}
	} else {
		printf("both not Vars");
		if(strcmp(termlist1->data->varorfunc, termlist2->data->varorfunc) == 0) {
			tmp = newTermList(termlist1->data);
			tmp->data->arguments = unifyTermLists(termlist1->data->arguments, termlist2->data->arguments);
			tmp->next = unifyTermLists(termlist1->next, termlist2->next);
//			return tmp;
		} else
			return NULL;
	}
	return tmp;
}

int checkSubterm(char *var, termlist* subterm){
//	printf("checkSubterm %s", var);

	if (subterm == NULL){
//		printf("\n");
		return 0;
	} else {
//		printTermList(subterm);
//		printf("\n");
		while (subterm != NULL) {
			if (strcmp(var, subterm->data->varorfunc) != 0) {
				if(!checkSubterm(var, subterm->data->arguments))
					return 0;
			} else {
				subterm = subterm->next;
			}
		}
	}

	return 1;
}

int checkVar(char* var){
	char* buffer = xstrdup(var);

	buffer[strlen(buffer)-1] = 0;
	if(strcmp(buffer, "v-") == 0)
		return 1;

	free(buffer);
	return 0;
}

int checkAtomsEqual(atom *atom1, atom *atom2){
//	printf("checkAtomsEqual\n");
	if (strcmp(atom1->predicate, atom2->predicate) == 0) {
		if (checkTermListsEqual(atom1->arguments, atom2->arguments))
			return 1;
	}

	return 0;
}

int checkTermListsEqual(termlist* termlist1, termlist* termlist2) {
//	printf("checkTermListsEqual\n");
	while(termlist1 != NULL && termlist2 != NULL){
		if (!checkTermsEqual(termlist1->data, termlist2->data))
			return 0;
		termlist1 = termlist1->next;
		termlist2 = termlist2->next;
	}

	return 1;
}

int checkTermsEqual(term* term1, term* term2){
//	printf("checkTermsEqual\n");
	if (strcmp(term1->varorfunc, term2->varorfunc) == 0) {
		if (checkTermListsEqual(term1->arguments, term2->arguments))

			return 1;
	}
	return 0;
}
