#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "error.h"
#include "line.h"

typedef struct __include_file_id {
	char * file;
	int fid;
	struct __include_file_id * next;
} INCLUDE_FILE_ID;

typedef struct __line_number {
	int fid;
	int line_parse;
	int line_file;
	struct __line_number * next;
} LINE_NUMBER;

static INCLUDE_FILE_ID * _include_file_fid = NULL;

static LINE_NUMBER * _line_number_root = NULL;

static LINE_NUMBER * _line_number_tail = NULL;

static LINE * line_number = NULL;

static int current_line_number = 1;

void
line_initial(void)
{
	_include_file_fid = NULL;
	_line_number_root = NULL;
	_line_number_tail = NULL;
	line_number = NULL;
	current_line_number = 1;
}

void
line_finish(void)
{

	LINE_NUMBER * temp;
	LINE_NUMBER * temp0;
	INCLUDE_FILE_ID * include;

	line_number = (LINE *)malloc(sizeof(LINE) * (current_line_number + 2));

	bzero(line_number, sizeof(LINE) * (current_line_number + 2));

	temp = _line_number_root;

	line_number[0].file = "EOF";

	while(temp){
		line_number[temp->line_parse].line = temp->line_file;
		include = _include_file_fid;

		while(include){
			if(include->fid == temp->fid){
				line_number[temp->line_parse].file = include->file;
				break;
			}
			include = include->next;
		}
		temp0 = temp;
		temp = temp->next;
		free(temp0);
	}
}

/* tell <line> that a file is included. */
int
line_register_file(char * filename)
{
	static int last_fid = 1;
	INCLUDE_FILE_ID * temp;

	//printf("file %s (start[%d]) reg\n", filename, current_line_number);

	/* check include more than one time. */
	temp = _include_file_fid;
	while(temp){
		if(strcmp(filename, temp->file) == 0){
			common_warning("file [%s] has already been included", filename);
			return 0;
		}
		temp = temp->next;
	}

	/* record this file in a linked-list. */
	temp = (INCLUDE_FILE_ID *)malloc(sizeof(INCLUDE_FILE_ID));
	temp->file = strdup(filename);
	last_fid ++;
	temp->fid = last_fid;

	temp->next = _include_file_fid;
	_include_file_fid = temp;

	/* use this id to record lines later. */
	return last_fid;
}


void
line_register_line(int fid, int line_file)
{
	LINE_NUMBER * temp;

	//printf("line %d %d %d\n", fid, line_file, current_line_number);

	temp = (LINE_NUMBER *)malloc(sizeof(LINE_NUMBER));
	temp->fid = fid;
	temp->line_file = line_file;
	temp->line_parse = current_line_number;

	if(_line_number_root == NULL){
		_line_number_root = temp;
		_line_number_tail = temp;
	}else{
		_line_number_tail->next = temp;
		_line_number_tail = temp;
	}
	current_line_number++;
}



LINE *
line_revert(int line)
{
	if(line >= current_line_number){
		return &line_number[0];
	}else{
		return &line_number[line];
	}
}
