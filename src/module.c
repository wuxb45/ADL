#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>

#include "common.h"
#include "util.h"
#include "config.h"

#include "module.h"



static void
module_check_contains(SIM_MODULE_LIST * module_list);

static void
module_check_contain(SIM_MODULE * module, SIM_MODULE_LIST * module_list);

static SIM_MODULE_TREE *
module_make_dependence_tree(SIM_MODULE * top_module);

static int
module_recursive_dependence_tree(SIM_MODULE_TREE * current_node);

static void
module_check_nets(SIM_MODULE_LIST * module_list);

static void
module_update_net(SIM_NET * net, SIM_MODULE_LIST * module_list);

static void
module_update_net_element(SIM_NET_ELEMENT * net_element, SIM_MODULE_LIST * module_list);

static int
module_check_net(SIM_NET * net);

static int
module_compare_data_type(SIM_NET_ELEMENT * ne1, SIM_NET_ELEMENT * ne2);

static SIM_MODULE_TREE *
module_make_entity_tree(SIM_MODULE * top_module);


//static void
//module_check_ports(SIM_MODULE_LIST * module_list);
//
//static void
//module_check_port_of_module(SIM_MODULE * module);



static void
module_recursive_entity_tree(SIM_MODULE_TREE * current_node);

static void
module_print(const char *format, ...);

/**
 * return the entity-tree of the top-module.
 */


SIM_MODULE_TREE *
module_do(SIM_MODULE_LIST * module_list)
{
	SIM_MODULE_TREE *dependence_tree = NULL;
	SIM_MODULE_TREE *entity_tree = NULL;
	char * top_module_name;
	SIM_MODULE * top_module;


	top_module_name = config_get_top_module();

	top_module = common_module_list_find_name(module_list, top_module_name);
	if (top_module == NULL) {
		common_error("Top Module not found. [%s]", top_module_name);
		return NULL;
	}

	module_check_contains(module_list);
	if(common_get_error_count()){
		return NULL;
	}

	dependence_tree = module_make_dependence_tree(top_module);
	if (dependence_tree == NULL) {
		return NULL;
	}

	module_check_nets(module_list);
	if (common_get_error_count()) {
		return NULL;
	}

//	module_check_ports(module_list);

	entity_tree = module_make_entity_tree(top_module);

	return entity_tree;
}


/**
 * Step 1 of module's work.
 * check that the top-module (given in command line) exists in module list.
 * check that all the 'contain' items is available, by calling module_check_contain().
 */
static void
module_check_contains(SIM_MODULE_LIST * module_list)
{
	SIM_MODULE_LIST * node;

	for (node = module_list; node; node = node->next) {
		module_check_contain(node->module, module_list);
	}
}

/**
 * Step 2 of module's work.
 * As all the containing statments' validity,
 * make a tree that represents the containing
 * relationship between modules.
 * Brothers are distinct modules.
 */
static void
module_check_contain(SIM_MODULE * module, SIM_MODULE_LIST * module_list)
{
	SIM_ELEMENT_LIST *list;
	SIM_ELEMENT * element;
	SIM_MODULE *contain;

	for(list = module->element_list; list; list = list->next) {
		element = list->element;

		if(element->type != ELEMENT_CONTAIN){
			continue;
		}

		contain = common_module_list_find_name(module_list, element->module_type);
//		module_print("check contain :[%s];\n", element->module_type);
		if(contain == NULL){
			common_error("contain module type not fount.");
			common_element_error(element);
		}
		/* set element the pointer to its module */
		element->module = contain;
	}
}

static SIM_MODULE_TREE *
module_make_dependence_tree(SIM_MODULE * top_module)
{
	int result;

	SIM_MODULE_TREE *tree_root;
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;

//Top Module contains port
	for(elist = top_module->element_list; elist; elist = elist->next){
		element = elist->element;
		if(element->type == ELEMENT_INPUT ||
				element->type == ELEMENT_OUTPUT ||
				element->type == ELEMENT_INOUT){
			common_warning("Top Module contains port. @ [%s]", top_module->name);
			break;
		}
	}

	tree_root = (SIM_MODULE_TREE *) calloc(1, sizeof (SIM_MODULE_TREE));
	tree_root->name = top_module->name;
	tree_root->module = top_module;

	//  module_print("tree down : %s\n", module->name);
	result = module_recursive_dependence_tree(tree_root);
	if (result) {
		common_error("Self containing!! @ module_make_dependence_tree");
		return NULL;
	} else {					/* success! */
		module_print("Dependence check succeeded!\n");
	}
	common_module_tree_print(tree_root);
	return tree_root;
}

/**
 * Step 3 of module's work.
 * Check for the links' validity in each module.
 */
static int
module_recursive_dependence_tree(SIM_MODULE_TREE * current_node)
{
	SIM_ELEMENT_LIST *elist;
	SIM_ELEMENT * element;

	SIM_MODULE_TREE *child;
	SIM_MODULE_TREE *up;
	SIM_MODULE_TREE *temp;
	int skip;
	int result;

	for(elist = current_node->module->element_list; elist; elist = elist->next){

		element = elist->element;
		if(element->type != ELEMENT_CONTAIN){
			continue;
		}

		for (skip = 0, temp = current_node->child; temp; temp = temp->brother) {
			if (temp->module == element->module) {
				skip = 1;
				break;
			}
		}

		if(skip){
			continue;
		}

		for (up = current_node; up; up = up->father) {
			if (up->module == element->module) {
				common_error("self_contain");
				common_module_error(up->module);
				common_module_error(element->module);
				return -1;
			}
		}

		child = common_module_tree_add_child(current_node, element->module, NULL);

		result = module_recursive_dependence_tree(child);
		if (result) {
			return -1;
		}
		// module_print("tree up : %s\n", contains->name2);

	}
	return 0;
}

static void
module_check_nets(SIM_MODULE_LIST * module_list)
{
	SIM_MODULE *module;
	SIM_MODULE_LIST * m_list;

	SIM_NET_LIST * nlist;

	module_print("++----module_check_nets\n");

	for(m_list = module_list; m_list; m_list = m_list->next){

		module = m_list->module;

		if(module->behavior){
			continue;
		}

		for(nlist = module->net_list; nlist; nlist = nlist->next){

			module_update_net(nlist->net, module_list);

			common_net_element_list_print(nlist->net->net_element_list);

			module_check_net(nlist->net);
		}
	}

}

static void
module_update_net(SIM_NET * net, SIM_MODULE_LIST * module_list)
{
	SIM_NET_ELEMENT_LIST * nelist;

	for(nelist = net->net_element_list; nelist; nelist = nelist->next){
		module_update_net_element(nelist->net_element, module_list);
	}
}

static void
module_update_net_element(SIM_NET_ELEMENT * net_element, SIM_MODULE_LIST * module_list)
{
	SIM_ELEMENT * contain;
	SIM_ELEMENT * element;
	SIM_MODULE * father;
	SIM_MODULE * module;
	char * name;
	int i, j;
	int * width_new;
	SIM_DIMENSION * dimension_new;

	father = net_element->father;

	// only update NET_ELEMENT_PORT
	if(net_element->type != NET_ELEMENT_PORT){
		return;
	}


	/* sub.port : make module_name.port_name */
	if(net_element->module_name){
		contain = common_element_list_find_name(father->element_list, net_element->module_name);

		if(contain == NULL || contain->type != ELEMENT_CONTAIN){
			common_error("Contain [%s] not found!", net_element->module_name);
			common_net_element_error(net_element);
			return;
		}

		// get contain's module name, and get the module struct
		name = contain->module_type;
		module = common_module_list_find_name(module_list, name);
		if(module == NULL){
			common_error("Module [%s] not found!", name);
			common_net_element_error(net_element);
			return;
		}
	}else{ /* net_element->module_name == NULL self-port */
		module = father;
	}

	// find the element in the module.
	element = common_element_list_find_name(module->element_list, net_element->port_name);
	if(element == NULL ||
			element->type == ELEMENT_CONTAIN ||
			element->type == ELEMENT_RAM ||
			element->type == ELEMENT_UNKNOWN){
		if(net_element->module_name){
			common_error("port not found at %s: %s.%s",
					father->name, net_element->module_name, net_element->port_name);
		}else{
			common_error("port not found at %s: %s",
					father->name, net_element->port_name);
		}
		return;
	}

	// remember the pointer in net_element
	net_element->sim_element = element;

	switch(element->type){
	case ELEMENT_INPUT:
		if(net_element->module_name){
			net_element->type = NET_ELEMENT_READER;
		}else{
			net_element->type = NET_ELEMENT_WRITER;
		}
		break;

	case ELEMENT_OUTPUT:
		if(net_element->module_name){
			net_element->type = NET_ELEMENT_WRITER;
		}else{
			net_element->type = NET_ELEMENT_READER;
		}
		break;

	case ELEMENT_INOUT:
		net_element->type = NET_ELEMENT_INOUT;
		break;

	case ELEMENT_CONTAIN:
		common_error("net_element cannot be [contain]!");
		common_net_element_error(net_element);
		break;

	case ELEMENT_RAM:
		common_error("net_element cannot be [ram]!");
		common_net_element_error(net_element);
		break;

	case ELEMENT_UNKNOWN:
		common_error("net_element xxx!");
		common_net_element_error(net_element);
		break;
	default:
		common_error("net_element xxxxxx!");
		common_net_element_error(net_element);
		break;
	}


	/* update dimension */

	if((element->dimension->count) < (net_element->way->count + net_element->way->way)){
		// you can use at most all dimension
		common_error("bad way");
		common_net_element_error(net_element);
		return ;
	}

	// check way bounds
	for(i = 0; i < net_element->way->count; i++){
		if(net_element->way->width[i] >= element->dimension->width[i]){
			common_error("Out of bound");
			common_net_element_error(net_element);
		}
	}

	// check way range bound
	if(net_element->way->way){
		if(net_element->way->way_high >= element->dimension->width[i]){
			common_error("Out of bound");
			common_net_element_error(net_element);
		}
	}

	// set the dimension of the net_element, a sub-set of a port.
	if((element->dimension->count - i) > 0){
		width_new = (int *)malloc(sizeof(int) * (element->dimension->count - i));

		j = 0;
		if(net_element->way->way){
			width_new[0] = net_element->way->way_high - net_element->way->way_low + 1;
			j++;
			i++;
		}

		for(; i < element->dimension->count; i++, j++){
			width_new[j] = element->dimension->width[i];
		}
	}else{
		width_new = NULL;
		j = 0;
	}

	dimension_new = common_dimension_new(j, width_new, 0, 0, 0 );
	net_element->dimension = dimension_new;

	net_element->split_ways = common_dimension_split(net_element->way, net_element->dimension);


}

static int
module_check_net(SIM_NET * net)
{
	SIM_NET_ELEMENT_LIST * list;
	SIM_NET_ELEMENT * net_element;
	SIM_NET_ELEMENT * compare = NULL;

	int writer_count = 0;
	int inout_count = 0;
	int normal_count = 0;

	/* check bus or normal, *count */
	for(list= net->net_element_list; list; list = list->next){
		net_element = list->net_element;
		switch(net_element->type){
		case NET_ELEMENT_WRITER:
			normal_count++;

			writer_count++;

			net->writer = net_element;

			if(compare == NULL){
				compare = net_element;
			}
			break;
		case NET_ELEMENT_READER:
			normal_count++;

			common_net_element_list_append(&(net->reader_list), net_element);

			if(compare == NULL){
				compare = net_element;
			}
			break;
		case NET_ELEMENT_INOUT:
			inout_count++;

			common_net_element_list_append(&(net->reader_list), net_element);

			if(compare == NULL){
				compare = net_element;
			}
			break;
		case NET_ELEMENT_PORT:
			common_error("xxx");
			return -1;
			break;
		case NET_ELEMENT_VALUE:

			normal_count++;

			writer_count++;
			net->writer = net_element;
			break;

		case NET_ELEMENT_UNKNOWN:
			common_error("xxx");
			common_net_element_error(net_element);
			break;
		}
	}

	module_print("writer_count %d, ", writer_count);
	module_print("inout_count %d, ", inout_count);
	module_print("normal_count %d\n", normal_count);

	// only normal, or only inout
	if(normal_count && inout_count){
		common_error("[inout] port cannot connect with ohter ports.");
		common_net_element_list_error(net->net_element_list);
		return -1;
	}

	// only one writer
	if(writer_count > 1){
		common_error("More than one writer.");
		common_net_element_list_error(net->net_element_list);
		return -1;
	}

	// normal net must has a writer
	if(writer_count == 0 && normal_count > 0){
		common_error("No writer.");
		common_net_element_list_error(net->net_element_list);
	}

	// a net contains at least two elements
	if(normal_count + inout_count <= 1){
		common_error("Net has too few elements.");
		common_net_element_list_error(net->net_element_list);
		return -1;
	}

	//
	if(compare == NULL){
		common_error("No port");
		common_net_element_list_error(net->net_element_list);
		return -1;
	}

	// compare <compare> with every elements
	for(list = net->net_element_list; list; list = list->next){
		net_element = list->net_element;
		switch(net_element->type){
		case NET_ELEMENT_WRITER:
		case NET_ELEMENT_READER:
		case NET_ELEMENT_INOUT:
			if(module_compare_data_type(net_element, compare)){
				return -1;
			}
			break;
		default:
			break;
		}
	}

	// clone a value writer dimension info from <compare>
	if(net->writer->type == NET_ELEMENT_VALUE){
//		net->writer->dimension = compare->dimension;
//		net->writer->way = common_dimension_new(0, NULL, 0,0,0);
	}

	net->data_type = compare->sim_element->data_type;
	net->dimension = compare->dimension;

	if(normal_count){
		net->type = NET_TYPE_NORMAL;
	}else if(inout_count){
		net->type = NET_TYPE_BUS;
	}

	return 0;
}

static int
module_compare_data_type(SIM_NET_ELEMENT * ne1, SIM_NET_ELEMENT * ne2)
{
	int i;
	if(ne1 == ne2){
		return 0;
	}

	// check data type
	if(ne1->sim_element->data_type != ne2->sim_element->data_type){
		common_error("data_type not match:");
		common_net_element_error(ne1);
		common_net_element_error(ne2);
	}

	// check the number of dimensions
	if(ne1->dimension->count != ne2->dimension->count){
		common_error("dimension not match");
		common_net_element_error(ne1);
		common_net_element_error(ne2);
		return 0;
	}

	// check all dimension width
	for(i = 0; i < ne1->dimension->count; i++){
		if(ne1->dimension->width[i] != ne2->dimension->width[i]){
			common_error("width not match");
			common_net_element_error(ne1);
			common_net_element_error(ne2);
			break;
		}
	}

	return 0;
}

static SIM_MODULE_TREE *
module_make_entity_tree(SIM_MODULE * top_module)
{
	SIM_MODULE_TREE *tree_root;

	tree_root = (SIM_MODULE_TREE *) calloc(1, sizeof (SIM_MODULE_TREE));
	tree_root->name = top_module->name;
	tree_root->module = top_module;
	tree_root->id = 0;

	module_recursive_entity_tree(tree_root);
	common_module_tree_print(tree_root);
	return tree_root;
}

static void
module_recursive_entity_tree(SIM_MODULE_TREE * current_node)
{
	SIM_ELEMENT_LIST *elist;
	SIM_ELEMENT * element;

	SIM_MODULE_TREE *child;
	char * child_name;
	static int id_count = 1;

	current_node->module->entity_count ++;

	for(elist = current_node->module->element_list; elist; elist = elist->next){
		element = elist->element;
		if(element->type != ELEMENT_CONTAIN){
			continue;
		}
		child_name = util_join(current_node->name, 0, ".", 0);
		child_name = util_join(child_name, 1, element->name, 0);

		child = common_module_tree_add_child(current_node, element->module, child_name);

		if(child->module->behavior){
			child->id = id_count;
			id_count++;
		}else{
			child->id = -1;
		}

		module_recursive_entity_tree(child);
	}
}

//
//static void
//module_check_ports(SIM_MODULE_LIST * module_list)
//{
//	SIM_MODULE * module;
//	SIM_MODULE_LIST * list;
//	for(list = module_list; list; list = list->next){
//		module = list->module;
//		if(module->behavior){
//			continue;
//		}
//		module_check_port_of_module(module);
//	}
//}
//
//
///**
// * check if all the port is in use
// */
//
//static void
//module_check_port_of_module(SIM_MODULE * module)
//{
//	SIM_ELEMENT_LIST * elist;
//	SIM_ELEMENT * element;
//	SIM_NET_LIST * nlist;
//	SIM_NET * net;
//	SIM_NET_ELEMENT_LIST * nelist;
//	SIM_NET_ELEMENT * net_element;
//
//	char ** dim_all;
//	char ** dim_net;
//	int i1, i2;
//	char * temp1;
//	char * temp2;
//	int used_flag;
//
//	if(module->behavior){
//		return;
//	}
//
//	for(elist = module->element_list; elist; elist = elist->next){
//		element = elist->element;
//
//
//
//
//	}
//	return;
//}



static void
module_print(const char *format, ...)
{
	va_list arg;
	if (config_get_verbose() == 0) {
		return;
	}
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}
