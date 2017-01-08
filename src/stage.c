#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#include "stage.h"
#include "common.h"
#include "util.h"
#include "parse.h"
#include "module.h"
#include "global.h"
#include "code.h"
//#include "graph.h"
#include "config.h"
//#include "verilog.h"
#include "pre.h"
#include "bus.h"

/**
 *  for yacc front end
 */
extern int yyparse(void);

extern FILE *yyin;


static int
stage_1(void);

static int
stage_2(void);

static int
stage_3(void);

static void
stage_print(const char *format, ...);



/**
 * implement function
 */
int
stage_do(int argc, char *argv[])
{

	int result;


	result = config_initial(argc, argv);
	if(result){
		return result;
	}


	result = stage_1();
	if (result) {
		return -1;
	}

	if(config_get_preprocess_only()){
		return 0;
	}

	result = stage_2();
	if (result) {
		return -1;
	}

	result = stage_3();
	if (result) {
		return -1;
	}


	return 0;
}


/**
 Stage 1 : Split the input source file into .arch and .code parts.
 In .arch part, %%.*%% in input file will be replaced by %%.
 In .code part, only %%.*%% segments remain.
 */
static int
stage_1(void)
{
	int result;
	stage_print("Parse_stage_1\n");

	FILE * file;
	/* open files to get ready to process. */
	file = config_file_open(CONFIG_FILE_ID_INPUT, "r");

	if(file == NULL){
		common_error("Open INPUT file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_INPUT));
		return -1;
	}

	file = config_file_open(CONFIG_FILE_ID_ARCH, "w");

	if(file == NULL){
		common_error("Open ARCH file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_ARCH));
		return -1;
	}

	file = config_file_open(CONFIG_FILE_ID_CODE1, "w");

	if(file == NULL){
		common_error("Open CODE1 file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_CODE1));
		return -1;
	}

	result = pre_process();
	if(result){
		return -1;
	}

	config_file_close(CONFIG_FILE_ID_INPUT);
	config_file_close(CONFIG_FILE_ID_ARCH);
	config_file_close(CONFIG_FILE_ID_CODE1);
	return 0;

}

/**
 * Stage 2 : scan .arch part to get modules' information.
 *
 */
static int
stage_2(void)
{
	int result;
	FILE * file;

	stage_print("Parse_stage_2\n");

	file = config_file_open(CONFIG_FILE_ID_ARCH, "r");
	if(file == NULL){
		common_error("Open ARCH file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_ARCH));
	}

	yyin = file;

	file = config_file_open(CONFIG_FILE_ID_CODE1, "r");
	if(file == NULL){
		common_error("Open CODE1 file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_CODE1));
		return -1;
	}

	file = config_file_open(CONFIG_FILE_ID_CODE2, "w");
	if(file == NULL){
		common_error("Open CODE2 file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_CODE2));
		return -1;
	}


	result = yyparse();
	if (result) {
		return result;
	}
	common_module_list_print(parse_get_module_list());

	yyin = NULL;
	config_file_close(CONFIG_FILE_ID_ARCH);
	config_file_close(CONFIG_FILE_ID_CODE1);
	config_file_close(CONFIG_FILE_ID_CODE2);

	return 0;
}



/**
 * Stage 3 : Check for grammer errors.
 * Check elements/module references.
 * Check module's self containing.
 * Schedule routing.
 * Code generation.
 */
static int
stage_3(void)
{
	FILE * file;

	SIM_MODULE_LIST * module_list = NULL;
	SIM_MODULE_TREE * entity_tree = NULL;
	SIM_LINK_LIST * link_list = NULL;
	//SIM_BUS_LIST * bus_list = NULL;

	module_list = parse_get_module_list();
	if(common_get_error_count()){
		return -1;
	}

	entity_tree = module_do(module_list);
	if(common_get_error_count()){
		return -1;
	}

	link_list = global_do(entity_tree);
	if(common_get_error_count()){
		return -1;
	}

	//bus_list = bus_do(entity_tree);
	//if(common_get_error_count()){
	//	return -1;
	//}



	file = config_file_open(CONFIG_FILE_ID_CODE2, "r");
	if (file == NULL) {
		common_error("Open CODE2 file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_CODE2));
	}

	file = config_file_open(CONFIG_FILE_ID_SIM_C, "w");
	if (file == NULL) {
		common_error("Open C file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_SIM_C));
	}

	file = config_file_open(CONFIG_FILE_ID_SIM_H, "w");
	if (file == NULL) {
		common_error("Open H file \"%s\" failed.", config_get_file_name(CONFIG_FILE_ID_SIM_H));
	}



	code_do(module_list, entity_tree, link_list);



	config_file_close(CONFIG_FILE_ID_CODE2);
	config_file_close(CONFIG_FILE_ID_SIM_C);
	config_file_close(CONFIG_FILE_ID_SIM_H);
	/*
	if((result == 0) && config_get_graph()){
		graph_dot();
	}

	if(result == 0){
		code_compile();
	}
	*/

	if(config_get_compile()){
		code_compile();
	}

	return 0;
}



static void
stage_print(const char *format, ...)
{
	va_list arg;
	if (config_get_verbose() == 0) {
		return;
	}
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}

