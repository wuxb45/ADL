#include <stdio.h>
#include <stdlib.h>
#include "stage.h"
#include "common.h"

static void
main_summary(void);

int
main(int argc, char *argv[])
{
	int result;

	result = stage_do(argc, argv);
	if(result > 0){
		exit(0);
	}else {
		main_summary();
		return result;
	}
}

static void
main_summary(void)
{
	int count = common_get_error_count();
	if(count){
		printf("Summary : %d ERROR(s).\n", count);
	} else {
		printf("Summary : Done!\n");
	}
}
