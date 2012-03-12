#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "common.h"
#include "util.h"
#include "verilog.h"
#include "code.h"

extern FILE *file_verilog;

static SIM_MODULE * __module_list;

static void
verilog_convert_module(SIM_MODULE * module);

static void
verilog_convert_port(SIM_LIST * port, char * prefix, char * prefix2);

static void
verilog_convert_structual(SIM_MODULE * module);

static void
verilog_write_to_v(const char *format, ...);

void
verilog_generate(SIM_MODULE * module_list)
{
	SIM_MODULE * temp;
	__module_list = module_list;
	temp = module_list;
	while(temp){
		verilog_convert_module(temp);
		temp = temp->next;
	}
}


static void
verilog_convert_module(SIM_MODULE * module)
{
	SIM_LIST * temp;


	// head, port definition
	verilog_write_to_v("module %s(\n", module->name);

	temp = module->input;
	while(temp){
		verilog_write_to_v("\t\t%s,\n", temp->name1);
		temp = temp->next;
	}

	temp = module->output;
	while(temp){
		verilog_write_to_v("\t\t%s,\n", temp->name1);
		temp = temp->next;
	}

	temp = module->inout;
	while(temp){
		verilog_write_to_v("\t\t%s,\n", temp->name1);
		temp = temp->next;
	}

	verilog_write_to_v("\t\tclock\n");
	verilog_write_to_v(");\n\n");

	// port declaration
	temp = module->input;
	while(temp){
		verilog_convert_port(temp, "input", NULL);
		temp = temp->next;
	}

	temp = module->output;
	while(temp){
		verilog_convert_port(temp, "output", NULL);
		temp = temp->next;
	}

	temp = module->inout;
	while(temp){
		verilog_convert_port(temp, "inout", NULL);
		temp = temp->next;
	}
	verilog_write_to_v("\tinput clock;\n\n");

	if(module->behavior == 0){
		verilog_convert_structual(module);
	}


	verilog_write_to_v("endmodule\n\n");
}

static void
verilog_convert_port(SIM_LIST * port, char * prefix, char * prefix2)
{
	int width;

	verilog_write_to_v("\t%s ", prefix);


	width = common_data_type_to_width(port->name2);
	if(width > 1){
		verilog_write_to_v("[%d:0] ", (width-1));
	}
	if(prefix2){
		verilog_write_to_v("%s", prefix2);
	}
	verilog_write_to_v("%s;\n", port->name1);
}

static void
verilog_convert_structual(SIM_MODULE * module)
{
	SIM_LIST * temp;
	SIM_LIST * temp2;
	SIM_LIST * link;
	SIM_MODULE * sub;
	char * prefix_dot;
	char * prefix_under;
	char * full_name_dot;
	char * full_name_under;
	char * full_name_2;

	// all internal wire
	temp = module->contain;
	while(temp){
		sub = common_module_list_find(__module_list, temp->name2);
		if(sub == NULL){
			return;
		}

		prefix_under = util_join(temp->name1, 0, "_", 0);

		temp2 = sub->output;
		while(temp2){
			verilog_convert_port(temp2, "wire", prefix_under);
			temp2 = temp2->next;
		}
		free(prefix_under);

		// TODO : how to convert inout?
		temp = temp->next;
	}

	// sub module
	temp = module->contain;
	while(temp){
		sub = common_module_list_find(__module_list, temp->name2);
		if(sub == NULL){
			return;
		}

		verilog_write_to_v("\t%s %s (\n", temp->name2, temp->name1);

		prefix_dot = util_join(temp->name1, 0, ".", 0);

		temp2 = sub->input;
		while(temp2){
			verilog_write_to_v("\t\t.%s", temp2->name1);

			full_name_dot = util_join(prefix_dot, 0, temp2->name1, 0);
			link = common_list_find_name1(module->link, full_name_dot);
			free(full_name_dot);
			if(link){
				full_name_2 = code_convert_entity_name(link->name2);
			}else{
				full_name_2 = NULL;
			}

			if(full_name_2){
				verilog_write_to_v("(%s),\n", full_name_2);
				free(full_name_2);
			}else{
				verilog_write_to_v("(),\n");
			}


			temp2 = temp2->next;
		}
		free(prefix_dot);

		prefix_under = util_join(temp->name1, 0, "_", 0);

		temp2 = sub->output;
		while(temp2){
			verilog_write_to_v("\t\t.%s", temp2->name1);

			full_name_under = util_join(prefix_under, 0, temp2->name1, 0);
			verilog_write_to_v("(%s),\n", full_name_under);
			free(full_name_under);
			temp2 = temp2->next;
		}
		free(prefix_under);

		verilog_write_to_v("\t\t.clock(clock)\n");
		verilog_write_to_v("\t);\n\n");
		temp = temp->next;
	}
}

static void
verilog_write_to_v(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	vfprintf(file_verilog, format, arg);
	va_end(arg);
	return;
}
