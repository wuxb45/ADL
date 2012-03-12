void parse_module_start(char *name);
void parse_module_finish(void);
void parse_cycle(int value);

void parse_input_start(char *data_type_string);
//void parse_input_finish(void);

void parse_output_start(char *data_type_string);
//void parse_output_finish(void);

void parse_inout_start(char *data_type_string);
//void parse_inout_finish(void);

void parse_ram_start(char *data_type_string);
//void parse_ram_finish(void);

void parse_contain_start(char *data_type_string);
//void parse_contain_finish(void);

void parse_element_finish(void);

void parse_code(void);

void parse_dimension(int value);
void parse_name(char *name);
void parse_way(int edge1, int edge2);

void parse_net_start(void);
void parse_net_finish(void);
void parse_net_element(char * name);
void parse_net_element_sub(char * module_name, char * name);
void parse_net_element_value(char * value_string);

SIM_MODULE_LIST *
parse_get_module_list(void);
