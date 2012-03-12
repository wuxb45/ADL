#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "util.h"
#include "common.h"
#include "config.h"

#include "code.h"








static void
code_create_h_typedef(SIM_MODULE_LIST * module_list);

static void
code_typedef(SIM_MODULE * module);

static void
code_create_h_schedule_declare(char * top_module);

static void
code_create_c_head(void);

static void
code_create_c_global_entities(SIM_MODULE_TREE * entity_tree);

static void
code_recursive_global_entity(SIM_MODULE_TREE * node);

static void
code_global_entity_declare(SIM_MODULE_TREE * node);

static void
code_global_entity_initial(SIM_MODULE_TREE * node);

static void
code_create_c_global_initial_all(SIM_MODULE_TREE * root);

static void
code_recursive_global_initial(SIM_MODULE_TREE * node);

static void
code_create_c_behavior_function(SIM_MODULE_LIST * module_list);

static void
code_behavior_function(SIM_MODULE * module);

static void
code_create_c_schedule_all(SIM_MODULE_TREE * root, SIM_LINK_LIST * link_list);

static void
code_behavior_execute_link(SIM_LINK * link);

static void
code_behavior_execute_entity(SIM_MODULE_TREE * node);

static void
code_write_user_ccode(SIM_MODULE * module);

static void
code_convert_ccode_reference(char *code, SIM_MODULE * module);

static void
code_trace_element_list(SIM_MODULE * module, SIM_ELEMENT_TYPE type);

static void
code_trace_execute_link(SIM_LINK * link);

static void
code_trace_execute_entity(SIM_MODULE_TREE * node);

static void
code_create_c_main(char * top_module_name);

static char *
code_convert_entity_name(char *name);

static void
code_write_to_h(const char *format, ...);

static void
code_write_to_c(const char *format, ...);
//
static void
code_print(const char *format, ...);


void
code_compile(void)
{
	char command[1024];
	char * ccode;
	char * out;
	ccode = config_get_file_name(CONFIG_FILE_ID_SIM_C);
	out = config_get_file_name(CONFIG_FILE_ID_OUT);
	sprintf(command, "gcc -o %s %s -lsim\n", out, ccode);

	printf("COMPILE: %s\n", command);
	system(command);

}




void
code_do(SIM_MODULE_LIST * module_list, SIM_MODULE_TREE * entity_tree, SIM_LINK_LIST * link_list)
{
	code_print("Code Begin.\n");

	code_create_h_typedef(module_list);
	code_create_h_schedule_declare(config_get_top_module());

	code_create_c_head();
	code_create_c_global_entities(entity_tree);
	code_create_c_global_initial_all(entity_tree);
	code_create_c_behavior_function(module_list);
	code_create_c_schedule_all(entity_tree, link_list);
	code_create_c_main(entity_tree->name);
}













/**
 * do nothing
 */
//static void
//code_create_h_head(void)
//{
//	return;
//}

static void
code_create_h_typedef(SIM_MODULE_LIST * module_list)
{
	SIM_MODULE *module;
	SIM_MODULE_LIST * mlist;

	// others
	for(mlist = module_list; mlist; mlist = mlist->next){

		module = mlist->module;

		if (module->behavior && module->entity_count) {
			code_typedef(module);
		}
	}
	return;
}



/**
 * module's typedef in header file
 */
static void
code_typedef(SIM_MODULE * module)
{
//	SIM_LIST *list;
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;
	char * temp;



	code_write_to_h(CODE_TYPEDEF_STRUCT);
	code_write_to_h(CODE_STRUCT_NAME_PREFIX "%s {\n", module->name);

	// cycle-param
//	if(module->behavior){
//		code_write_to_h("\t/* cycle value. */\n");
//		code_write_to_h("\t" CODE_SIM_TYPE_PREFIX);
//		code_write_to_h("xxxx");
//	}
	// global name
	code_write_to_h("\t char * " CODE_ENTITY_SELF_NAME ";\n");

	//elements
	for(elist = module->element_list; elist; elist = elist->next){
		element = elist->element;

		switch(element->type){
			case ELEMENT_INPUT:
				code_write_to_h("\t/* input port. */\n");
				break;
			case ELEMENT_OUTPUT:
				code_write_to_h("\n\t/* output port. */\n");
				break;
			case ELEMENT_INOUT:
				code_write_to_h("\n\t/* inout port. */\n");
				break;
			case ELEMENT_RAM:
				code_write_to_h("\n\t/* ram. */\n");
				break;
//			case ELEMENT_CONTAIN:
//				break;
//			case ELEMENT_UNKNOWN:
//				break;
			default:
				common_error("code xxx");
				return;
				break;
		}
		code_write_to_h("\t" CODE_SIM_TYPE_PREFIX);
		temp = common_data_type_to_string(element->data_type);
		code_write_to_h("%s (* %s)", temp, element->name);


		temp = common_dimension_to_string(element->dimension);
		code_write_to_h("%s;\n", temp);
	}

	code_write_to_h("\n} " CODE_TYPEDEF_PREFIX "%s;\n\n", module->name);
	return;
}

/**
 * 2 function of initial and run-cycle
 */
static void
code_create_h_schedule_declare(char * top_module)
{
	// declare initial
	code_write_to_h(CODE_MAIN_INITIAL_DECLARE "(void);\n\n");

	// declare entry
	code_write_to_h(CODE_SCHEDULE_DECLARE_PREFIX "%s(void);\n\n", top_module);
	return;
}

/**
 * copy %%include%% into c file,
 * append include libsim,
 * append include libc headers,
 *
 */
static void
code_create_c_head(void)
{
	char buffer[1024] = {0};
	char * header;
	FILE * code2;

	code_write_to_c("#include <stdio.h>\n\n");
	code_write_to_c("#include <stdlib.h>\n\n");
	code_write_to_c("#include <libsim/sim.h>\n\n");

	header = config_get_file_name(CONFIG_FILE_ID_SIM_H);
	if(header != NULL){
		code_write_to_c("#include \"%s\"\n", header);
	}

	code2 = config_get_file(CONFIG_FILE_ID_CODE2);
	rewind(code2);
	while(fgets(buffer, 1023, code2)){

		if(strncmp(buffer, "%%%", 3) == 0){

			while(fgets(buffer, 1023, code2)){

				if(strncmp(buffer, "%%", 2) == 0){
					break;
				}

				code_write_to_c("%s", buffer);
			}
		}
	}
	return;
}


/**
 * all global : definitions / initial functions / initial_all
 */
static void
code_create_c_global_entities(SIM_MODULE_TREE * entity_tree)
{
	code_recursive_global_entity(entity_tree);
	return;
}

/**
 * through the tree
 */
static void
code_recursive_global_entity(SIM_MODULE_TREE * node)
{
	SIM_MODULE_TREE *temp;


	for (temp = node; temp; temp = temp->brother) {
		code_recursive_global_entity(temp->child);
		if (temp->module->behavior == 0) {

			continue;
		}
		code_global_entity_declare(temp);
		code_global_entity_initial(temp);
	}
	return;
}

static void
code_global_entity_declare(SIM_MODULE_TREE * node)
{
	char *entity_prefix;
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;

	SIM_MODULE *module;
	char * temp;

	int initial_zero;

	if(config_get_initial_value() == '0'){
		initial_zero = 1;
	}else{
		initial_zero = 0;
	}


	module = node->module;

	entity_prefix = code_convert_entity_name(node->name);

	code_write_to_c("/* BEGIN entity content : %s */\n", node->name);

	for(elist = module->element_list; elist; elist = elist->next){

		element = elist->element;

		switch(element->type){
		case ELEMENT_INPUT:
		case ELEMENT_OUTPUT:
		case ELEMENT_INOUT:
		case ELEMENT_RAM:

			code_write_to_c(CODE_STATIC CODE_SIM_TYPE_PREFIX);

			temp = common_data_type_to_string(element->data_type);
			code_write_to_c("%s  " CODE_GLOBAL_PORT_NAME_PREFIX, temp);

			code_write_to_c("%s__%s", entity_prefix, element->name);

			temp = common_dimension_to_string(element->dimension);
			code_write_to_c("%s", temp);
			if(initial_zero){
				code_write_to_c(" = {0}");
			}
			code_write_to_c(";\n");


			break;
//		case ELEMENT_CONTAIN:
//			break;
//		case ELEMENT_UNKNOWN:
//			break;
		default:
			break;
		}
	}

	// entity struct
	code_write_to_c("\n/* entity struct declaration */\n");
	code_write_to_c(CODE_STATIC CODE_TYPEDEF_PREFIX "%s  ", module->name);
	code_write_to_c(CODE_ENTITY_NAME_PREFIX "%s;\n\n", entity_prefix);

	return;
}

static void
code_global_entity_initial(SIM_MODULE_TREE * node)
{
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;

	SIM_MODULE *module;
	char *entity_prefix;

	char ** split_ways;
	char * way;
	char * way_cut;
	int i;
	int width;

	module = node->module;

	entity_prefix = code_convert_entity_name(node->name);

	// initial function
	code_write_to_c(CODE_STATIC CODE_INITIAL_DECLARE_PREFIX "%s", entity_prefix);
	code_write_to_c("(void)\n{\n");

	// entity name

	code_write_to_c("\t" CODE_ENTITY_NAME_PREFIX "%s.", entity_prefix);
	code_write_to_c(CODE_ENTITY_SELF_NAME "= \"%s\";\n", node->name);


	for(elist = module->element_list; elist; elist = elist->next){

		element = elist->element;

		switch(element->type){
		case ELEMENT_INPUT:
		case ELEMENT_OUTPUT:
		case ELEMENT_INOUT:
		case ELEMENT_RAM:
			code_write_to_c("\t" CODE_ENTITY_NAME_PREFIX);
			code_write_to_c("%s.%s = &(", entity_prefix, element->name);
			code_write_to_c(CODE_GLOBAL_PORT_NAME_PREFIX);
			code_write_to_c("%s__%s);\n", entity_prefix, element->name);

			// vcd var
			split_ways = element->split_ways;
			if(element->data_type == SDT_W01 && element->dimension->count){
				width = element->dimension->width[element->dimension->count-1];
			}else{
				width = 0;
			}

			for(i = 0; split_ways[i]; i++){
				if(width && (i % width)){
					continue;
				}

				code_write_to_c("\t" CODE_VCD_SYMBOL_ADD);
				code_write_to_c("(\"%s:%s", entity_prefix, element->name);

				if(width){
					way = strdup(element->split_ways[i]);
					way_cut = strrchr(way, '[');
					if(way_cut){
						*way_cut = '\0';
					}
				}else{
					way = element->split_ways[i];
				}
				code_write_to_c("%s\", ", way);

				code_write_to_c("\"\\\"%s__%s%s\\\"\", ", entity_prefix, element->name, way);
				if(width){
					free(way);
				}

				code_write_to_c("ST_%s, ", common_data_type_to_string(element->data_type));
				code_write_to_c("&(" CODE_GLOBAL_PORT_NAME_PREFIX);
				code_write_to_c("%s__%s%s), ", entity_prefix, element->name, element->split_ways[i]);
				code_write_to_c("%d);\n", width ? width : 1);
			}
			// vcd var end
			break;
		default:
			break;
		}
	}


	code_write_to_c("\n}\n");
	code_write_to_c("/* END entity content : %s */\n\n\n", entity_prefix);

	free(entity_prefix);
}

static void
code_create_c_global_initial_all(SIM_MODULE_TREE * root)
{
	char * vcd;
	code_write_to_c(CODE_MAIN_INITIAL_DECLARE "(void)\n{\n");

	vcd = config_get_file_name(CONFIG_FILE_ID_VCD);
	code_write_to_c("\t" CODE_VCD_INITIAL "(\"%s\");\n", vcd);

	code_recursive_global_initial(root);
	code_write_to_c("\t" CODE_VCD_ENDDEF "();\n");
	code_write_to_c("}\n\n");
}

/**
 * call all initial function of entities.
 */
static void
code_recursive_global_initial(SIM_MODULE_TREE * node)
{
	SIM_MODULE_TREE *temp;
	char * name;

	if(config_get_trace()){
		code_write_to_c("\t" );
	}

	for (temp = node; temp; temp = temp->brother) {
		// vcd scope
		name = strrchr(temp->name, '.');
		if(name){
			name++;
		}else{
			name = temp->name;
		}
		code_write_to_c("\t" CODE_VCD_SCOPE "(\"%s\");\n", name);


		if(temp->child){
			code_recursive_global_initial(temp->child);
		}

		if (temp->module->behavior) {
			name = code_convert_entity_name(temp->name);
			code_write_to_c("\t" CODE_INITIAL_NAME_PREFIX "%s();\n", name);
			free(name);
		}

		// vcd upscope
		code_write_to_c("\t" CODE_VCD_UPSCOPE "();\n");
	}
	return;
}

static void
code_create_c_behavior_function(SIM_MODULE_LIST * module_list)
{
	SIM_MODULE_LIST *temp;
	SIM_MODULE * module;

	for(temp = module_list; temp; temp = temp->next) {
		module = temp->module;
		if (module->behavior && module->entity_count) {
			code_behavior_function(module);
		}
	}
	return;
}

static void
code_behavior_function(SIM_MODULE * module)
{
	// head
	code_write_to_c(CODE_STATIC CODE_BEHAVIOR_DECLARE_PREFIX "%s (", module->name);

	code_write_to_c(CODE_TYPEDEF_PREFIX "%s * " CODE_FUNCTION_SELF_REF, module->name);
	code_write_to_c(")\n{\n");

	// print entity trace
	if(config_get_trace()){
		code_trace_element_list(module, ELEMENT_INPUT);
	}

	// convert & copy codes
	code_write_user_ccode(module);

	// print output trace
	if(config_get_trace()){
		code_trace_element_list(module, ELEMENT_OUTPUT);
	}

	code_write_to_c("\n}\n\n");

	return;
}

static void
code_create_c_schedule_all(SIM_MODULE_TREE * root, SIM_LINK_LIST * link_list)
{

	char *top_module_name;
	SIM_LINK_LIST * list;
	SIM_LINK * link;
	int i;
	int count;
	SIM_MODULE_TREE * node;
	SIM_MODULE * module;


	top_module_name = root->name;
	count = root->sortid;

	// function head
	code_write_to_c("\t/* Schedule function. */\n");
	code_write_to_c(CODE_SCHEDULE_DECLARE_PREFIX "%s(void)\n{\n\n", top_module_name);

	// set const
	for(list = link_list; list; list = list->next){
		link = list->link;
		if(link->type == LINK_CONST){
			common_link_print(link);
			code_behavior_execute_link(link);
		}
	}

	// to-zore-cycle links
	for(i = 1; i < count; i++){
		code_print("\n\n++++[ %d ]\n", i);

		node = common_module_tree_find_by_sortid(root, i);
		if(node == NULL){
			continue;
		}

		module = node->module;
		if(module->cycle == 0){

			for(list = link_list; list; list = list->next){
				link = list->link;
				if(link->dest_module_node->sortid == i){
					common_link_print(link);
//					code_print("@: [%s]\n", link->dest_module_node->name);
					if(link->type == LINK_RW){
						code_behavior_execute_link(link);
					}
				}
			}

		}
		code_behavior_execute_entity(node);
		code_print("execute entity: [%s]\n", node->name);
	}

	// 1+ cycle links
	for(i = 1; i < count; i++){
		code_print("\n\n++++[ %d ]\n", i);

		node = common_module_tree_find_by_sortid(root, i);
		if(node == NULL){
			continue;
		}

		module = node->module;
		if(module->cycle > 0){
			for(list = link_list; list; list = list->next){
				link = list->link;
				if(link->dest_module_node->sortid == i){
					common_link_print(link);
					if(link->type == LINK_RW){
						code_behavior_execute_link(link);
					}
				}
			}
		}
	}
	code_write_to_c("\t" CODE_VCD_DUMP_ONE_CYCLE "();\n");
	code_write_to_c("\n}\n");

	return;
}

//static void
//code_behavior_function_declare(SIM_MODULE * module)
//{
//	code_write_to_h(CODE_BEHAVIOR_DECLARE_PREFIX "%s (", module->name);
//	code_write_to_h(CODE_TYPEDEF_PREFIX "%s * " CODE_FUNCTION_SELF_REF, module->name);
//	code_write_to_h(");\n\n");
//
//	return;
//}

static void
code_behavior_execute_link(SIM_LINK * link)
{
	char * name;
	char * type;
	code_write_to_c("\t/* %s = %s */\n",link->src_full_name, link->dest_full_name);

	type = common_data_type_to_string(link->dest_port_type->data_type);

	if(link->type == LINK_RW){

		code_write_to_c("\t" CODE_ASSIGN_PREFIX "%s", type);

		// dest
		name = code_convert_entity_name(link->dest_full_name);
		code_write_to_c("(&(" CODE_GLOBAL_PORT_NAME_PREFIX "%s),", name);
		free(name);

		// src

		name = code_convert_entity_name(link->src_full_name);
		code_write_to_c("&(" CODE_GLOBAL_PORT_NAME_PREFIX "%s));\n\n", name);
		free(name);
	}else{
		code_write_to_c("\t" CODE_SET_PREFIX "%s ( ", type);

		name = code_convert_entity_name(link->dest_full_name);
		code_write_to_c("&(" CODE_GLOBAL_PORT_NAME_PREFIX "%s), ", name);
		free(name);

		name = link->src_full_name;
		code_write_to_c("(%s));\n\n", name);
	}

	if(config_get_trace()){
		code_trace_execute_link(link);
	}

	return;

}

static void
code_behavior_execute_entity(SIM_MODULE_TREE * node)
{
	char *name;
	code_write_to_c("\t/* do entity [%s] */\n", node->name);

	name = code_convert_entity_name(node->name);
	code_write_to_c("\t" CODE_BEHAVIOR_NAME_PERFIX "%s", node->module->name);
	code_write_to_c("(&(" CODE_ENTITY_NAME_PREFIX "%s));\n\n", name);
	free(name);

	if(config_get_trace()){
		code_trace_execute_entity(node);
	}
	return;
}



static void
code_write_user_ccode(SIM_MODULE * module)
{
	static char buffer[1024];
	char * head;
	char * temp;
	int length;
	FILE * code2;


	buffer[0] = '\0';
	head = util_join("%%", 0, module->name, 0);
	length = strlen(head) + 1;

	code2 = config_get_file(CONFIG_FILE_ID_CODE2);
	if(code2 == NULL){
		common_error("FILE error");
		return;
	}
	rewind(code2);

	while (fgets(buffer, 1023, code2)) {
		if(buffer[strlen(buffer) - 1] == '\n'){
			buffer[strlen(buffer) - 1] = '\0';
		}

		temp = util_trim_front(buffer);
		if (strncmp(head, temp, length) == 0) {
			break;
		}
	}
	code_write_to_c("\t/* User code BEGIN. */\n");

	while (fgets(buffer, 1023, code2)) {
		temp = util_trim_front(buffer);
		if (strncmp("%%", temp, 2) == 0) {
			break;
		}
		code_convert_ccode_reference(buffer, module);
		code_write_to_c("%s", buffer);
	}

	code_write_to_c("\t/* User code END. */\n");

	return;
}


static void
code_convert_ccode_reference(char *code, SIM_MODULE * module)
{
	static char buffer[1024];
	static int comment_flag = 0;
	char name[256];
	int nid;

	char *dest;
	char *src;


	buffer[0] = '\0';

	dest = buffer;
	src = code;

	// clean comment
	while(*src){
		if(comment_flag == 0){
			if(src[0] == '/'){
				if(src[1] == '/'){
					*src = '\0';
					break;
				}

				if(src[1] == '*'){
					comment_flag = 1;
//					src[0] = ' ';
//					src[1] = ' ';
					src++;
				}
			}
		}else{
			if(src[0] == '*' && src[1] == '/'){
				comment_flag = 0;

//				src[0] = ' ';
//				src[1] = ' ';
				src++;
			}else{
				if(!isspace(*src)){
					*src = ' ';
				}
			}
		}
		src++;
	}

	// main convert

	for (src = code; *src; *(dest++) = *(src++)) {
		if (*src != '$') {		// $(var)
			continue;
		}

		src++;				// (
		if (*src == '(') {  // $(var) -->> (*(__entity__->var))
			src++;			// name
			// prefix
			strcpy(dest, "(*(" CODE_FUNCTION_SELF_REF "->");
			dest = buffer + (strlen(buffer));
			nid = 0;
			while ((*src) && *src != ')') {
				*dest = *src;
				name[nid++] = *src;
				dest++;
				src++;
			}

			*dest = ')';
			dest++;

			name[nid] = '\0';
			if (common_element_list_find_name(module->element_list, name) == NULL) {
				common_error("C code variable name not found : \"%s\".\n", name);
			}

		} else {			// $var -->> (*(__entity->var))

			strcpy(dest, "(*(" CODE_FUNCTION_SELF_REF "->");
			dest = buffer + (strlen(buffer));
			if (!isalpha(*src) && *src != '_') {
				return;
			}
			nid = 0;
			while ((*src) && (isalnum(*src) || *src == '_')) {
				*dest = *src;
				name[nid++] = *src;
				dest++;
				src++;

			}
			name[nid] = '\0';
			if (common_element_list_find_name(module->element_list, name) == NULL) {
				common_error("C code variable name not found :"
				"\"%s\". @ [%s], %s\n", name, module->name, code);
			}
			*dest = ')';
			dest++;
			*dest = ')';
			dest++;
		}

	}
	*dest = '\0';
	strcpy(code, buffer);
	return;
}

static void
code_trace_element_list(SIM_MODULE * module, SIM_ELEMENT_TYPE type)
{
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;
	char ** split_dims;
	char * temp;
	int i;

//	code_write_to_c("\t" CODE_TRACE_TEXT );
//	code_write_to_c("(\"MODULE:[%s]:\");\n", module->name);
//
//	code_write_to_c("\t" CODE_TRACE_TEXT );
//	code_write_to_c("(" CODE_FUNCTION_SELF_REF "->" CODE_ENTITY_SELF_NAME ");\n");

//	code_write_to_c("\t" CODE_TRACE_TEXT "(\"\\n\");\n\n" );

	code_write_to_c("\t/* TRACE %s ports BEGIN */\n", common_data_type_to_string(type));
	for (elist = module->element_list; elist; elist = elist->next) {
		element = elist->element;

		if(element->type != type){
			continue;
		}

		temp = common_data_type_to_string(element->data_type);
		split_dims = common_dimension_split(NULL, element->dimension);

		code_write_to_c("\t" CODE_TRACE_TEXT );
		code_write_to_c("(\"[%s]:\");\n", module->name);

		code_write_to_c("\t" CODE_TRACE_TEXT );
		code_write_to_c("(" CODE_FUNCTION_SELF_REF "->" CODE_ENTITY_SELF_NAME ");\n");

		code_write_to_c("\t" CODE_TRACE_TEXT );
		code_write_to_c("(\":%s\\n\");\n", element->name);

		for(i = 0; split_dims[i]; i++){
			code_write_to_c("\t" CODE_TRACE_TEXT );
			code_write_to_c("(\"\\t\\t%s:\");\n", split_dims[i]);

			code_write_to_c("\t" CODE_TRACE_VALUE_PREFIX "%s(&(", temp);

			code_write_to_c("(*(" CODE_FUNCTION_SELF_REF "->%s))", element->name);
			code_write_to_c("%s));\n", split_dims[i]);
		}
	}

	code_write_to_c("\t/* TRACE %s ports END */\n", common_data_type_to_string(type));
}



static void
code_trace_execute_link(SIM_LINK * link)
{
	char * temp;

	code_write_to_c("\t/* TRACE LINK BEGIN*/\n");

	code_write_to_c("\t" CODE_TRACE_TEXT );
	code_write_to_c("(\"LINK: %s ==>> %s\");\n", link->src_full_name, link->dest_full_name);

	temp = common_data_type_to_string(link->dest_port_type->data_type);
	code_write_to_c("\t" CODE_TRACE_VALUE_PREFIX "%s(&(", temp);

	temp = code_convert_entity_name(link->dest_full_name);


	code_write_to_c(CODE_GLOBAL_PORT_NAME_PREFIX  "%s));\n", temp);
	free(temp);


	code_write_to_c("\t/* TRACE LINK END*/\n\n");
}


static void
code_trace_execute_entity(SIM_MODULE_TREE * node)
{
	code_write_to_c("\t/* TRACE ENTITY BEGIN*/\n");

	code_write_to_c("\t" CODE_TRACE_TEXT );
	code_write_to_c("(\"ENTITY EXECUTE: [%s]\\n\");\n", node->name);

	code_write_to_c("\t/* TRACE ENTITY END*/\n\n");
}


//static void
//code_behavior_invoke_bus(SIM_LIST * behavior, int net)
//{
//
//	SIM_LIST *temp;
//	SIM_LIST *sample = NULL;
//	int count;
//
//	count = 0;
//	code_write_to_c("\t/* BUS group %d */\n", net);
//
//	temp = behavior;
//
//	while (temp) {
//		if (temp->value1 == net) {
//			sample = temp;
//			break;
//		}
//		temp = temp->next;
//	}
//	if(sample == NULL){
//		common_error("In code_behavior_invoke_bus: bus net [%d]\n", net);
//	}
//	code_write_to_c("\t"CODE_BUS_DO_FUNCTION_PREFIX"%s(", sample->name2);
//	code_write_to_c(CODE_ENTITY_BUS_GROUP_PREFIX"%d);\n", net);
//
//	return;
//
//}

static void
code_create_c_main(char * top_module_name)
{
	code_write_to_c("\t/* main() function. */\n");
	code_write_to_c("int\nmain(int argc, char *argv[])\n{\n");
	code_write_to_c("\tint i;\n");

	// initial
	code_write_to_c("\t" CODE_MAIN_INITIAL_NAME"();\n");

	// loop
	code_write_to_c("\tfor( i = 0; i < %d; i++ ){\n", config_get_run_cycle());
	if(config_get_trace()){
		code_write_to_c("\t\ttrace_cycle( i );\n");
	}
	code_write_to_c("\t\t" CODE_SCHEDULE_NAME_PREFIX "%s();\n", top_module_name);

	code_write_to_c("\t}\n");
	code_write_to_c("\treturn 0;\n}\n");
	return;
}

static char *
code_convert_entity_name(char *name)
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

static void
code_write_to_h(const char *format, ...)
{
	va_list arg;
	static FILE * file_h = NULL;

	if(file_h == NULL){
		file_h = config_get_file(CONFIG_FILE_ID_SIM_H);
	}

	va_start(arg, format);
	vfprintf(file_h, format, arg);
	va_end(arg);

//	va_start(arg, format);
//	vfprintf(stdout, format, arg);
//	va_end(arg);
	return;
}

static void
code_write_to_c(const char *format, ...)
{
	va_list arg;
	static FILE * file_c = NULL;

	if(file_c == NULL){
		file_c = config_get_file(CONFIG_FILE_ID_SIM_C);
	}

	va_start(arg, format);
	vfprintf(file_c, format, arg);
	va_end(arg);

//	va_start(arg, format);
//	vfprintf(stdout, format, arg);
//	va_end(arg);
	return;
}

static void
code_print(const char *format, ...)
{
	va_list arg;

	if (config_get_verbose() == 0) {
		return;
	}

	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
	return;
}
