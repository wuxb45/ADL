#include <stdio.h>
#include "simtype.h"

#include "trace.h"

void trace_cycle(int cycle)
{
	printf("\n++++  Cycle  [ %d ]\n", cycle);
}


void trace_value_U32(SIM_TYPE__U32 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %u | 0x%08x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XXXXXXXX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZZZZZZZ \n");
		break;
	default:
		printf(" ???????? \n");
		break;
	}
}

void trace_value_U16(SIM_TYPE__U16 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %u | 0x%04x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XXXX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZZZ \n");
		break;
	default:
		printf(" ???? \n");
		break;
	}
}

void trace_value_U08(SIM_TYPE__U08 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %u | 0x%02x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZ \n");
		break;
	default:
		printf(" ?? \n");
		break;
	}
}

void trace_value_I32(SIM_TYPE__I32 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %d | 0x%08x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XXXXXXXX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZZZZZZZ \n");
		break;
	default:
		printf(" ???????? \n");
		break;
	}
}

void trace_value_I16(SIM_TYPE__I16 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %d | 0x%04x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XXXX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZZZ \n");
		break;
	default:
		printf(" ???? \n");
		break;
	}
}

void trace_value_I08(SIM_TYPE__I08 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %d | 0x%02x ) \n", v->value, v->value);
		break;
	case SIM_OPTION_X:
		printf(" XX \n");
		break;
	case SIM_OPTION_Z:
		printf(" ZZ \n");
		break;
	default:
		printf(" ?? \n");
		break;
	}
}

void trace_value_F32(SIM_TYPE__F32 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %f ) \n", v->value);
		break;
	case SIM_OPTION_X:
		printf(" X \n");
		break;
	case SIM_OPTION_Z:
		printf(" Z \n");
		break;
	default:
		printf(" ? \n");
		break;
	}
}

void trace_value_F64(SIM_TYPE__F64 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %lf ) \n", v->value);
		break;
	case SIM_OPTION_X:
		printf(" X \n");
		break;
	case SIM_OPTION_Z:
		printf(" Z \n");
		break;
	default:
		printf(" ? \n");
		break;
	}
}

void trace_value_W01(SIM_TYPE__W01 * v)
{
	switch(v->option){
	case SIM_OPTION_VALID:
		printf(" ( %s ) \n", v->value ? "1" : "0");
		break;
	case SIM_OPTION_X:
		printf(" X \n");
		break;
	case SIM_OPTION_Z:
		printf(" Z \n");
		break;
	default:
		printf(" ? \n");
		break;
	}
}

