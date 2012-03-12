
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "graph.h"
#include "util.h"
#include "config.h"

extern FILE * file_script;



static void
 graph_recursive_entity(SIM_TREE * node, int level);

static void
 graph_write_tabs(int count);

static void
 graph_write_to_script(const char * format, ...);

static char *
 graph_convert_entity_name(char *name);



void
graph_generate(SIM_TREE * entity_tree, SIM_LIST * sorted[])
{
	if(file_script == NULL){
		return;
	}

	graph_recursive_entity(entity_tree, 1);
}

void
graph_dot(void)
{
	char command[1024];
	char * script;

	script = config_get_file_name_script();
	sprintf(command, "dot -Tpng -o %s.png %s\n", script, script);
	system(command);
}

static void
graph_recursive_entity(SIM_TREE * node, int level)
{
	char * name;
	char * name2;
	SIM_LIST *temp;

	while(node){
		graph_write_tabs(level - 1);
		name = graph_convert_entity_name(node->name);


		if(level == 1){
			graph_write_to_script("digraph %s {\n", name);
			//graph_write_to_script("\tfontcolor=crimson;\n");

			graph_write_to_script("\tfontname=\"monospace\";\n");
			graph_write_to_script("\tfontsize=40;\n");
			graph_write_to_script("\tstyle=rounded;\n");
			//graph_write_to_script("\tnode [style=filled];\n");
			graph_write_to_script("\tnode [fontname=\"monospace\",shape=box,fontsize=32];\n");
		}else{
			graph_write_to_script("subgraph cluster_%s {\n", name);
		}


		graph_write_tabs(level);
		graph_write_to_script("labelloc = t;\n");


		graph_write_tabs(level);
		name2 = util_split_last(name, '_');
		graph_write_to_script("label = \"%s\";\n", name2);

		free(name);
		free(name2);
		name = NULL;
		name2 = NULL;


		//graph_write_tabs(level);
		if(node->module->behavior){
			//graph_write_to_script("color=navy;\n");
		}else{
			//graph_write_to_script("color=gold;\n");
		}

		temp = node->module->input;
		while(temp){
			graph_write_tabs(level);

			name = graph_convert_entity_name(node->name);
			name2 = graph_convert_entity_name(temp->name1);

			//graph_write_to_script("node [label = \"%s_%s\",", name, name2);
			graph_write_to_script("node [label = \"%s\",", name2);
			graph_write_to_script("shape=box,");

			if(level == 1){
				//graph_write_to_script("color=sienna] ");
			}else if(node->module->behavior){
				//graph_write_to_script("color=blue] ");
			}else{
				//graph_write_to_script("color=gray] ");
			}
			graph_write_to_script("] ");

			graph_write_to_script("%s_%s;\n", name, name2);
			free(name);
			free(name2);
			temp = temp->next;
		}

		temp = node->module->output;
		while(temp){
			graph_write_tabs(level);

			name = graph_convert_entity_name(node->name);
			name2 = graph_convert_entity_name(temp->name1);

			//graph_write_to_script("node [label = \"%s_%s\",", name, name2);
			graph_write_to_script("node [label = \"%s\",", name2);
			graph_write_to_script("shape=box,");

			if(level == 1){
				//graph_write_to_script("color=sienna] ");
			}else if(node->module->behavior){
				//graph_write_to_script("color=red] ");
			}else{
				//graph_write_to_script("color=gray] ");
			}
			graph_write_to_script("] ");

			graph_write_to_script("%s_%s;\n", name, name2);
			free(name);
			free(name2);
			temp = temp->next;
		}

		temp = node->module->inout;
		while(temp){
			graph_write_tabs(level);

			name = graph_convert_entity_name(node->name);
			name2 = graph_convert_entity_name(temp->name1);

			//graph_write_to_script("node [label = \"%s_%s\",", name, name2);
			graph_write_to_script("node [label = \"%s\",", name2);
			graph_write_to_script("shape=box,");

			if(level == 1){
				//graph_write_to_script("color=sienna] ");
			}else if(node->module->behavior){
				//graph_write_to_script("color=green] ");
			}else{
				//graph_write_to_script("color=gray] ");
			}
			graph_write_to_script("] ");

			graph_write_to_script("%s_%s;\n", name, name2);
			free(name);
			free(name2);
			temp = temp->next;
		}
		if(node->child){
			graph_recursive_entity(node->child, level + 1);
		}
		temp = node->module->link;
		while(temp){
			graph_write_tabs(level);

			name = graph_convert_entity_name(node->name);
			graph_write_to_script("%s_", name);
			free(name);

			name = graph_convert_entity_name(temp->name2);
			graph_write_to_script("%s ->", name);
			free(name);

			name = graph_convert_entity_name(node->name);
			graph_write_to_script("%s_", name);
			free(name);

			name = graph_convert_entity_name(temp->name1);
			graph_write_to_script("%s", name);
			free(name);

			if(temp->value1){
				graph_write_to_script(" [arrowhead=dot, arrowtail=dot];\n");
			}else{
				graph_write_to_script(" [arrowhead=normal, arrowtail=inv];\n");
			}

			temp = temp->next;
		}

		graph_write_tabs(level - 1);
		graph_write_to_script("}\n");


		node = node->brother;
	}
}

static void
graph_write_tabs(int count)
{
	int i;
	for(i = 0; i < count; i++){
		graph_write_to_script("\t");
	}
}

static void
graph_write_to_script(const char * format, ...)
{
	va_list arg;

	va_start(arg, format);
	vfprintf(file_script, format, arg);
	va_end(arg);
}

static char *
graph_convert_entity_name(char *name)
{
	char * temp;
	char * x1;
	char * x2;
	int length;

	x1 = strchr(name, ':');

	if(x1){
		// port, abc.def:g -->> abc_def__g
		length = strlen(name);
		length += 3;
		temp = (char *)malloc(sizeof(char) * length);
		strcpy(temp, name);
		x2 = strchr(temp, ':');
		strcpy(x2 + 1, x1);
		*x2 = '_';
		*(x2+1) = '_';
	}else{
		temp = strdup(name);
	}
	// module, abc.def.ghi -->> abc_def_ghi
	x1 = temp;
	while (*x1) {
		if (*x1 == '.') {
			*x1 = '_';
		}
		x1++;
	}

	return temp;
}
