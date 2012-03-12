/**
 * In these data types, option and value make the value of the variable.
 * option == 0, value has its binary value.
 * option == 1 means 'x'.
 * option == 2 means 'z'.
 */

#define SIM_OPTION_VALID 0
#define SIM_OPTION_X     1
#define SIM_OPTION_Z     2

typedef struct __sim_type_u32 {
	int option;
	unsigned int value;
} SIM_TYPE__U32;

typedef struct __sim_type_u16 {
	int option;
	unsigned short value;
} SIM_TYPE__U16;

typedef struct __sim_type_u08 {
	int option;
	unsigned char value;
} SIM_TYPE__U08;

typedef struct __sim_type_i32 {
	int option;
	int value;
} SIM_TYPE__I32;

typedef struct __sim_type_i16 {
	int option;
	short value;
} SIM_TYPE__I16;

typedef struct __sim_type_i08 {
	int option;
	char value;
} SIM_TYPE__I08;

typedef struct __sim_type_f32 {
	int option;
	float value;
} SIM_TYPE__F32;

typedef struct __sim_type_f64 {
	int option;
	double value;
} SIM_TYPE__F64;

typedef struct __sim_type_wire {
	int option;
	int value;
} SIM_TYPE__W01;


typedef enum __sim_type_e {
	ST_U32,
	ST_U16,
	ST_U08,
	ST_I32,
	ST_I16,
	ST_I08,
	ST_F32,
	ST_F64,
	ST_W01,
	ST_UNKNOWN
} SIM_TYPE_E;

