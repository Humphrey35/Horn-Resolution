
%option noyywrap

%{
	#include "hornres.h"
	#include "hornres.tab.h"
%}

DIGIT      [0-9]
PRED       [A-Z]{DIGIT}*
FUNC       [a-m]{DIGIT}*
VAR        [n-z]{DIGIT}*

%%

"&" {return AND;}
"->" {return JUNC;}
"," {return COMMA;}
"(" {return OB;}
")" {return CB;}
\n  {return NEWLINE;}
\r  {return NEWLINE;}
"true" {return TRUE;}
"false" {return FALSE;}

{VAR} {yylval.vorf = strdup(yytext); return VORF;}
{FUNC} {yylval.vorf = strdup(yytext); return VORF;}
{PRED} {yylval.pred = strdup(yytext); return PRED;}

[ ] { /* Skip  whitespace*/ }

 /* Everything else is an invalid character. */
.    { return ERROR;}

%%
