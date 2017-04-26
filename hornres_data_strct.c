//
// Created by Hans Potsch on 25.04.17.
//

#include "hornres_data_strct.h"

extern int var_count;
extern int line;

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

varList_elem* newVar(char* var, varList_elem* anchor){
	char* buffer;
	varList_elem* tmp = anchor;

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
	varList_elem* new_varlist = xmalloc(sizeof(varList_elem));
//	printf("\tnewVar %s %p --> %p\n", var, anchor, new_varlist);
	new_varlist->var = xstrdup(var);
	new_varlist->var_count = var_count;
	new_varlist->line = line;

	buffer = xmalloc(sizeof(char)+ sizeof(int));
	sprintf(buffer, "v-%d", new_varlist->var_count);

	new_varlist->new_name = xstrdup(buffer);
	free(buffer);

	new_varlist->next = anchor;
	anchor = new_varlist;

	return new_varlist;
}

uniList_elem* newUnification(char* var, term* unif){
	uniList_elem* new_elem = xmalloc(sizeof(uniList_elem));

	new_elem->var = xstrdup(var);
	new_elem->unif = unif;

	return new_elem;
}

uniList* addUniListElem(uniList* anchor, uniList* new_uni_l){
	new_uni_l->next = anchor;
	anchor->next = new_uni_l;

	return new_uni_l;
};