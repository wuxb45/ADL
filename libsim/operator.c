
#include "simtype.h"
#include "operator.h"

/**
 * pass 'x' or 'z'
 */
void
op_check_x_1(void *result, void *a)
{
	*((int *) result) = *((int *) a);
}

void
op_check_x_2(void *result, void *a, void *b)
{
	*((int *) result) = ((*((int *) a)) | (*((int *) b))) ? 1 : 0;
}

void
op_assign_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_F32(SIM_TYPE__F32 * result, SIM_TYPE__F32 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_F64(SIM_TYPE__F64 * result, SIM_TYPE__F64 * a)
{
	result->option = a->option;
	result->value = a->value;
}

void
op_assign_W01(SIM_TYPE__W01 * result, SIM_TYPE__W01 * a)
{
	result->option = a->option;
	result->value = a->value;
}



/**
 * ADD operation.
 */
void
op_add_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_F32(SIM_TYPE__F32 * result, SIM_TYPE__F32 * a, SIM_TYPE__F32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}

void
op_add_F64(SIM_TYPE__F64 * result, SIM_TYPE__F64 * a, SIM_TYPE__F64 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) + (b->value);
}
/**
 * ADD_carry operation.
 */
void
op_addc_U32(SIM_TYPE__U32 * result, SIM_TYPE__W01 * carry, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	op_check_x_2(result, a, b);
	op_check_x_2(carry, a, b);
	result->value = a->value + b->value;
	if(result->value < a->value && result->value < b->value){
		carry->value = -1;
	}else{
		carry->value = 0;
	}
}

void
op_addc_U16(SIM_TYPE__U16 * result, SIM_TYPE__W01 * carry, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	op_check_x_2(result, a, b);
	op_check_x_2(carry, a, b);
	result->value = a->value + b->value;
	if(result->value < a->value && result->value < b->value){
		carry->value = -1;
	}else{
		carry->value = 0;
	}
}

void
op_addc_U08(SIM_TYPE__U08 * result, SIM_TYPE__W01 * carry, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	op_check_x_2(result, a, b);
	op_check_x_2(carry, a, b);
	result->value = a->value + b->value;
	if(result->value < a->value && result->value < b->value){
		carry->value = -1;
	}else{
		carry->value = 0;
	}
}

/**
 * SUB operation.
 */
void
op_sub_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_F32(SIM_TYPE__F32 * result, SIM_TYPE__F32 * a, SIM_TYPE__F32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

void
op_sub_F64(SIM_TYPE__F64 * result, SIM_TYPE__F64 * a, SIM_TYPE__F64 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) - (b->value);
}

/**
 * MUL operation.
 */
void
op_mul_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_F32(SIM_TYPE__F32 * result, SIM_TYPE__F32 * a, SIM_TYPE__F32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

void
op_mul_F64(SIM_TYPE__F64 * result, SIM_TYPE__F64 * a, SIM_TYPE__F64 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) * (b->value);
}

/**
 * DIV operation.
 */
void
op_div_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_F32(SIM_TYPE__F32 * result, SIM_TYPE__F32 * a, SIM_TYPE__F32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

void
op_div_F64(SIM_TYPE__F64 * result, SIM_TYPE__F64 * a, SIM_TYPE__F64 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) / (b->value);
}

/**
 * MOD operation.
 */
void
op_mod_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_mod_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_mod_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_mod_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_mod_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_mod_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) % (b->value);
}

void
op_and_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) & (b->value);
}

void
op_and_W01(SIM_TYPE__W01 * result, SIM_TYPE__W01 * a, SIM_TYPE__W01 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = ((a->value) && (b->value)) ? 0xffffffff : 0;
}

void
op_or_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) | (b->value);
}

void
op_or_W01(SIM_TYPE__W01 * result, SIM_TYPE__W01 * a, SIM_TYPE__W01 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = ((a->value) || (b->value)) ? 0xffffffff : 0;
}

void
op_xor_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a, SIM_TYPE__U32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a, SIM_TYPE__U16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a, SIM_TYPE__U08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a, SIM_TYPE__I32 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a, SIM_TYPE__I16 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a, SIM_TYPE__I08 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (a->value) ^ (b->value);
}

void
op_xor_W01(SIM_TYPE__W01 * result, SIM_TYPE__W01 * a, SIM_TYPE__W01 * b)
{
	result->option = ((a->option) | (b->option)) ? 1 : 0;
	result->value = (((a->value) && (b->value == 0)) || ((a->value == 0)
					 && (b->value))) ? 0xffffffff : 0;
}

void
op_not_U32(SIM_TYPE__U32 * result, SIM_TYPE__U32 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_U16(SIM_TYPE__U16 * result, SIM_TYPE__U16 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_U08(SIM_TYPE__U08 * result, SIM_TYPE__U08 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_I32(SIM_TYPE__I32 * result, SIM_TYPE__I32 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_I16(SIM_TYPE__I16 * result, SIM_TYPE__I16 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_I08(SIM_TYPE__I08 * result, SIM_TYPE__I08 * a)
{
	result->option = a->option;
	result->value = ~(a->value);
}

void
op_not_W01(SIM_TYPE__W01 * result, SIM_TYPE__W01 * a)
{
	result->option = (a->option) ? 1 : 0;
	result->value = (a->value) ? 0 : 0xffffffff;
}

void
op_set_U32(SIM_TYPE__U32 * result, unsigned int value)
{
	result->option = 0;
	result->value = value;
}

void
op_set_U16(SIM_TYPE__U16 * result, unsigned short value)
{
	result->option = 0;
	result->value = value;
}
void
op_set_U08(SIM_TYPE__U08 * result, unsigned char value)
{
	result->option = 0;
	result->value = value;
}

void
op_set_I32(SIM_TYPE__I32 * result, int value)
{
	result->option = 0;
	result->value = value;
}

void
op_set_I16(SIM_TYPE__I16 * result, short value)
{
	result->option = 0;
	result->value = value;
}
void
op_set_I08(SIM_TYPE__I08 * result, char value)
{
	result->option = 0;
	result->value = value;
}

void
op_set_F32(SIM_TYPE__F32 * result, float value)
{
	result->option = 0;
	result->value = value;
}

void
op_set_F64(SIM_TYPE__F64 * result, double value)
{
	result->option = 0;
	result->value = value;
}

void
op_z_U32(SIM_TYPE__U32 * result, unsigned int value)
{
	result->option = 2;
}

void
op_z_U16(SIM_TYPE__U16 * result, unsigned short value)
{
	result->option = 2;
}

void
op_z_U08(SIM_TYPE__U08 * result, unsigned char value)
{
	result->option = 2;
}

void
op_z_I32(SIM_TYPE__I32 * result, int value)
{
	result->option = 2;
}

void
op_z_I16(SIM_TYPE__I16 * result, short value)
{
	result->option = 2;
}
void
op_z_I08(SIM_TYPE__I08 * result, char value)
{
	result->option = 2;
}

void
op_z_F32(SIM_TYPE__F32 * result, float value)
{
	result->option = 2;
}

void
op_z_F64(SIM_TYPE__F64 * result, double value)
{
	result->option = 2;
}

