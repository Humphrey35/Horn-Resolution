//
// Created by Hans Potsch on 18.04.17.
//

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

typedef struct varlist {
	char* var;
	char* new_name;
	int var_count;
	int line;
	struct varlist* next;
} varlist;
/*---------------------------------------------------------------------------*/

int yyerror(char* err);

/*---------------------------------------------------------------------------*/

void* xmalloc(size_t size);
char* xstrdup(char* str);

/*---------------------------------------------------------------------------*/

term* newTerm(char* payload, termlist* arguments);
termlist* newTermList(term* data);

atom* newAtom(char* payload, termlist* arguments);
atomlist* newAtomList(atom* data);

formular* newFormular(atom* head, atomlist* body);
formularlist* newFormularList(formular* data);

termlist* addTermListElem(termlist* anchor, termlist* next);
atomlist* addAtomListElem(atomlist* anchor, atomlist* next);
formularlist* addFormularListElem(formularlist* anchor, formularlist* next);


varlist* newVar(char* var, varlist* anchor);
void printVarList(varlist* anchor);
char* varRename(char* var, varlist* anchor);


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