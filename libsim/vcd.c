#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "simtype.h"

#include "vcd.h"


static char *
vcd_id(int value);

static void
vcd_var_id_list_append(VCD_VAR_ID_LIST ** plist, VCD_VAR_ID * vi);

static int
vcd_dump_var_U32(VCD_VAR_ID * vi);

static int
vcd_dump_var_I32(VCD_VAR_ID * vi);

static int
vcd_dump_var_U16(VCD_VAR_ID * vi);

static int
vcd_dump_var_I16(VCD_VAR_ID * vi);

static int
vcd_dump_var_U08(VCD_VAR_ID * vi);

static int
vcd_dump_var_I08(VCD_VAR_ID * vi);

static int
vcd_dump_var_F32(VCD_VAR_ID * vi);

static int
vcd_dump_var_F64(VCD_VAR_ID * vi);

static int
vcd_dump_var_W01(VCD_VAR_ID * vi);

static VCD_DUMP __vcd_dump;







static char *
vcd_id(int value)
{
	static char buf[16];
	int i;

	for(i=0;i<15;i++){

		buf[i]=(char)((value%94)+33); /* for range 33..126 */

		value=value/94;

		if(value == 0) {
			buf[i+1]=0;
			break;
		}
	}

	if(value != 0){
		buf[15] = 0;
	}

	return(buf);
}


static void
vcd_var_id_list_append(VCD_VAR_ID_LIST ** plist, VCD_VAR_ID * vi)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID_LIST * new_list;
	new_list = (VCD_VAR_ID_LIST *) calloc (1, sizeof(VCD_VAR_ID_LIST));
	new_list->next = NULL;
	new_list->vi = vi;

	if(*plist == NULL){
		*plist = new_list;
		return;
	}

	list = *plist;
	while(list->next){
		list = list->next;
	}
	list->next = new_list;

}

int
vcd_initial(char * src_file_name)
{
	FILE * dump_file;
	time_t walltime;

	if(src_file_name == NULL){
		return -1;
	}

	dump_file = fopen(src_file_name, "w");
	if(dump_file == NULL){
		return -1;
	}

	__vcd_dump.current_id = 0;
	__vcd_dump.cycle = 0;
	__vcd_dump.var_id_list = NULL;
	__vcd_dump.output = dump_file;

	// fill initial command

	// date
	fprintf(dump_file, VCD_COMMAND_DATE);
	time(&walltime);
	fprintf(dump_file, "%s", asctime(localtime(&walltime)));
	fprintf(dump_file, VCD_COMMAND_END);

	// version
	fprintf(dump_file, VCD_COMMAND_VERSION "Wuxb's adlforsim 2, wuxb45@gmail.com");
	fprintf(dump_file, VCD_COMMAND_END);

	// timescale
	fprintf(dump_file, VCD_COMMAND_TIMESCALE "1s" VCD_COMMAND_END);

	fflush(dump_file);
	return 0;
}



int
vcd_symbol_add(char * name, char * show, SIM_TYPE_E type, void * pdata, int width_w01)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID * vi;
	int id;

	for(list = __vcd_dump.var_id_list; list; list = list->next){
		vi = list->vi;

		if(strcmp(vi->var, name) == 0){ // exists
			return -1;
		}
	}

	id = (__vcd_dump.current_id)++;
	vi = (VCD_VAR_ID *) malloc (sizeof(VCD_VAR_ID));
	vi->id = id;
	vi->var = name;
	vi->show = show;
	vi->sim_type = type;
	vi->pdata = pdata;

	switch(type){
	case ST_W01:
		vi->type = "wire";
		vi->width = width_w01;
		vi->pdata_1 = (SIM_TYPE__W01 *)malloc(sizeof(SIM_TYPE__W01) * width_w01);
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__W01) * width_w01);
		break;

	case ST_F32:
		vi->type = "real";
		vi->width = 32;
		vi->pdata_1 = (SIM_TYPE__F32 *)malloc(sizeof(SIM_TYPE__F32));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__F32));
		break;

	case ST_U32:
		vi->type = "integer";
		vi->width = 32;
		vi->pdata_1 = (SIM_TYPE__U32 *)malloc(sizeof(SIM_TYPE__U32));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__U32));
		break;

	case ST_I32:
		vi->type = "integer";
		vi->width = 32;
		vi->pdata_1 = (SIM_TYPE__I32 *)malloc(sizeof(SIM_TYPE__I32));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__I32));
		break;

	case ST_F64:
		vi->type = "real";
		vi->width = 64;
		vi->pdata_1 = (SIM_TYPE__F64 *)malloc(sizeof(SIM_TYPE__F64));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__F64));
		break;

	case ST_I08:
		vi->type = "integer";
		vi->width = 8;
		vi->pdata_1 = (SIM_TYPE__I08 *)malloc(sizeof(SIM_TYPE__I08));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__I08));
		break;

	case ST_U08:
		vi->type = "integer";
		vi->width = 8;
		vi->pdata_1 = (SIM_TYPE__U08 *)malloc(sizeof(SIM_TYPE__U08));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__U08));
		break;

	case ST_I16:
		vi->type = "integer";
		vi->width = 16;
		vi->pdata_1 = (SIM_TYPE__I16 *)malloc(sizeof(SIM_TYPE__I16));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__I16));
		break;

	case ST_U16:
		vi->type = "integer";
		vi->width = 16;
		vi->pdata_1 = (SIM_TYPE__U16 *)malloc(sizeof(SIM_TYPE__U16));
		memcpy(vi->pdata_1, vi->pdata, sizeof(SIM_TYPE__U16));
		break;

	default:
		vi->width = 0;
		break;
	}

	if(vi->width == 0){
		return -1;
	}

	vcd_var_id_list_append(&(__vcd_dump.var_id_list), vi);
	fprintf(__vcd_dump.output, VCD_COMMAND_VAR "%s %d %s %s" VCD_COMMAND_END,
			vi->type, vi->width, vcd_id(vi->id), vi->show);

	return 0;

}

int
vcd_symbol_alias(char * new_name, char * alias_name, void * pdata)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID * vi;
	VCD_VAR_ID * alias;

	alias = NULL;
	for(list = __vcd_dump.var_id_list; list; list = list->next){
		vi = list->vi;

		if(strcmp(vi->var, alias_name) == 0){ // exists
			alias = vi;
			break;
		}
	}

	if(alias == NULL){
		return -1;
	}

	vi = (VCD_VAR_ID *) malloc (sizeof(VCD_VAR_ID));
	vi->var = new_name;
	vi->show = alias->show;
	vi->id = alias->id;
	vi->sim_type = alias->sim_type;
	vi->width = alias->width;
	vi->type = alias->type;
	vi->pdata = pdata;

	vcd_var_id_list_append(&(__vcd_dump.var_id_list), vi);

	return 0;

}

void
vcd_scope(char * entity_name)
{
	fprintf(__vcd_dump.output, VCD_COMMAND_SCOPE "module %s" VCD_COMMAND_END, entity_name);
}

void
vcd_upscope(void)
{
	fprintf(__vcd_dump.output, VCD_COMMAND_UPSCOPE  VCD_COMMAND_END);
}

void
vcd_enddef(void)
{
	fprintf(__vcd_dump.output, VCD_COMMAND_ENDDEF  VCD_COMMAND_END);
}

/**
 * declare the var
 */
int
vcd_var(char * name)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID * temp;
	VCD_VAR_ID * vi;


	vi = NULL;
	for(list = __vcd_dump.var_id_list; list; list = list->next){
		temp = list->vi;
		if(strcmp(temp->var, name)){
			vi = temp;
			break;
		}
	}

	if(vi == NULL){
		return -1;
	}

	fprintf(__vcd_dump.output, VCD_COMMAND_VAR );
	fprintf(__vcd_dump.output, "%s %d %s %s", vi->type, vi->width, vcd_id(vi->id), vi->show);
	fprintf(__vcd_dump.output, VCD_COMMAND_END);
	return 0;
}

int
vcd_dump_var(char * name)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID * temp;
	VCD_VAR_ID * vi;

	vi = NULL;
	for(list = __vcd_dump.var_id_list; list; list = list->next){
		temp = list->vi;

		if(strcmp(temp->var, name) == 0){
			vi = temp;
			break;
		}
	}

	if(vi == NULL){
		return -1;
	}

	switch(vi->sim_type){
	case ST_W01:
		vcd_dump_var_W01(vi);
		break;
	case ST_F32:
		vcd_dump_var_F32(vi);
		break;
	case ST_U32:
		vcd_dump_var_U32(vi);
		break;
	case ST_I32:
		vcd_dump_var_I32(vi);
		break;
	case ST_F64:
		vcd_dump_var_F64(vi);
		break;
	case ST_I08:
		vcd_dump_var_I08(vi);
		break;
	case ST_U08:
		vcd_dump_var_U08(vi);
		break;
	case ST_I16:
		vcd_dump_var_I16(vi);
		break;
	case ST_U16:
		vcd_dump_var_U16(vi);
		break;
	default:
		break;
	}
	return 0;
}

void
vcd_dump_one_cycle(void)
{
	VCD_VAR_ID_LIST * list;
	VCD_VAR_ID * vi;

	fprintf(__vcd_dump.output, "#%d\n", __vcd_dump.cycle);
	(__vcd_dump.cycle)++;
	
	for(list = __vcd_dump.var_id_list; list; list = list->next){
		vi = list->vi;

		switch(vi->sim_type){
		case ST_W01:
			vcd_dump_var_W01(vi);
			break;
		case ST_F32:
			vcd_dump_var_F32(vi);
			break;
		case ST_U32:
			vcd_dump_var_U32(vi);
			break;
		case ST_I32:
			vcd_dump_var_I32(vi);
			break;
		case ST_F64:
			vcd_dump_var_F64(vi);
			break;
		case ST_I08:
			vcd_dump_var_I08(vi);
			break;
		case ST_U08:
			vcd_dump_var_U08(vi);
			break;
		case ST_I16:
			vcd_dump_var_I16(vi);
			break;
		case ST_U16:
			vcd_dump_var_U16(vi);
			break;
		default:
			break;
		}
	}
}

static int
vcd_dump_var_U32(VCD_VAR_ID * vi)
{
	SIM_TYPE__U32 * var;
	char buffer[33];
	char * bit;
	unsigned int mask;
	int i;


	if(vi->sim_type != ST_U32){
		return -1;
	}
	
	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__U32)) == 0){
		return 0;
	}
	var = vi->pdata;

	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x80000000, i = 0; i < 32; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[32] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_I32(VCD_VAR_ID * vi)
{
	SIM_TYPE__I32 * var;
	char buffer[33];
	char * bit;
	unsigned int mask;
	int i;

	if(vi->sim_type != ST_I32){
		return -1;
	}
	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__I32)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x80000000, i = 0; i < 32; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[32] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}


static int
vcd_dump_var_U16(VCD_VAR_ID * vi)
{
	SIM_TYPE__U16 * var;
	char buffer[17];
	char * bit;
	unsigned short mask;
	int i;

	if(vi->sim_type != ST_U16){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__U16)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x8000, i = 0; i < 16; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[16] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_I16(VCD_VAR_ID * vi)
{
	SIM_TYPE__I16 * var;
	char buffer[17];
	char * bit;
	unsigned short mask;
	int i;

	if(vi->sim_type != ST_I16){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__I16)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x8000, i = 0; i < 16; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[16] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_U08(VCD_VAR_ID * vi)
{
	SIM_TYPE__U08 * var;
	char buffer[9];
	char * bit;
	unsigned char mask;
	int i;

	if(vi->sim_type != ST_U08){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__U08)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x80, i = 0; i < 8; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[8] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_I08(VCD_VAR_ID * vi)
{
	SIM_TYPE__I08 * var;
	char buffer[9];
	char * bit;
	unsigned char mask;
	int i;

	if(vi->sim_type != ST_I08){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__I08)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		for(mask = 0x80, i = 0; i < 8; i++, mask >>=1){
			buffer[i] = (var->value & mask) ? '1' : '0';
		}
		buffer[8] = '\0';
		bit = buffer;
		while(bit[0] == '0' && (bit[1])){
			bit++;
		}
		break;
	case SIM_OPTION_X:
		bit = "x";
		break;
	case SIM_OPTION_Z:
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_F32(VCD_VAR_ID * vi)
{
	SIM_TYPE__F32 * var;
	char buffer[32];
	char * bit;
	char head;

	if(vi->sim_type != ST_F32){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__F32)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		sprintf(buffer, "%.16g", var->value);
		head = 'r';
		bit = buffer;
		break;
	case SIM_OPTION_X:
		head = 'b';
		bit = "x";
		break;
	case SIM_OPTION_Z:
		head = 'b';
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}

static int
vcd_dump_var_F64(VCD_VAR_ID * vi)
{
	SIM_TYPE__F64 * var;
	char buffer[32];
	char * bit;
	char head;

	if(vi->sim_type != ST_F64){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, sizeof(SIM_TYPE__F64)) == 0){
		return 0;
	}

	var = vi->pdata;
	switch(var->option){
	case SIM_OPTION_VALID:
		sprintf(buffer, "%.16g", var->value);
		head = 'r';
		bit = buffer;
		break;
	case SIM_OPTION_X:
		head = 'b';
		bit = "x";
		break;
	case SIM_OPTION_Z:
		head = 'b';
		bit = "z";
		break;
	default:
		return -1;
		break;
	}

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}


static int
vcd_dump_var_W01(VCD_VAR_ID * vi)
{
	SIM_TYPE__I08 * var;
	char * bit;
	int width;
	int i;


	if(vi->sim_type != ST_W01){
		return -1;
	}

	if(memcmp(vi->pdata, vi->pdata_1, (sizeof(SIM_TYPE__W01)) * vi->width ) == 0){
		return 0;
	}

	var = vi->pdata;
	width = vi->width;
	bit = (char *)malloc((width + 1) * sizeof(char));

	for(i = 0; i < width; i++){
		switch(var[i].option){
		case SIM_OPTION_VALID:
			bit[i] = var[i].value ? '1':'0';
			break;
		case SIM_OPTION_X:
			bit = "x";
			break;
		case SIM_OPTION_Z:
			bit = "z";
			break;
		default:
			return -1;
			break;
		}
	}
	bit[width] = 0;

	fprintf(__vcd_dump.output, "b%s %s\n", bit, vcd_id(vi->id));
	return 0;
}
