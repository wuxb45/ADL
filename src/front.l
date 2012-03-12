%{


#include <stdio.h>

#include "common.h"
#include "front.tab.h"


%}



int_dec    [+-]?(0|([1-9][0-9]*))[uUlL]?
int_oct    0[0-9]+[uUlL]?
int_hex    0[xX][0-9a-fA-F]+[uUlL]?
float      [+-]?[0-9]+\.[0-9]*[fFlL]?


%%

{int_dec}				{yylval.string=strdup(yytext); return DEC;}
{int_oct}				{yylval.string=strdup(yytext); return OCT;}
{int_hex}				{yylval.string=strdup(yytext); return HEX;}
{float}					{yylval.string=strdup(yytext); return FLOAT;}

\[                      {return '['; }
\]                      {return ']'; }
\{                      {return '{'; }
\}                      {return '}'; }
\(                      {return '('; }
\)                      {return ')'; }
\;                      {return ';'; }
\,                      {return ','; }
\:                      {return ':'; }
\.                      {return '.'; }

"module"                {return MODULE;}
"input"                 {return INPUT;}
"output"                {return OUTPUT;}
"inout"                 {return INOUT;}
"contain"               {return CONTAIN;}
"ram"                   {return RAM;}
"net"                   {return NET;}
\%\%                    {return CODE;}

[_a-zA-Z][_0-9a-zA-Z]*  {yylval.string=strdup(yytext); return TOKEN;}
[ \t]+                  {}
[\r\n]+                 {}
.						{common_warning("Drop unsupported :[%s] @ %d", yytext, yylineno);}
%%

