//
// Created by Hans Potsch on 18.04.17.
//
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
    atomlist* next;

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
    termlist* next;

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
