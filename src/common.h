/**
 * Common data types/functions that used for generate simulator.
 */

#define ADLFORSIM_VERSION "0.4"


typedef enum __sim_data_type {
	SDT_U32,
	SDT_U16,
	SDT_U08,
	SDT_I32,
	SDT_I16,
	SDT_I08,
	SDT_F32,
	SDT_F64,
	SDT_W01,
	SDT_UNKNOWN
} SIM_DATA_TYPE;

typedef enum __sim_element_type_enum {
		ELEMENT_INPUT,
		ELEMENT_OUTPUT,
		ELEMENT_INOUT,
		ELEMENT_RAM,
		ELEMENT_CONTAIN,
		ELEMENT_UNKNOWN
} SIM_ELEMENT_TYPE;

typedef enum __sim_net_element_type {
	NET_ELEMENT_WRITER,
	NET_ELEMENT_READER,
	NET_ELEMENT_INOUT,
	NET_ELEMENT_PORT,
//	NET_ELEMENT_INTEGER,
//	NET_ELEMENT_FP,
	NET_ELEMENT_VALUE,
	NET_ELEMENT_UNKNOWN
} SIM_NET_ELEMENT_TYPE;

typedef enum __sim_net_type {
	NET_TYPE_NORMAL,
	NET_TYPE_BUS
} SIM_NET_TYPE;

typedef enum __sim_link_type {
	LINK_RW,
	LINK_CONST
} SIM_LINK_TYPE;


typedef struct __sim_dimension SIM_DIMENSION;

typedef struct __sim_element SIM_ELEMENT;
typedef struct __sim_element_list SIM_ELEMENT_LIST;

typedef struct __sim_net_element SIM_NET_ELEMENT;
typedef struct __sim_net_element_list SIM_NET_ELEMENT_LIST;

typedef struct __sim_net SIM_NET;
typedef struct __sim_net_list SIM_NET_LIST;

typedef struct __sim_module SIM_MODULE;
typedef struct __sim_module_list SIM_MODULE_LIST;

typedef struct __sim_module_tree SIM_MODULE_TREE;

typedef struct __sim_link SIM_LINK;
typedef struct __sim_link_list SIM_LINK_LIST;

typedef struct __sim_bus SIM_BUS;
typedef struct __sim_bus_list SIM_BUS_LIST;



struct __sim_dimension {
	int count;		// count of dimensions
	int * width;	// an array of all dimensions' width
	int way;		// if a [x:y] way option is declared
	int way_low;	// the x in [x:y]
	int way_high;	// the y in [x:y]
};






struct __sim_element {
	SIM_ELEMENT_TYPE type;		// what kind of the element is, port(3)/ram/sub-moudle
	SIM_MODULE * father;		// element belongs to which kind of module

	SIM_DATA_TYPE data_type;	// if port/ram, has a data type
	char * module_type;			// if sub-module, has a module type name
	SIM_MODULE * module;		// pointer to the module, filled by <module>

	char * name;				// name of the element
	SIM_DIMENSION * dimension;	// dimension
	char ** split_ways;
};

struct __sim_element_list {
	SIM_ELEMENT_LIST * next;
	SIM_ELEMENT * element;
};






struct __sim_net_element {
	SIM_NET_ELEMENT_TYPE type;	// can be port/value, change to in/out/inout by <module>
	SIM_MODULE * father;		// the end's father module

	SIM_ELEMENT * sim_element;	// the referred element
	char * module_name;			// sub-module's name
	char * port_name;			// port name
	int integer;				// if a int value
	float fp;					// if a float value
	SIM_DIMENSION * way;		// has way declared
	SIM_DIMENSION * dimension;	// the dimension that the net_element contains, generated
	char ** split_ways;
};

struct __sim_net_element_list {
	SIM_NET_ELEMENT_LIST * next;
	SIM_NET_ELEMENT * net_element;
};





struct __sim_net {
	SIM_NET_ELEMENT_LIST * net_element_list;	// all net_elements

	SIM_NET_ELEMENT * writer;					// the only writer
	SIM_NET_ELEMENT_LIST * reader_list;			// at least 1 reader

	SIM_DATA_TYPE data_type;					// a net's data_type, all the same
	SIM_DIMENSION * dimension;					// same

	SIM_NET_TYPE type;							// w/r net or bus net
};

struct __sim_net_list {
	SIM_NET_LIST * next;
	SIM_NET * net;
};





struct __sim_module {
	char *name;
	int cycle;
	int behavior;						// 1:behavior, 0:struct
	SIM_ELEMENT_LIST * element_list;
	SIM_NET_LIST * net_list;
	int entity_count;
};

struct __sim_module_list {
	SIM_MODULE_LIST *next;
	SIM_MODULE * module;
};

struct __sim_module_tree {
	SIM_MODULE_TREE *brother;
	SIM_MODULE_TREE *child;
	SIM_MODULE_TREE *father;
	char *name;
	int id;						// -1 if a struct module, 0 top moudle, >0 behavior module
	int sortid;				// sort index
	SIM_MODULE *module;			// refers to a module
};



struct __sim_link {
	SIM_LINK_TYPE type;

	char * src_full_name;
	SIM_MODULE_TREE * src_module_node;
	char * src_module_name;
	SIM_MODULE * src_module_type;
	SIM_ELEMENT * src_port_type;
	char * src_port_way;

	char * dest_full_name;
	SIM_MODULE_TREE * dest_module_node;
	char * dest_module_name;
	SIM_MODULE * dest_module_type;
	SIM_ELEMENT * dest_port_type;
	char * dest_port_way;

};

struct __sim_link_list {
	SIM_LINK_LIST * next;
	SIM_LINK * link;
};


struct __sim_bus {
	int in;
	SIM_NET_ELEMENT_LIST * net_element_list;
};

struct __sim_bus_list {
	SIM_BUS_LIST * next;
	SIM_BUS * bus;
};


int
common_get_error_count(void);

void
common_error(const char *format, ...);

void
common_error_line(int parse_line, const char *format, ...);

void
common_warning(const char *format, ...);

SIM_DATA_TYPE
common_string_to_data_type(char *type_string);

char *
common_data_type_to_string(SIM_DATA_TYPE type);

SIM_DIMENSION *
common_dimension_new(int count, int * width, int way, int way_low, int way_high);

void
common_dimension_free(SIM_DIMENSION * dimension);

void
common_dimension_print(SIM_DIMENSION * dimension);

char *
common_dimension_array_to_string(int count, int * array);

char **
common_dimension_split(SIM_DIMENSION * way, SIM_DIMENSION * dimension);

char *
common_dimension_to_string(SIM_DIMENSION * dimension);

SIM_ELEMENT *
common_element_new(SIM_ELEMENT_TYPE type, SIM_MODULE * father, SIM_DATA_TYPE data_type,
		char * module_type, char * name, SIM_DIMENSION * dimension);

void
common_element_free(SIM_ELEMENT * element);

SIM_ELEMENT *
common_element_list_find_name(SIM_ELEMENT_LIST * element_list, char *name);

void
common_element_print(SIM_ELEMENT * element);

void
common_element_error(SIM_ELEMENT * element);

void
common_element_list_append(SIM_ELEMENT_LIST ** phead, SIM_ELEMENT * element);

void
common_element_list_remove(SIM_ELEMENT_LIST ** phead, SIM_ELEMENT * node);

void
common_element_list_print(SIM_ELEMENT_LIST * head);

SIM_NET_ELEMENT *
common_net_element_new(SIM_NET_ELEMENT_TYPE type, SIM_MODULE * father,
		SIM_ELEMENT * sim_element, char * module_name, char * port_name, SIM_DIMENSION * way);

void
common_net_element_free(SIM_NET_ELEMENT * net_element);

void
common_net_element_print(SIM_NET_ELEMENT * net_element);

void
common_net_element_error(SIM_NET_ELEMENT * net_element);

void
common_net_element_list_append(SIM_NET_ELEMENT_LIST ** phead, SIM_NET_ELEMENT * element);

void
common_net_element_list_print(SIM_NET_ELEMENT_LIST * net_element_list);

void
common_net_element_list_error(SIM_NET_ELEMENT_LIST * net_element_list);

void
common_net_list_append_element_list(SIM_NET_LIST ** phead, SIM_NET_ELEMENT_LIST * element_list);

void
common_net_list_print(SIM_NET_LIST * net_list);

void
common_net_print(SIM_NET * net);

SIM_MODULE *
common_module_new(char * name, int cycle, int behavior,
		SIM_ELEMENT_LIST * element_list, SIM_NET_LIST * net_list);

void
common_module_free(SIM_MODULE * module);

void
common_module_print(SIM_MODULE * module);

void
common_module_error(SIM_MODULE * module);

SIM_MODULE *
common_module_list_find_name(SIM_MODULE_LIST * module_list, char *name);

void
common_module_list_append(SIM_MODULE_LIST ** phead, SIM_MODULE * module);

void
common_module_list_print(SIM_MODULE_LIST * head);

void
common_module_list_free(SIM_MODULE_LIST * root);

void
common_module_tree_print(SIM_MODULE_TREE * root);

SIM_MODULE_TREE *
common_module_tree_add_child(SIM_MODULE_TREE * father, SIM_MODULE * child, char *name);

SIM_MODULE_TREE *
common_module_tree_find_node(SIM_MODULE_TREE * root, char *name);

SIM_MODULE *
common_module_tree_find_module(SIM_MODULE_TREE * root, char *name);

SIM_MODULE_TREE *
common_module_tree_find_by_id(SIM_MODULE_TREE * root, int id);

SIM_MODULE_TREE *
common_module_tree_find_by_sortid(SIM_MODULE_TREE * root, int sortid);

void
common_module_tree_free(SIM_MODULE_TREE * root);

SIM_LINK *
common_link_list_find_dest(SIM_LINK_LIST * link_list, char * full_name);

void
common_link_print(SIM_LINK * link);

SIM_LINK_LIST *
common_link_list_append(SIM_LINK_LIST ** phead, SIM_LINK * link);

void
common_link_list_print(SIM_LINK_LIST * link_list);

