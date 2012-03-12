void trace_cycle(int cycle);
void trace_value_U32(SIM_TYPE__U32 * v);
void trace_value_U16(SIM_TYPE__U16 * v);
void trace_value_U08(SIM_TYPE__U08 * v);
void trace_value_I32(SIM_TYPE__I32 * v);
void trace_value_I16(SIM_TYPE__I16 * v);
void trace_value_I08(SIM_TYPE__I08 * v);
void trace_value_F32(SIM_TYPE__F32 * v);
void trace_value_F64(SIM_TYPE__F64 * v);
void trace_value_W01(SIM_TYPE__W01 * v);

#define __SIM_TRACE_TEXT__(x) (printf("%s", (x)))

