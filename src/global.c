#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "util.h"
#include "common.h"
#include "config.h"

#include "global.h"



static int
global_recursive_make_link(SIM_MODULE_TREE * node, SIM_LINK_LIST ** plist);

static void
global_make_link_from_net(SIM_MODULE_TREE * node, SIM_NET * net, SIM_LINK_LIST ** plist);



static void
global_link_expand(SIM_LINK_LIST ** plist);


//static int
//global_make_compare_table(SIM_MODULE_TREE * module_tree, SIM_LINK_LIST * list);

static int **
global_compare_table_initial(int size, SIM_MODULE_TREE * module_tree, SIM_LINK_LIST * link_list);

static int
global_compare_table_set(int ** compare_table, int id1, int id2);

static void
global_compare_table_print(int ** compare_table);


static int *
global_link_sort(int ** compare_table);



static void
global_sorted_id_mark(SIM_MODULE_TREE * root, int * sort_result);

static void
global_recursive_check_coverage(SIM_MODULE_TREE * node, SIM_LINK_LIST * list);

static void
global_check_way_coverage(SIM_MODULE_TREE * node, SIM_ELEMENT * element, SIM_LINK_LIST * link_list);

static void
global_print(const char *format, ...);







SIM_LINK_LIST *
global_do(SIM_MODULE_TREE * entity_tree)
{

	SIM_LINK_LIST * link_list = NULL;
	int behavior_entity_count;
	int ** compare_table;
	int * sort_result;

	global_print("GLOBAL!!!!!\n\n\n\n");


	behavior_entity_count = global_recursive_make_link(entity_tree, &link_list);
	if(common_get_error_count()){
		return NULL;
	}

//	global_print("@@@@@@@@@@@new global links -- start\n");
//	common_link_list_print(_link_list);
//	global_print("@@@@@@@@@@@new global links -- end\n\n\n\n\n\n");

	global_link_expand(&link_list);
	if(common_get_error_count()){
		return NULL;
	}

//	global_print("@@@@@@@@@@@global_link_expand -- start\n");
//	common_link_list_print(_link_list);
//	global_print("@@@@@@@@@@@global_link_expand -- end\n");
//	global_print("++++++  entity_count %d\n\n\n\n\n\n", _behavior_entity_count);
	// sort
	// tasks

	global_recursive_check_coverage(entity_tree, link_list);

	compare_table = global_compare_table_initial(behavior_entity_count, entity_tree, link_list);

	if(compare_table == NULL){
		return NULL;
	}

	if(common_get_error_count()){
		return NULL;
	}

	sort_result = global_link_sort(compare_table);
	free(compare_table[0]);
	free(compare_table);
	if(common_get_error_count()){
		return NULL;
	}

	global_sorted_id_mark(entity_tree, sort_result);
	if(common_get_error_count()){
		return NULL;
	}

	common_module_tree_print(entity_tree);

	return link_list;
}

static int
global_recursive_make_link(SIM_MODULE_TREE * node, SIM_LINK_LIST ** plist)
{
	SIM_MODULE * module;
	SIM_NET_LIST * net_list;
	SIM_NET * net;
	SIM_MODULE_TREE * temp;
	//SIM_NET_ELEMENT_LIST * net_element_list;
	static int behavior_entity_count = 0;

	for(temp = node; temp; temp = temp->brother){
		module = temp->module;
//		global_print("IN:%s\n", temp->name);


		if(temp->id > behavior_entity_count){
			behavior_entity_count = temp->id;
		}

		if(module->behavior){
			continue;
		}

		for(net_list = module->net_list; net_list; net_list = net_list->next){
			net = net_list->net;
			if(net->type == NET_TYPE_NORMAL){
				global_make_link_from_net(temp, net, plist);
			}
		}
		//global_print("DOWN:%s\n", temp->child->name);
		global_recursive_make_link(temp->child, plist);
		//global_print("UP:%s\n", temp->child->name);
	}

	return behavior_entity_count;

}

static void
global_make_link_from_net(SIM_MODULE_TREE * node, SIM_NET * net, SIM_LINK_LIST ** plist)
{
	SIM_NET_ELEMENT_LIST * temp;
	SIM_LINK * link;
	SIM_NET_ELEMENT * src;
	SIM_NET_ELEMENT * dest;

	SIM_LINK_TYPE link_type;

	char * temp_name = NULL;
	int i;

	char ** src_split_dims = NULL;
	char ** dest_split_dims = NULL;

	char *src_full_name_const = NULL;

	SIM_MODULE_TREE * src_module_node = NULL;
	char * src_module_name = NULL;
	SIM_MODULE * src_module_type = NULL;
	SIM_ELEMENT * src_port_type = NULL;
//	char * src_port_way_prefix = NULL;

	SIM_MODULE_TREE * dest_module_node = NULL;
	char * dest_module_name = NULL;
	SIM_MODULE * dest_module_type = NULL;
	SIM_ELEMENT * dest_port_type = NULL;
//	char * dest_port_way_prefix = NULL;
	SIM_LINK_LIST * tail;

	if(net->writer == NULL){
		// wrong
		return;
	}else{
		src = net->writer;
	}

	/* src set */
	// src_module_name
	if(src->type == NET_ELEMENT_VALUE){

		link_type = LINK_CONST;

		src_full_name_const = src->port_name;

	}else{

		link_type = LINK_RW;

		temp_name = node->name;
		src_module_node = node;
		if(src->module_name){
			temp_name = util_join(temp_name, 0, ".", 0);
			temp_name = util_join(temp_name, 1, src->module_name, 0);
			src_module_node = common_module_tree_find_node(node, temp_name);
			if(src_module_node == NULL){
				common_error("Sub module not found.");
				return;
			}
		}

		src_module_name = temp_name;
		src_module_type = src_module_node->module;
		src_port_type = src->sim_element;

	}

	src_split_dims = common_dimension_split(src->way, src->dimension);

	/* all reader */
	tail = NULL;
	for(temp = net->reader_list; temp; temp = temp->next){

		dest = temp->net_element;

		temp_name = node->name;
		dest_module_node = node;

		if(dest->module_name){
			temp_name = util_join(temp_name, 0, ".", 0);
			temp_name = util_join(temp_name, 1, dest->module_name, 0);
			dest_module_node = common_module_tree_find_node(node, temp_name);
			if(dest_module_node == NULL){
				common_error("Sub module not found.");
				return;
			}
		}

		dest_module_name = temp_name;
		dest_module_type = dest_module_node->module;
		dest_port_type = dest->sim_element;

//		dest_port_way_prefix = common_dimension_array_to_string(dest->way->count, dest->way->width);
		dest_split_dims = common_dimension_split(dest->way, dest->dimension);

		for(i = 0; src_split_dims[i]; i++){
			if(dest_split_dims[i] == NULL){
				common_error("dims.....");
				return;
			}
			link = (SIM_LINK *)calloc(1, sizeof(SIM_LINK));
			link->type = link_type;

			if(link_type == LINK_RW){
				/* src_module_name */
				link->src_module_node = src_module_node;
				link->src_module_name = src_module_name;
				link->src_module_type = src_module_type;
				link->src_port_type = src_port_type;
//				link->src_port_way = util_join(src_port_way_prefix, 0, split_dims[i], 0);
				link->src_port_way = src_split_dims[i];

				temp_name = util_join(src_module_name, 0, ":", 0);
				temp_name = util_join(temp_name, 1, src_port_type->name, 0);
				temp_name = util_join(temp_name, 1, link->src_port_way, 0);
				link->src_full_name = temp_name;
			}else if(link_type == LINK_CONST){

				link->src_full_name = src_full_name_const;
			}


			link->dest_module_node = dest_module_node;
			link->dest_module_name = dest_module_name;
			link->dest_module_type = dest_module_type;
			link->dest_port_type = dest_port_type;
//			link->dest_port_way = util_join(dest_port_way_prefix, 0, split_dims[i], 0);
			link->dest_port_way = dest_split_dims[i];

			// full-name == dest_module_name:dest_port_type->name link->dest_port_way
			temp_name = util_join(dest_module_name, 0, ":", 0);
			temp_name = util_join(temp_name, 1, dest_port_type->name, 0);
			temp_name = util_join(temp_name, 1, link->dest_port_way, 0);
			link->dest_full_name = temp_name;

			if(common_link_list_find_dest(*plist, link->dest_full_name)){
				common_error("port [%s] has multiple driver.", link->dest_full_name);

			}else{
				if(tail == NULL){
					tail = common_link_list_append(plist, link);
				}else{
					tail = common_link_list_append(&tail, link);
				}
//				common_link_print(link);
			}
		}
	}
}





//static void
//global_make_bus_from_net(char * prefix, SIM_NET * net)
//{
//
//}

static void
global_link_expand(SIM_LINK_LIST ** plist)
{
	SIM_LINK_LIST * temp1;
	SIM_LINK_LIST * temp2;
	SIM_LINK * link1;
	SIM_LINK * link2;

	int convert_count;

	do{
		convert_count = 0;
		for(temp1 = *plist; temp1; temp1 = temp1->next){
			link1 = temp1->link;

			if(link1 == NULL){
				continue;
			}

			if(link1->dest_module_node->id > 0){
				continue;
			}

			if(link1->dest_module_type->behavior != 0){
				continue;
			}

			//global_print("Expand::\n");
			for(temp2 = *plist; temp2; temp2 = temp2->next){

				link2 = temp2->link;
				if(link2 == link1){
					continue;
				}

				if(link2 == NULL){
					continue;
				}

				if(link2->type == LINK_CONST){
					continue;
				}

				if(link2->src_module_type->behavior){
					continue;
				}

				//global_print("!!!compare:\n");
				//common_link_print(link);
				//common_link_print(link2);
				if(strcmp(link1->dest_full_name, link2->src_full_name)){
					continue;
				}
				//global_print("!!!match:\n");

				link2->type = link1->type;
				link2->src_full_name = link1->src_full_name;
				link2->src_module_name = link1->src_module_name;
				link2->src_module_node = link1->src_module_node;
				link2->src_module_type = link1->src_module_type;
				link2->src_port_type = link1->src_port_type;
				link2->src_port_way = link1->src_port_way;
				convert_count++;

			}

			free(link1);
			temp1->link = NULL;

		}

	} while(convert_count);

	// clean no use writer
	for(temp1 = *plist; temp1; temp1 = temp1->next){
		link1 = temp1->link;
		if(link1 == NULL){
			continue;
		}

		if(link1->type == LINK_RW && link1->src_module_type->behavior == 0){
			free(link1);
			temp1->link = NULL;
		}
	}


	while((*plist) && ((*plist)->link == NULL)){
		temp1 = (*plist);
		(*plist) = (*plist)->next;
		free(temp1);
	}
	temp1 = (*plist);

	while(temp1 && temp1->next){
		if(temp1->next->link == NULL){
			temp2 = temp1->next;
			temp1->next = temp2->next;
			free(temp2);
		}else{
			temp1 = temp1->next;
		}
	}

	//common_link_list_print(_link_list);
}


static int **
global_compare_table_initial(int size, SIM_MODULE_TREE * module_tree, SIM_LINK_LIST * link_list)
{
	SIM_LINK_LIST * temp;
	SIM_LINK * link;

	int result;
	int id1;
	int id2;
	int id3;
	int set_count;

	int * dim2;
	int ** compare_table;

	compare_table = (int **)malloc(sizeof(int *) * (size + 1));
	dim2 = (int *)calloc((size + 1) * (size + 1), sizeof(int));

	for(id1 = 0; id1 <= size; id1++){
		compare_table[id1] = &(dim2[id1 * (size + 1)]);
	}
	compare_table[0][0] = size;


	for(temp = link_list; temp; temp = temp->next){
		link = temp->link;

		if(link->type == LINK_CONST){
			continue;
		}

		if(link->dest_module_type->cycle > 0){
	//			global_print("skip:");
	//			common_link_print(link);
			continue;
		}

		id1 = link->src_module_node->id;
		if(id1 <= 0){
			common_error("id <= 0 ???");
			return NULL;
		}

		id2 = link->dest_module_node->id;
		if(id2 <= 0){
			common_error("id <= 0 ???");
			return NULL;
		}

		result = global_compare_table_set(compare_table, id1, id2);

		if(result < 0){
			return NULL;
		}
	}

	global_compare_table_print(compare_table);

	do{
		set_count = 0;

		for(id1 = 1; id1 <= size; id1 ++){
			for(id2 = 1; id2 <= size; id2 ++){
				if(compare_table[id1][id2] <= 0){
					continue;
				}

				for(id3 = 1; id3 <= size; id3 ++){
					if(compare_table[id2][id3] <= 0){
						continue;
					}
					result = global_compare_table_set(compare_table, id1, id3);
					if(result < 0){
						return NULL;
					}else if(result == 0){
						set_count++;
					}
				}
			}
		}
		global_print("set_count %d\n", set_count);
//		global_compare_table_print(compare_table);
	}while(set_count > 0);

	global_compare_table_print(compare_table);

	return compare_table;
}



static int
global_compare_table_set(int ** compare_table, int id1, int id2)
{
	// pointer test?
	int last_result;

	if(id1 <= 0 || id2 <= 0){
		common_error("id == 0......");
		return -1;
	}

	if(id1 > compare_table[0][0] || id2 > compare_table[0][0]){
		common_error("id > b_count......");
		return -1;
	}

	last_result = compare_table[id1][id2];

	if(compare_table[id1][id2] < 0){
		common_error("modules cannot be sorted id1[%d], id2[%d]\n", id1, id2);
	}else{
		compare_table[id1][id2] = 1;
		compare_table[id2][id1] = -1;
	}

	return last_result;
}


static void
global_compare_table_print(int ** compare_table)
{
	int id1, id2, value;
	int size;

	size = compare_table[0][0];

	global_print("TABLE size(%d)\n", compare_table[0][0]);

	global_print("   ");
	for(id1 = 0; id1 <= size; id1 ++){
		global_print(" %2d", id1);
	}
	global_print("\n");

	for(id1 = 1; id1 <= size; id1 ++){
		global_print("[%4d]", id1);
		for(id2 = 1; id2 <= size; id2 ++){
			value = compare_table[id1][id2];

			if(id1 == id2){
				global_print("\\");
			}else{
				global_print(" ");
			}

			if(value > 0){
				global_print("+ ", value);
			}else if(value < 0){
				global_print("# " );
			}else{
				global_print("_ " );

			}
		}
		global_print("\n");
	}
}


static int *
global_link_sort(int ** compare_table)
{
	int t, id1, id2;
	int * sort_convert;
	int select;
	int skip;
	int size;


	size = compare_table[0][0];

	sort_convert = (int *)malloc(sizeof(int) * (size + 1));

	for(t = 0; t < size; t++){
		skip = 1;
		for(id1 = 1; id1 <= size && skip; id1++){
			skip = 0;
			for(id2 = 1; id2 <= size; id2++){
				if(compare_table[id1][id2] < 0){
					skip = 1;
					break;
				}
			}
			if(skip == 0){
				select = id1;
			}
		}
		if(skip){
			common_error("cannot sort any more!");
			return NULL;
		}

		sort_convert[t] = select;

		for(id1 = 1; id1 <= size; id1++){
			compare_table[id1][select] = 1;
			compare_table[select][id1] = -1;
		}

//		global_compare_table_print();
//		global_print("select:%d\n", select);

	}

	global_compare_table_print(compare_table);

	global_print("sort done\n");
	for(t = 0; t < size; t++){
		global_print("%d ", sort_convert[t]);
	}
	global_print("\n");
	sort_convert[size] = 0;
	return sort_convert;
}


static void
global_sorted_id_mark(SIM_MODULE_TREE * root, int * sort_result)
{
	SIM_MODULE_TREE * node;

	int id;
	for(id = 0; sort_result[id]; id ++){
		node = common_module_tree_find_by_id(root, sort_result[id]);
		if(node == NULL){
			common_error("xxx");
			return;
		}

		node->sortid = id + 1;
	}
	root->sortid = id + 1;
}

static void
global_recursive_check_coverage(SIM_MODULE_TREE * node, SIM_LINK_LIST * link_list)
{
	SIM_MODULE * module;
	SIM_MODULE_TREE * temp;
	char * entity_name;
	char * port_name;
	SIM_ELEMENT_LIST * elist;
	SIM_ELEMENT * element;


	for(temp = node; temp; temp = temp->brother){

		module = temp->module;
		entity_name = temp->name;

		if(module->behavior == 0){
			if(temp->child){
				global_recursive_check_coverage(temp->child, link_list);
			}
			continue;
		}

		for(elist = module->element_list; elist; elist = elist->next){
			element = elist->element;
			port_name = element->name;

			switch(element->type){
			case ELEMENT_CONTAIN:
				break;
			case ELEMENT_INOUT:
				break;
			case ELEMENT_INPUT:
			case ELEMENT_OUTPUT:

				global_check_way_coverage(temp, element, link_list);
				break;
			case ELEMENT_RAM:
				break;
			default:
				break;
			}
		}


	}
}
static void
global_check_way_coverage(SIM_MODULE_TREE * node, SIM_ELEMENT * element, SIM_LINK_LIST * link_list)
{
	int i;
	char ** split_dims;
	SIM_LINK_LIST * temp;
	SIM_LINK * link;
	int find;

	split_dims = element->split_ways;
	for(i = 0; split_dims[i]; i++){

//		global_print("COVER CHECK [ %s:%s%s ]\n", node->name, element->name, split_dims[i]);
		find = 0;
		for(temp = link_list; temp; temp = temp->next){
			link = temp->link;

			if(link->dest_module_node == node && link->dest_port_type == element){
				if(strcmp(link->dest_port_way, split_dims[i]) == 0){
					find++;
					break;
				}
			}

			if(link->src_module_node == node && link->src_port_type == element){
				if(strcmp(link->src_port_way, split_dims[i]) == 0){
					find++;
				}
			}
		}

		if(find == 0){
			common_warning("Port not Used : %s:%s %s;", node->name, element->name, split_dims[i]);
		}

	}

}

static void
global_print(const char *format, ...)
{
	va_list arg;
	if (config_get_verbose() == 0) {
		return;
	}
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}
