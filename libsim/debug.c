#include <stdlib.h>
#include "debug.h"



static DEBUG_REGISTER * __register = NULL;
static DEBUG_REGISTER * __register_tail = NULL;
static int cycle;


void
debug_register_variable(char * name, DATA_TYPE_E type, void * data)
{
	DEBUG_REGISTER * temp;

	temp = (DEBUG_REGISTER *)malloc(sizeof(DEBUG_REGISTER));
	temp->name = name;
	temp->type = type;
	temp->data = data;
	temp->next = NULL;

	if(__register_tail){
		__register_tail->next = temp;
	}

	if(__register == NULL){
		__register = temp;
	}

	__register_tail = temp;
}
/*
void
debug_find_variable(char * name)
{
	DEBUG_REGISTER * temp;
	temp = __register;
	while(temp){
		if(strcmp(temp->name, name) == 0){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}
*/

void
debug_print_all(void){

	DEBUG_REGISTER * temp = __register;
	while(temp){
		debug_print_variable(temp, stdout);
		temp = temp->next;
	}
}
void
debug_print_variable(DEBUG_REGISTER * var, FILE * file)
{
	char * type;
	char * format;
	switch(var->type){
		case U32:
			type = "U32";
			format = "%08x";
			break;
		case U16:
			type = "U16";
			format = "%04x";
			break;
		case U08:
			type = "U08";
			format = "%02x";
			break;
		case I32:
			type = "I32";
			format = "%d";
			break;
		case I16:
			type = "I16";
			format = "%d";
			break;
		case I08:
			type = "I08";
			format = "%d";
			break;
		case F64:
			type = "F64";
			format = "%lf";
			break;
		case F32:
			type = "F32";
			format = "%f";
			break;
		case W01:
			type = "W01";
			format = NULL;
			break;
		default:
			type = "???";
			format = "???";
			break;
	}

	fprintf(file, "+------------------\n| %s\n| %s\n| ", var->name, type);
	if(format == NULL){
		fprintf(file, "%s", *(var->data) ? "True" : "False");
	}else{
		fprintf(file, format, *(var->data));
	}
	fprintf(file, "\n+------------------\n");
}

void
debug_enter(void)
{
	char buffer[1024];
	buffer[1023] = '\0';

	cycle++;

	printf("[cycle : %d]\n", cycle);
	debug_print_all();
	fgets(buffer, 1023, stdin);

}
