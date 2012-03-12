//#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "common.h"
#include "util.h"


#define CONFIG_FILE_SUFFIX_ARCH ".arch"
#define CONFIG_FILE_SUFFIX_CODE1 ".code1"
#define CONFIG_FILE_SUFFIX_CODE2 ".code2"
#define CONFIG_FILE_SUFFIX_SIM_H ".sim.h"
#define CONFIG_FILE_SUFFIX_SIM_C ".sim.c"
#define CONFIG_FILE_SUFFIX_DOT ".dot"
#define CONFIG_FILE_SUFFIX_VERILOG ".v"
#define CONFIG_FILE_SUFFIX_VCD ".vcd"
#define CONFIG_FILE_SUFFIX_OUT ".out"


typedef struct __CONFIG_FILE {
	char * name;
	FILE * file;
} CONFIG_FILE;

typedef struct __CONFIG {
	int run_cycle;
	int graph;
	int verilog;
	int preprocess_only;
	int verbose;
	int keep_arch;
	int keep_code1;
	int keep_code2;
	int initial_value;         //char value
	int trace;
	int compile;
	char * top_module;
	CONFIG_FILE file[CONFIG_FILE_ID_COUNT];
} CONFIG;

static CONFIG config;
static void config_help(void);


int config_initial(int argc, char * argv[])
{
	int opt;
	int i;
	char * fi;
	char * temp;

	bzero(&config, sizeof(CONFIG));
	config.run_cycle = 10;

	if(argc <= 1){
		config_help();
		exit(0);
	}

	// in cygwin-libc-getopt(), options must be in front of other arguments
	while ((opt = getopt(argc, argv, "hvg:pm:k:r:i:tc")) != -1) {
		switch (opt) {
		case 'h': /* print help message */
			config_help();
			exit(0);
			break;

		case 'v': /* verbose mode */
			config.verbose = 1;
			break;

		case 'g': /* generate '.dot','.v' file */
			i = 0;
			while(optarg[i]){
				if(optarg[i] == 'g'){
					config.graph = 1;
				}else if(optarg[i] == 'v'){
					config.verilog = 1;
				}
				i++;
			}
			break;

		case 'p': /* preprocess_only */
			config.preprocess_only = 1;
			break;

		case 'm': /* Top module. */
			config.top_module = strdup(optarg);
			break;

		case 'k': /* keep which temp files? */
			i = 0;
			while(optarg[i]){
				if(optarg[i] == 'a'){
					config.keep_arch = 1;
				}else if(optarg[i] == '1'){
					config.keep_code1 = 1;
				}else if(optarg[i] == '2'){
					config.keep_code2 = 1;
				}
				i++;
			}
			break;

		case 'r': /* run how many cycles */
			if(sscanf(optarg, "%d", &i) > 0){
				config.run_cycle = i;
			}
			break;

		case 'i': /* initial value of all data */
			switch(optarg[0]){
			case '0':
				config.initial_value = '0';
				break;
			case 'x':
				config.initial_value = 'x';
				break;
//			case 'z':
//				config.initial_value = 'z';
//				break;
			default:
				config.initial_value = 'x';
				break;
			}
			break;

		case 't':
			config.trace = 1;
			break;
		case 'c':
			config.compile = 1;
			break;

		case '?': /* unknown option */
			config_help();
			exit(0);
			break;
		}
	}

	if (optind >= argc) {
		return -1;
	}

	fi = strdup(argv[optind]);
	config.file[CONFIG_FILE_ID_INPUT].name = strdup(fi);

	temp = strrchr(fi, '.');
	if(temp && strcmp(temp, ".adl")){
		*temp = '\0';
	}
	config.file[CONFIG_FILE_ID_ARCH].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_ARCH, 0);
	config.file[CONFIG_FILE_ID_CODE1].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_CODE1, 0);
	config.file[CONFIG_FILE_ID_CODE2].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_CODE2, 0);

	if(config.file[CONFIG_FILE_ID_SIM_H].name == NULL){
		config.file[CONFIG_FILE_ID_SIM_H].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_SIM_H, 0);
	}

	if(config.file[CONFIG_FILE_ID_SIM_C].name == NULL){
		config.file[CONFIG_FILE_ID_SIM_C].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_SIM_C, 0);
	}

	config.file[CONFIG_FILE_ID_DOT].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_DOT, 0);
	config.file[CONFIG_FILE_ID_VERILOG].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_VERILOG, 0);
	config.file[CONFIG_FILE_ID_VCD].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_VCD, 0);
	config.file[CONFIG_FILE_ID_OUT].name = util_join(fi, 0, CONFIG_FILE_SUFFIX_OUT, 0);

	if(config.top_module == NULL){
		config.top_module = "top";
	}

	return 0;

}


/* setter */

void config_set_run_cycle(int cycle) { if(cycle >0){ config.run_cycle = cycle; } }

void config_set_dot(int value) { config.graph = value; }

void config_set_verilog(int value) { config.verilog = value; }

void config_set_preprocess_only(int value) { config.preprocess_only = value; }

void config_set_verbose(int value) { config.verbose = value; }

void config_set_keep_arch(int value) { config.keep_arch = value; }

void config_set_keep_code1(int value) { config.keep_code1 = value; }

void config_set_keep_code2(int value) { config.keep_code2 = value; }

void config_set_initial_value(int value) { config.initial_value = value; }

void config_set_trace(int value) { config.trace = value; }

void config_set_compile(int value) { config.compile = value; }

void config_set_top_module(char * name) { config.top_module = strdup(name); }


/* getter */
int config_get_run_cycle(void) { return config.run_cycle; }

int config_get_dot(void) { return config.graph; }

int config_get_verilog(void) { return config.verilog; }

int config_get_preprocess_only(void) { return config.preprocess_only; }

int config_get_verbose(void) { return config.verbose; }

int config_get_keep_arch(void) { return config.keep_arch; }

int config_get_keep_code1(void) { return config.keep_code1; }

int config_get_keep_code2(void) { return config.keep_code2; }

int config_get_initial_value(void) { return config.initial_value; }

int config_get_trace(void) { return config.trace; }

int config_get_compile(void) { return config.compile; }

char * config_get_top_module(void) { return config.top_module; }



/* files */
void
config_set_file_input(int fid, char * name)
{
	if(fid < 0 || fid >= CONFIG_FILE_ID_COUNT){
		return;
	}

	config.file[fid].name = strdup(name);
}

FILE *
config_file_open(int fid, char * mode)
{
	FILE * file;

	if(fid < 0 || fid >= CONFIG_FILE_ID_COUNT){
		return NULL;
	}

	file = fopen(config.file[fid].name, mode);
	if(file == NULL){
		return NULL;
	}

	config.file[fid].file = file;
	return file;
}

void
config_file_close(int fid)
{
	FILE * file;

	if(fid < 0 || fid >= CONFIG_FILE_ID_COUNT){
		return;
	}

	file = config.file[fid].file;
	if(file){
		fclose(file);
	}

	config.file[fid].file = NULL;
}

FILE *
config_get_file(int fid)
{
	if(fid < 0 || fid >= CONFIG_FILE_ID_COUNT){
		return NULL;
	}
	
	return config.file[fid].file;
}

char *
config_get_file_name(int fid)
{
	if(fid < 0 || fid >= CONFIG_FILE_ID_COUNT){
		return NULL;
	}

	return config.file[fid].name;
}


static void config_help(void)
{
	puts("ADL for Simulation v " ADLFORSIM_VERSION "\n @ " __DATE__ "  " __TIME__);
}
