%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"
#include "parse.h"
#include "line.h"



/* yy* */


int yydebug = 0;

extern int yylineno;

extern FILE * yyin;

extern char * yytext;

extern int yylex(void);
extern int yyunput(int c, register char * yy_bp );

static void
yyerror(const char *format, ...)
{
	LINE * line;
	va_list arg;
	FILE * file;
	file = stderr;
	
	line = line_revert(yylineno);
	fprintf(file, "YYerror @ [%s:%d]: \"%s\"\n", 
			line->file, line->line, yytext);
	
	va_start(arg, format);
	vfprintf(file, format, arg);
	va_end(arg);
	fprintf(file, "\n");
}


%}

%union
{
	char * string;
	int integer;
}
%type <integer> value_int
%type <string> value_string

%token <string> FLOAT
%token <string> DEC
%token <string> OCT
%token <string> HEX
%token MODULE
%token INPUT
%token OUTPUT
%token INOUT
%token CONTAIN
%token RAM
%token NET
%token CODE
%token <string> TOKEN

%start all


%%
all:
	segments
;

segments:
	/* empty */
	|
	segments ccode
	|
	segments module
;

module:
	MODULE TOKEN
	{
		parse_module_start($2);
		free($2);
		if(common_get_error_count()) YYABORT;
	}
	cycle description
	{
		parse_module_finish();
		if(common_get_error_count()) YYABORT;
	}
;

cycle:
	/* empty */
	{
		parse_cycle(1);
	}
	|
	'(' value_int ')'
	{
		parse_cycle($2);
	}
;

description:
	'{' ports internal '}'
;
	

ports:
	/* empty */
	|
	ports port
;

port:
	input_port
	|
	output_port
	|
	inout_port
;

input_port:
	INPUT '[' TOKEN ']'
	{
		parse_input_start($3);
		free($3);
	}
	dimensions names ';'
	{
		parse_element_finish();
	}

;

output_port:
	OUTPUT '[' TOKEN ']'
	{
		parse_output_start($3);
		free($3);
	}
	dimensions names ';'
	{
		parse_element_finish();
	}

;

inout_port:
	INOUT '[' TOKEN ']'
	{
		parse_inout_start($3);
		free($3);
	}
	dimensions names ';'
	{
		parse_element_finish();
	}
;

internal:
	structural
	|
	behavioral
;

structural:
	entities
;

entities:
	/* empty */
	|
	entities entity
;

entity:
	contain
	|
	net
;

behavioral:
	rams ccode
;

rams:
	/* empty */
	|
	rams ram
;

ram:
	RAM '[' TOKEN ']'
	{
		parse_ram_start($3); /* data-type */
		free($3);
	}
	dimensions names ';'
	{
		parse_element_finish();
	}

;

contain:
	CONTAIN TOKEN 
	{
		parse_contain_start($2); /* module-type */
		free($2);
	}
	names ';'
	{
		parse_element_finish();
	}
;

net:
	NET
	{
		parse_net_start();
	}
	ends ';'
	{
		parse_net_finish();
	}
	;

ccode:
	CODE
	{
		parse_code();
	}
;

dimensions:
	/* empty */
	|
	dimensions dimension
;

dimension:
	'[' value_int ']'
	{
		parse_dimension($2);
	}
;

names:
	name
	|
	names ',' name
;

name:
	TOKEN
	{
		parse_name($1);
		free($1);
	}
;

ends:
	end
	|
	ends ',' end
;

end:
	TOKEN ways
	{
		parse_net_element($1);
		free($1);
	}
	|
	TOKEN '.' TOKEN ways
	{
		parse_net_element_sub($1, $3);
		free($1);
		free($3);
	}
	|
	value_string
	{
		parse_net_element_value($1);
	}
;

ways:
	dimensions way
	|
	dimensions
;

way:
	'[' value_int ':' value_int ']'
	{
		parse_way($2, $4);
	}
;

value_int:
	DEC
	{
		sscanf($1, "%d", &($$));
	}
	|
	OCT
	{
		sscanf($1, "%o", &($$));
	}
	|
	HEX
	{
		sscanf($1, "%x", &($$));
	}
;

value_string:
	DEC
	{
		
		$$ = $1;
	}
	|
	OCT
	{
		$$ = $1;
	}
	|
	HEX
	{
		$$ = $1;
	}
	|
	FLOAT
	{
		$$ = $1;
	}
;
