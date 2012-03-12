#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "util.h"
#include "config.h"
#include "line.h"
#include "pre.h"

static int
pre_recursive_process(FILE * input, char * current_file);

// convert input file into .arch and .code1.
int
pre_process(void)
{
	int result;
	FILE * input;
	char * input_name;

	input = config_get_file(CONFIG_FILE_ID_INPUT);
	input_name = config_get_file_name(CONFIG_FILE_ID_INPUT);
	result = pre_recursive_process(input, input_name);
	line_finish();

	return result;
}

static int
pre_recursive_process(FILE * input, char * current_file)
{
	/* TODO: self-include. */
	FILE *include;
	FILE *file_arch;
	FILE *file_code1;

	char buffer[1024];
	char *temp;
	char *include_name;
	int flag;//0:adl, 1:ccode
	int result;
	int fid;
	int line;

	file_arch = config_get_file(CONFIG_FILE_ID_ARCH);
	if(file_arch == NULL){
		return -1;
	}

	file_code1 = config_get_file(CONFIG_FILE_ID_CODE1);
	if(file_code1 == NULL){
		return -1;
	}

	line = 1;
	flag = 0;
	// split into two parts.

	fid = line_register_file(current_file);
	if(fid == 0){
		return 0;
	}

	while (fgets(buffer, 1023, input)) {

		if(flag == 0){
			temp = strchr(buffer, '#');
			if(temp){
				temp[0] = '\n';
				temp[1] = '\0';
			}
		}

		temp = util_trim_front(buffer);


		if (flag == 0 && temp[0] == '@') {	// include
			include_name = util_trim(temp + 1);
			include = fopen(include_name, "r");
			if (include == NULL) {
				common_error("Cannot open include file [%s].\n", temp);
				result = -1;
			}else{
				result = pre_recursive_process(include, include_name);
				fclose(include);
				include = NULL;
			}
			if (result) {
				return result;
			} else {
				line ++;
				continue;
			}
		} else if (strncmp(temp, "%%", 2) == 0) {
			if (flag) {
				fputs(buffer, file_code1);
				fputs("\n", file_arch);
			} else {
				fputs(buffer, file_arch);
			}
			flag = (~flag);
		} else {
			if (flag) {
				fputs(buffer, file_code1);
				fputs("\n", file_arch);
			} else {
				fputs(buffer, file_arch);
			}
		}
		line_register_line(fid, line);
		line ++;

	}
	return 0;
}
