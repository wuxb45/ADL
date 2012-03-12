typedef enum __data_type_enum {
	U32,
	U16,
	U08,
	I32,
	I16,
	I08,
	F32,
	F64,
	W01
} DATA_TYPE_E;
typedef struct __debug_register {
	struct __debug_register * next;
	char * name;
	DATA_TYPE_E type;
	void * data;
}DEBUG_REGISTER;


void debug_register_variable(char * name, DATA_TYPE_E type, void * data);

//void debug_find_variable(char * name);

void debug_print_all(void);
void debug_print_variable(DEBUG_REGISTER * var);

