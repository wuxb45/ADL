#include <stdio.h>
#include "simtype.h"
#include "bus.h"
#define sim_error() return

void
bus_do_U32(SIM_TYPE__U32 * bus[])
{
	SIM_TYPE__U32 ** temp;
	int write_count = 0;
	SIM_TYPE__U32 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_U16(SIM_TYPE__U16 * bus[])
{
	SIM_TYPE__U16 ** temp;
	int write_count = 0;
	SIM_TYPE__U16 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_U08(SIM_TYPE__U08 * bus[])
{
	SIM_TYPE__U08 ** temp;
	int write_count = 0;
	SIM_TYPE__U08 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_I32(SIM_TYPE__I32 * bus[])
{
	SIM_TYPE__I32 ** temp;
	int write_count = 0;
	SIM_TYPE__I32 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_I16(SIM_TYPE__I16 * bus[])
{
	SIM_TYPE__I16 ** temp;
	int write_count = 0;
	SIM_TYPE__I16 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_I08(SIM_TYPE__I08 * bus[])
{
	SIM_TYPE__I08 ** temp;
	int write_count = 0;
	SIM_TYPE__I08 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_F32(SIM_TYPE__F32 * bus[])
{
	SIM_TYPE__F32 ** temp;
	int write_count = 0;
	SIM_TYPE__F32 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_F64(SIM_TYPE__F64 * bus[])
{
	SIM_TYPE__F64 ** temp;
	int write_count = 0;
	SIM_TYPE__F64 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

void
bus_do_W01(SIM_TYPE__W01 * bus[])
{
	SIM_TYPE__W01 ** temp;
	int write_count = 0;
	SIM_TYPE__W01 * write = NULL;

	temp = bus;
	while(*temp){
		if((*temp)->option == SIM_OPTION_VALID){
			write_count ++;
			write = *temp;
		}
		temp++;
	}

	if(write_count == 0){
		temp = bus;
		while(*temp){
			(*temp)->option = SIM_OPTION_X;
			temp++;
		}
	}else if(write_count == 1){
		temp = bus;
		while(*temp){
			(*temp)->value = write->value;
			temp ++;
		}
	}else{
		sim_error();
	}
}

