#define CONFIG_FILE_ID_INPUT     0
#define CONFIG_FILE_ID_SIM_H     1
#define CONFIG_FILE_ID_SIM_C     2
#define CONFIG_FILE_ID_ARCH      3
#define CONFIG_FILE_ID_CODE1     4
#define CONFIG_FILE_ID_CODE2     5
#define CONFIG_FILE_ID_DOT       6
#define CONFIG_FILE_ID_VERILOG   7
#define CONFIG_FILE_ID_VCD       8
#define CONFIG_FILE_ID_OUT       9

#define CONFIG_FILE_ID_COUNT     10


int config_initial(int argc, char * argv[]);
void config_set_run_cycle(int cycle);
void config_set_dot(int value);
void config_set_verilog(int value);
void config_set_preprocess_only(int value);
void config_set_verbose(int value);
void config_set_keep_arch(int value);
void config_set_keep_code1(int value);
void config_set_keep_code2(int value);
void config_set_initial_value(int value);
void config_set_trace(int value);
void config_set_compile(int value);
void config_set_top_module(char * name);
int config_get_run_cycle(void);
int config_get_dot(void);
int config_get_verilog(void);
int config_get_preprocess_only(void);
int config_get_verbose(void);
int config_get_keep_arch(void);
int config_get_keep_code1(void);
int config_get_keep_code2(void);
int config_get_initial_value(void);
int config_get_trace(void);
int config_get_compile(void);
char * config_get_top_module(void);

/* file */
void config_set_file_input(int fid, char * name);
FILE * config_file_open(int fid, char * mode);
void config_file_close(int fid);
FILE * config_get_file(int fid);
char * config_get_file_name(int fid);
