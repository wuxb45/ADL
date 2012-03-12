#include <stdio.h>
#include <stdarg.h>


#include "common.h"
#include "config.h"
#include "bus.h"


static void
bus_recursive_make_bus(SIM_MODULE_TREE * node, SIM_BUS_LIST ** plist);


static void
bus_make_bus_from_net(SIM_MODULE_TREE * node, SIM_NET * net, SIM_BUS_LIST ** plist);


static void
bus_print(const char *format, ...);

SIM_BUS_LIST *
bus_do(SIM_MODULE_TREE * entity_tree)
{
	SIM_BUS_LIST * bus_list = NULL;
	return bus_list;
}

static void
bus_recursive_make_bus(SIM_MODULE_TREE * node, SIM_BUS_LIST ** plist)
{
	SIM_MODULE * module;
	SIM_NET_LIST * net_list;
	SIM_NET * net;
	SIM_MODULE_TREE * temp;
	//SIM_NET_ELEMENT_LIST * net_element_list;

	for(temp = node; temp; temp = temp->brother){
		module = temp->module;

		if(module->behavior){
			continue;
		}

		for(net_list = module->net_list; net_list; net_list = net_list->next){
			net = net_list->net;
			if(net->type == NET_TYPE_BUS){
				bus_make_bus_from_net(temp, net, plist);
			}
		}
		//global_print("DOWN:%s\n", temp->child->name);
		bus_recursive_make_bus(temp->child, plist);
		//global_print("UP:%s\n", temp->child->name);
	}

}




static void
bus_make_bus_from_net(SIM_MODULE_TREE * node, SIM_NET * net, SIM_BUS_LIST ** plist)
{
	bus_print("make bus:\n");
	common_net_print(net);
}



static void
bus_print(const char *format, ...)
{
	va_list arg;
	if (config_get_verbose() == 0) {
		return;
	}
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}
