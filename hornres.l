
%option noyywrap

%{
	#include <string.h>
	#include "hornres_data_strct.h"
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

{VAR} 	{yylval.var = strdup(yytext);
		return VAR;}
{FUNC} 	{yylval.func = strdup(yytext);
		return FUNC;}
{PRED} 	{yylval.pred = strdup(yytext);
		return PRED;}

[ ] { /* Skip  whitespace*/ }

 /* Everything else is an invalid character. */
.    { return ERROR;}

%%
