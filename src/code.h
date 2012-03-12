
#define CODE_TYPEDEF_STRUCT "typedef struct "

#define CODE_STRUCT_NAME_PREFIX "__sim_struct__"

#define CODE_TYPEDEF_PREFIX "SIM_STRUCT__"

#define CODE_BEHAVIOR_NAME_PERFIX "sim_function__"

#define CODE_BEHAVIOR_DECLARE_PREFIX "void inline\nsim_function__"

#define CODE_INITIAL_NAME_PREFIX "sim_initial__"

#define CODE_INITIAL_DECLARE_PREFIX "void\nsim_initial__"

#define CODE_FUNCTION_SELF_REF "__p_entity"

#define CODE_SIM_TYPE_PREFIX "SIM_TYPE__"

#define CODE_STATIC "static "

#define CODE_GLOBAL_PORT_NAME_PREFIX "__global_port__"

#define CODE_ENTITY_NAME_PREFIX "__entity__"

#define CODE_VAR_INDEX "__index__"

#define CODE_VAR_WRITE "__write__"

#define CODE_ENTITY_BUS_GROUP_PREFIX "__bus_group__"

#define CODE_SCHEDULE_DECLARE_PREFIX "void\nsim_main_schedule__"

#define CODE_SCHEDULE_NAME_PREFIX "sim_main_schedule__"

#define CODE_MAIN_INITIAL_NAME "sim_main_initial__"

#define CODE_MAIN_INITIAL_DECLARE "void\nsim_main_initial__"

#define CODE_BUS_DO_FUNCTION_PREFIX "bus_do_"


#define CODE_ENTITY_SELF_NAME "__global_entity_name__"

#define CODE_ASSIGN_PREFIX "op_assign_"

#define CODE_SET_PREFIX "op_set_"


#define CODE_TRACE_TEXT "__SIM_TRACE_TEXT__"

#define CODE_TRACE_VALUE_PREFIX "trace_value_"

// VCD

#define CODE_VCD_INITIAL "vcd_initial"

#define CODE_VCD_SYMBOL_ADD "vcd_symbol_add"

#define CODE_VCD_SYMBOL_ALIAS "vcd_symbol_alias"

#define CODE_VCD_SCOPE "vcd_scope"

#define CODE_VCD_UPSCOPE "vcd_upscope"

#define CODE_VCD_ENDDEF "vcd_enddef"

#define CODE_VCD_VAR "vcd_var"

#define CODE_VCD_DUMP_VAR "vcd_dump_var"

#define CODE_VCD_DUMP_ONE_CYCLE "vcd_dump_one_cycle"




void
code_do(SIM_MODULE_LIST * module_list, SIM_MODULE_TREE * entity_tree, SIM_LINK_LIST * link_list);

void
code_compile(void);
