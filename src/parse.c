/**
 * by wuxb
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdarg.h>


#include "util.h"
#include "common.h"
#include "config.h"

#include "parse.h"

static SIM_ELEMENT_TYPE _element_type;
static int _width_count = 0;
static int _width[256];
static int _way_count = 0;
static int _way_low;
static int _way_high;

static SIM_DATA_TYPE _data_type;
static char * _module_type;

static SIM_NET_ELEMENT_LIST * _net_element_list;

static SIM_MODULE_LIST *_module_list = NULL;

/**
 * Pointer to current processing module.
 */
static SIM_MODULE *_current_module = NULL;


static void
parse_print(const char *format, ...);

static void
parse_normal_element_start(SIM_ELEMENT_TYPE type, char * data_type_string);

/**
 *
 */
void
parse_module_start(char *name)
{
	parse_print(" // Parse Module Start : %s\n", name);

	/* check name is distinct. */
	if (common_module_list_find_name(_module_list, name) != NULL) {
		common_error("Module <%s> has been defined.", name);
		return;
	}

	/* create module entity. */
	_current_module = common_module_new(strdup(name), 0, 0, NULL, NULL);

	/* append to the end of the module_list. */
	common_module_list_append(&_module_list, _current_module);
	return;
}

/**
 * end of a module.
 */
void
parse_module_finish(void)
{

	parse_print(" \\\\ finish %s\n\n", _current_module->name);
	_current_module = NULL;
	return;
}

void
parse_cycle(int value)
{

	parse_print("++ cycle [%d]\n", value);
	_current_module->cycle = value;
	return;
}

void
parse_input_start(char *type_string)
{
	parse_print(" ++ input [%s]", type_string);
	parse_normal_element_start(ELEMENT_INPUT, type_string);
	return;
}

void
parse_output_start(char *type_string)
{
	parse_print(" ++ output [%s]", type_string);
	parse_normal_element_start(ELEMENT_OUTPUT, type_string);

	return;
}

void
parse_inout_start(char *type_string)
{
	parse_print(" ++ inout [%s]", type_string);
	parse_normal_element_start(ELEMENT_INOUT, type_string);

	return;
}

void
parse_ram_start(char *type_string)
{
	parse_print(" ++ ram [%s]", type_string);
	parse_normal_element_start(ELEMENT_RAM, type_string);

	return;
}

static void
parse_normal_element_start(SIM_ELEMENT_TYPE element_type, char * data_type_string)
{
	SIM_DATA_TYPE type;

	_element_type = element_type;

	type = common_string_to_data_type(data_type_string);
	if(type == SDT_UNKNOWN){
		common_error("Unknown type [%s]. @ %s", data_type_string, _current_module->name);
		return;
	}

	_data_type = type;

	return;
}

void
parse_contain_start(char *type_string)
{
	_element_type = ELEMENT_CONTAIN;

	parse_print("++ contain : module_type : %s", type_string);

	_module_type = strdup(type_string);

	return;
}

void
parse_element_finish(void)
{
	_element_type = ELEMENT_UNKNOWN;

	_width_count = 0;

	_data_type = SDT_UNKNOWN;

	if(_module_type){
		free(_module_type);
		_module_type = NULL;
	}
	parse_print(";\n");
}


void
parse_code(void)
{
	static char buffer[1024];
	FILE * file_code1;
	FILE * file_code2;
	char * temp;

	file_code1 = config_get_file(CONFIG_FILE_ID_CODE1);
	if(file_code1 == NULL){
		common_error("Open code1 failed!");
	}
	file_code2 = config_get_file(CONFIG_FILE_ID_CODE2);
	if(file_code2 == NULL){
		common_error("Open code2 failed!");
	}

	parse_print("++ code : ....\n");

	if(_current_module == NULL){ // head code for includes and declares
		fprintf(file_code2, "%%%%%%%%\n");
	}else{ // mark the name of module
		_current_module->behavior = 1;
		fprintf(file_code2, "%%%%%s\n", _current_module->name);
	}

	// join the code lines from code_1 temp file.
	while (fgets(buffer, 1023, file_code1)) {
		temp = util_trim_front(buffer);
		if(strncmp(temp, "%%", 2) == 0){
			break;
		}
		fputs(buffer, file_code2);
	}

	fputs(buffer, file_code2);
	return;
}

void
parse_dimension(int value)
{
	parse_print(" ++ dim[%d]", value);
	if(_width_count >=63){
		common_error("kill me !");
	}

	_width[_width_count] = value;
	_width_count++;
}

void
parse_name(char * name)
{
	SIM_ELEMENT * element;
	SIM_DIMENSION * dimension;
	int * width;
	parse_print(" <\"%s\">", name);

	element = common_element_list_find_name(_current_module->element_list, name);
	if(element){
		common_error("Name [%s] has been defined. @ %s", name, _current_module->name);
		return;
	}


	width = (int *)malloc(sizeof(int) * _width_count);
	memcpy(width, _width, sizeof(int) * _width_count);
	dimension = common_dimension_new(_width_count, width, _way_count, _way_low, _way_high);
	switch(_element_type){
	case ELEMENT_INPUT:
	case ELEMENT_OUTPUT:
	case ELEMENT_INOUT:
	case ELEMENT_RAM:

		element = common_element_new(_element_type, _current_module, _data_type,
				NULL, strdup(name), dimension);
		break;
	case ELEMENT_CONTAIN:
		element = common_element_new(_element_type, _current_module, SDT_UNKNOWN,
				strdup(_module_type), strdup(name), dimension);

		break;
	default:
		common_error("what??");
		return;
		break;
	}

	common_element_list_append(&(_current_module->element_list), element);
}


void
parse_way(int edge1, int edge2)
{
	if(edge1 > edge2){
		_way_high = edge1;
		_way_low = edge2;_way_count++;
	}else if(edge1 < edge2){
		_way_high = edge2;
		_way_low = edge1;_way_count++;
	}else{
		parse_dimension(edge1);
	}
}


void
parse_net_start(void)
{
	parse_print("net:");
}


void
parse_net_finish(void)
{
	parse_print(";\n");
	common_net_list_append_element_list(&(_current_module->net_list), _net_element_list);
	_net_element_list = NULL;
}

void
parse_net_element(char * name)
{

	SIM_NET_ELEMENT * net_element;

	int * width;
	SIM_DIMENSION * dimension;
	parse_print("<\"%s\">", name);

	width = (int *)malloc(sizeof(int) * _width_count);
	memcpy(width, _width, sizeof(int) * _width_count);
	dimension = common_dimension_new(_width_count, width, _way_count, _way_low, _way_high);

	net_element = common_net_element_new(NET_ELEMENT_PORT, _current_module, NULL,
			NULL, strdup(name), dimension);
	_width_count = 0;
	_way_count = 0;
	_way_low = 0;
	_way_high = 0;

	common_net_element_list_append(&(_net_element_list), net_element);

}

void
parse_net_element_sub(char * module_name, char * name)
{

	SIM_NET_ELEMENT * net_element;
	int * width;
	SIM_DIMENSION * dimension;

	parse_print("<\"%s\":\"%s\">", module_name, name);

	width = (int *)malloc(sizeof(int) * _width_count);
	memcpy(width, _width, sizeof(int) * _width_count);
	dimension = common_dimension_new(_width_count, width, _way_count, _way_low, _way_high);

	net_element = common_net_element_new(NET_ELEMENT_PORT, _current_module, NULL,
			strdup(module_name), strdup(name), dimension);
	_width_count = 0;
	_way_count = 0;
	_way_low = 0;
	_way_high = 0;

	common_net_element_list_append(&(_net_element_list), net_element);
}


void
parse_net_element_value(char * value_string)
{
	parse_print("<%s>", value_string);
	SIM_NET_ELEMENT * net_element;
	net_element = common_net_element_new(NET_ELEMENT_VALUE, _current_module,
			NULL, NULL, value_string, NULL);

	common_net_element_list_append(&(_net_element_list), net_element);
}


SIM_MODULE_LIST *
parse_get_module_list(void)
{
	return _module_list;
}


static void
parse_print(const char *format, ...)
{
	va_list arg;
	return;
	if (config_get_verbose() == 0) {
		return;
	}
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
	return;
}
