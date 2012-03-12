

#define VCD_COMMOND_COMMENT             "$comment "
#define VCD_COMMAND_DATE                "$date "
#define VCD_COMMAND_ENDDEF              "$enddefinitions "
#define VCD_COMMAND_SCOPE               "$scope "
#define VCD_COMMAND_TIMESCALE           "$timescale "
#define VCD_COMMAND_UPSCOPE             "$upscope "
#define VCD_COMMAND_VAR                 "$var "
#define VCD_COMMAND_VERSION             "$version "

#define VCD_COMMAND_DUMPALL             "$dumpall "
#define VCD_COMMAND_DUMPOFF             "$dumpoff "
#define VCD_COMMAND_DUMPON              "$dumpon "
#define VCD_COMMAND_DUMPVARS            "$dumpvars "

#define VCD_COMMAND_END                 " $end\n"




typedef struct __vcd_var_id VCD_VAR_ID;

typedef struct __vcd_var_id_list VCD_VAR_ID_LIST;

typedef struct __vcd_dump VCD_DUMP;

struct __vcd_var_id {
	char * var;
	char * show;
	int id;
	int width;
	SIM_TYPE_E sim_type;
	char * type;
	void * pdata;
	void * pdata_1;
};

struct __vcd_var_id_list {
	VCD_VAR_ID_LIST * next;
	VCD_VAR_ID * vi;
};


struct __vcd_dump {
	FILE * output;
	VCD_VAR_ID_LIST * var_id_list;
	int current_id;
	int cycle;
};







int
vcd_initial(char * src_file_name);

int
vcd_symbol_add(char * name, char * show, SIM_TYPE_E type, void * pdata, int width_w01);

int
vcd_symbol_alias(char * new_name, char * alias_name, void * pdata);

void
vcd_scope(char * entity_name);

void
vcd_upscope(void);

void
vcd_enddef(void);

int
vcd_var(char * name);

int
vcd_dump_var(char * name);

void
vcd_dump_one_cycle(void);








