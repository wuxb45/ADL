#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"
#include "util.h"
#include "line.h"
#include "config.h"


static char * __data_type_name_list[] = {
		"U32", "U16", "U08", "I32", "I16", "I08", "F32", "F64", "W01", NULL
};

/*
static int __data_type_width_list[] = {
		32, 16, 8, 32, 16, 8, 32, 64, 1, 1
};
*/

static SIM_DATA_TYPE __data_type_e_list[] = {
	SDT_U32, SDT_U16, SDT_U08,
	SDT_I32, SDT_I16, SDT_I08,
	SDT_F32, SDT_F64, SDT_W01,
	SDT_UNKNOWN
};

static int common_error_count = 0;
static int output_level = 0;

static void
common_print(const char * format, ...);


int
common_get_error_count(void)
{
	return common_error_count;
}

/* error & warning */
void
common_error(const char *format, ...)
{
	va_list arg;
	FILE * output;
	output = stderr;

	common_error_count++;

	fprintf(output, "[ERROR %d]:\n\t", common_error_count);
	va_start(arg, format);
	vfprintf(output, format, arg);
	va_end(arg);

	fprintf(output, "\n");
	return;
}

void
common_error_line(int parse_line, const char *format, ...)
{
	va_list arg;
	LINE * line;
	FILE * output;
	output = stderr;

	common_error_count++;

	line = line_revert(parse_line);
	fprintf(output, "[ERROR %d @ \"%s : %d\" :\n\t", common_error_count, line->file, line->line);

	va_start(arg, format);
	vfprintf(output, format, arg);
	va_end(arg);

	fprintf(output, "\n");
	return;
}

void
common_warning(const char *format, ...)
{
	va_list arg;
	FILE * output;
	output = stderr;


	fprintf(output, "[WARNING]:\n\t");
	va_start(arg, format);
	vfprintf(output, format, arg);
	va_end(arg);

	fprintf(output, "\n");
	return;
}

/* get data-type enum from text */
SIM_DATA_TYPE
common_string_to_data_type(char *type_string)
{
	int i;

	if (type_string == NULL) {
		return SDT_UNKNOWN;
	}

	type_string[0] = toupper(type_string[0]);

	for(i = 0; __data_type_name_list[i]; i++){
		if(strcmp(type_string, __data_type_name_list[i]) == 0){
			return __data_type_e_list[i];
		}
	}

	return SDT_UNKNOWN;
}

char *
common_data_type_to_string(SIM_DATA_TYPE type)
{
	char * string;
	switch(type){
	case SDT_U32: string = "U32"; break;
	case SDT_U16: string = "U16"; break;
	case SDT_U08: string = "U08"; break;
	case SDT_I32: string = "I32"; break;
	case SDT_I16: string = "I16"; break;
	case SDT_I08: string = "I08"; break;
	case SDT_F32: string = "F32"; break;
	case SDT_F64: string = "F64"; break;
	case SDT_W01: string = "W01"; break;
	case SDT_UNKNOWN: string = "???"; break;
	default: string = "???"; break;
	}

	return string;
}



SIM_DIMENSION *
common_dimension_new(int count, int * width, int way, int way_low, int way_high)
{
	SIM_DIMENSION * dimension = (SIM_DIMENSION *)malloc(sizeof(SIM_DIMENSION));
	dimension->count = count;
	dimension->width = width;
	dimension->way = way;
	dimension->way_low = way_low;
	dimension->way_high = way_high;
	return dimension;
}

void
common_dimension_free(SIM_DIMENSION * dimension)
{
	free(dimension->width);
	free(dimension);
}

void
common_dimension_print(SIM_DIMENSION * dimension)
{
	int i;
	int count;
	if(dimension == NULL){
		return;
	}

	for(i = 0, count = dimension->count; i < count; i++){
		common_print("[%d]", (dimension->width)[i]);
	}

	if(dimension->way){
		common_print("[%d:%d]", dimension->way_low, dimension->way_high);
	}
}

char *
common_dimension_to_string(SIM_DIMENSION * dimension)
{
	return common_dimension_array_to_string(dimension->count, dimension->width);
}

char *
common_dimension_array_to_string(int count, int * array)
{
	char buffer[1024];
	char * temp = NULL;
	int i;


	for(i = 0; i < count; i++){
		sprintf(buffer, "[%d]", array[i]);
		if(temp){
			temp = util_join(temp, 1, buffer, 0);
		}else{
			temp = strdup(buffer);
		}
	}
	if(temp == NULL){
		return "";
	}
	return temp;
}



char **
common_dimension_split(SIM_DIMENSION * way, SIM_DIMENSION * dimension)
{
	char ** split_dims = NULL;
	int way_count;
	int dim_count = 0;
	int * dim_base = NULL;
	int * dim_temp = NULL;
	int * dim_edge = NULL;
	int i,j;
	int carry;
	int split_count;

	if(way == NULL){
		way = common_dimension_new(0, NULL, 0, 0, 0);
	}

	if(dimension == NULL){
		dimension = common_dimension_new(0, NULL, 0, 0, 0);

//		split_dims = (char**)malloc(2 * sizeof(char *));
//		split_dims[0] = "";
//		split_dims[1] = NULL;
//		return split_dims;
	}
	way_count = way->count;
	dim_count = dimension->count;

	dim_base = (int *)calloc(way_count + dim_count, (sizeof(int)));
	dim_temp = dim_base + way_count;
	dim_edge = (int *)malloc(dim_count * (sizeof(int)));

	for(i = 0; i < way_count; i++){
		dim_base[i] = way->width[i];
	}

	// set start and edge
	for(i = 0; i < dim_count; i++){
		dim_edge[i] = dimension->width[i];
	}

	// adjust start and edge
	if(way && way->way){
		dim_temp[0] = way->way_low;
		dim_edge[0] = way->way_high + 1;
	}

	// number count
	for(i = 0, split_count = 1; i < dim_count; i++){
		split_count *= (dim_edge[i] - dim_temp[i]);
	}

	// initial **
	split_dims = (char **)malloc((split_count + 1) * sizeof(char *));
	split_dims[split_count] = NULL;

	for(j = 0; j < split_count; j ++){
		split_dims[j] = common_dimension_array_to_string(dim_count + way_count, dim_base);

		/* way + 1 ......*/
		for(carry = 1, i = dim_count - 1; i >=0 && carry; i--){
			if(carry == 0){
				break;
			}
			if(dim_temp[i] + 1  == dim_edge[i]){
				dim_temp[i] = 0;
			}else{
				dim_temp[i] ++;
				carry = 0;
			}
		}
	}
	return split_dims;

}


SIM_ELEMENT *
common_element_new(SIM_ELEMENT_TYPE type, SIM_MODULE * father, SIM_DATA_TYPE data_type,
		char * module_type, char * name, SIM_DIMENSION * dimension)
{
	SIM_ELEMENT * e = (SIM_ELEMENT *)malloc(sizeof(SIM_ELEMENT));
	bzero(e, sizeof(SIM_ELEMENT));

	e->type = type;
	e->father = father;
	e->data_type = data_type;
	e->module_type = module_type;
	e->name = name;
	e->dimension = dimension;
	e->split_ways = common_dimension_split(NULL, dimension);
	return e;
}


void
common_element_free(SIM_ELEMENT * element)
{
	if(element->module_type){
		free(element->module_type);
	}

	if(element->name){
		free(element->name);
	}

	if(element->dimension->width){
		free(element->dimension->width);
	}

	free(element);
}


/* find a element in list by given name, return NULL no match */
SIM_ELEMENT *
common_element_list_find_name(SIM_ELEMENT_LIST * element_list, char *name)
{
	SIM_ELEMENT_LIST * temp;

	if(name == NULL){
		return NULL;
	}

	temp = element_list;
	while(temp){
		if(temp->element->name && strcmp(temp->element->name, name) == 0){
			return temp->element;
		}
		temp = temp->next;
	}

	/* no match. */
	return NULL;
}


/* print an element */
void
common_element_print(SIM_ELEMENT * element)
{
	char * data_type;
	common_print("\t\t|---[ELEMENT]--^[%s]--", element->father->name);

	switch(element->type){
	case ELEMENT_INPUT:
		common_print("[input]");
		break;
	case ELEMENT_OUTPUT:
		common_print("[output]");
		break;
	case ELEMENT_INOUT:
		common_print("[inout]");
		break;
	case ELEMENT_RAM:
		common_print("[ram]");
		break;
	case ELEMENT_CONTAIN:
		common_print("[contain]");
		break;
	case ELEMENT_UNKNOWN:
		common_print("[unknown]");
		break;
	}

	switch(element->type){
	case ELEMENT_INPUT:
	case ELEMENT_OUTPUT:
	case ELEMENT_INOUT:
	case ELEMENT_RAM:
		data_type = common_data_type_to_string(element->data_type);
		common_print("[%s]", data_type);

		break;
	case ELEMENT_CONTAIN:
		common_print("[%s]", element->module_type);

		break;
	case ELEMENT_UNKNOWN:
		common_print("[??]");

		break;
	}

	common_print("[\"%s\"]", element->name);

	common_dimension_print(element->dimension);
	common_print("------\n");
	return;
}


void
common_element_error(SIM_ELEMENT * element)
{
	output_level = 1;
	common_print("\t!ERROR @ [%s]'s ELEMENT:\n", element->father->name);
	common_element_print(element);
	output_level = 0;
}

void
common_element_list_append(SIM_ELEMENT_LIST ** phead, SIM_ELEMENT * element)
{
	SIM_ELEMENT_LIST *temp;
	SIM_ELEMENT_LIST *find;
	temp = (SIM_ELEMENT_LIST *) malloc(sizeof (SIM_ELEMENT_LIST));
	bzero(temp, sizeof (SIM_ELEMENT_LIST));
	temp->element = element;

	find = *phead;

	if (find) {
		while (find->next) {
			find = find->next;
		}
		find->next = temp;
	} else {
		*phead = temp;
	}
	return;
}


void
common_element_list_remove(SIM_ELEMENT_LIST ** phead, SIM_ELEMENT * node)
{
	SIM_ELEMENT_LIST *temp;
	SIM_ELEMENT_LIST *temp2;

	temp = *phead;
	if (temp->element == node) {
		(*phead) = (*phead)->next;
		common_element_free(temp->element);
		free(temp);

		return;
	}

	while (temp->next) {
		if (temp->next->element == node) {
			temp2 = temp->next;
			temp->next = temp2->next;
			common_element_free(temp2->element);
			free(temp2);

			return;
		}
		temp = temp->next;
	}
	return;
}

void
common_element_list_print(SIM_ELEMENT_LIST * head)
{
	SIM_ELEMENT_LIST * temp;
	temp = head;
	while(temp){
		if(temp->element){
			common_element_print(temp->element);
		}
		temp = temp->next;
	}
}



SIM_NET_ELEMENT *
common_net_element_new(SIM_NET_ELEMENT_TYPE type, SIM_MODULE * father,
		SIM_ELEMENT * sim_element, char * module_name, char * port_name, SIM_DIMENSION * way)
{
	SIM_NET_ELEMENT * ne = (SIM_NET_ELEMENT *)malloc(sizeof(SIM_NET_ELEMENT));
	bzero(ne, sizeof(SIM_NET_ELEMENT));

	ne->type = type;
	ne->father = father;
	ne->sim_element = sim_element;
	ne->module_name = module_name;
	ne->port_name = port_name;
//	ne->integer = integer;
//	ne->fp = fp;
	ne->way = way;

	return ne;
}

void
common_net_element_free(SIM_NET_ELEMENT * net_element)
{
	if(net_element->module_name){
		free(net_element->module_name);
	}

	if(net_element->port_name){
		free(net_element->port_name);
	}

	if(net_element->way){
		if(net_element->way->width){
			free(net_element->way->width);
		}
		free(net_element->way);
	}

	free(net_element);
}

void
common_net_element_print(SIM_NET_ELEMENT * net_element)
{
	if(net_element == NULL){
		common_print("<empty>");
		return;
	}

	common_print("<");
	switch(net_element->type){
	case NET_ELEMENT_UNKNOWN:
		common_print("[????]");
		break;
	case NET_ELEMENT_PORT:
		common_print("PORT");
		break;
	case NET_ELEMENT_WRITER:
		common_print("WRITER");
		break;
	case NET_ELEMENT_READER:
		common_print("READER");
		break;
	case NET_ELEMENT_INOUT:
		common_print("INOUT");
		break;
	case NET_ELEMENT_VALUE:
		common_print("VALUE");
		break;
	}

	common_print(":");

	switch(net_element->type){
	case NET_ELEMENT_UNKNOWN:
		common_print("??");
		break;
	case NET_ELEMENT_PORT:
	case NET_ELEMENT_WRITER:
	case NET_ELEMENT_READER:
	case NET_ELEMENT_INOUT:
		if(net_element->module_name){
			common_print("%s.", net_element->module_name);
		}
		if(net_element->port_name){
			common_print("%s", net_element->port_name);
		}
		common_dimension_print(net_element->way);
		if(net_element->way->count + net_element->way->way > 0){
			common_print(";");
		}
		common_dimension_print(net_element->dimension);
		break;
	case NET_ELEMENT_VALUE:
		common_print("\"%s\"", net_element->port_name);
		break;
	}
	common_print(">   ");
}
void
common_net_element_error(SIM_NET_ELEMENT * net_element)
{
	output_level = 1;
	common_print("\t!ERROR @ [%s]'s NET_ELEMENT:\n\t", net_element->father->name);
	common_net_element_print(net_element);
	common_print("\n");
	output_level = 0;
}

void
common_net_element_list_append(SIM_NET_ELEMENT_LIST ** phead, SIM_NET_ELEMENT * element)
{
	SIM_NET_ELEMENT_LIST * net_element_list;
	SIM_NET_ELEMENT_LIST * temp;
	net_element_list = (SIM_NET_ELEMENT_LIST *)calloc(1, sizeof(SIM_NET_ELEMENT_LIST));
	net_element_list->net_element = element;
	if(*phead == NULL){
		*phead = net_element_list;
	}else{
		temp = *phead;
		while(temp->next){
			temp = temp->next;
		}
		temp->next = net_element_list;
	}
}

void
common_net_element_list_print(SIM_NET_ELEMENT_LIST * net_element_list)
{
	common_print("\t\t|---[NET] { ");
	while(net_element_list){
		common_net_element_print(net_element_list->net_element);
		net_element_list =  net_element_list->next;
	}
	common_print(" };\n");
}

void
common_net_element_list_error(SIM_NET_ELEMENT_LIST * net_element_list)
{
	output_level = 1;
	common_print("\t!ERROR @ ELEMENT_LIST:\n");
	common_net_element_list_print(net_element_list);
	output_level = 0;
}

void
common_net_list_append_element_list(SIM_NET_LIST ** phead, SIM_NET_ELEMENT_LIST * element_list)
{
	SIM_NET_LIST * net_list;
	SIM_NET * net;
	SIM_NET_LIST * temp;
	net_list = (SIM_NET_LIST *)calloc(1, sizeof(SIM_NET_LIST));
	net = (SIM_NET *)calloc(1, sizeof(SIM_NET));
	net_list->net = net;
	net->net_element_list = element_list;

	if(*phead == NULL){
		*phead = net_list;
	}else{
		temp = *phead;
		while(temp->next){
			temp = temp->next;
		}
		temp->next = net_list;
	}
}

void
common_net_list_print(SIM_NET_LIST * net_list)
{
	while(net_list){
		common_net_print(net_list->net);
		net_list = net_list->next;
	}
}

void
common_net_print(SIM_NET * net)
{
	common_print("NET(%s)", net->type == NET_TYPE_BUS ? "BUS" : "LINKS");
	common_net_element_list_print(net->net_element_list);
}

SIM_MODULE *
common_module_new(char * name, int cycle, int behavior,
		SIM_ELEMENT_LIST * element_list, SIM_NET_LIST * net_list)
{
	SIM_MODULE * m = (SIM_MODULE *)malloc(sizeof(SIM_MODULE));
	bzero(m, sizeof(SIM_MODULE));

	m->name = name;
	m->cycle = cycle;
	m->behavior = behavior;
	m->element_list = element_list;
	m->net_list = net_list;

	return m;
}

void
common_module_free(SIM_MODULE * module)
{
}

void
common_module_print(SIM_MODULE * module)
{
	common_print("+---- module begin:%s\n", module->name);

	/* cycle */
	if (module->behavior) {
		common_print("#   ~cycle:%d\n", module->cycle);
	}

	/* behavior */
	common_print("# %s\n", module->behavior ? "behavior" : "structural");

	/* elements */
	common_element_list_print(module->element_list);

	/*  nets */
	common_net_list_print(module->net_list);

	common_print("----- module-end----\n\n");
}

/* find a module in list by given name, return NULL no match */
void
common_module_error(SIM_MODULE * module)
{
	output_level = 1;
	common_print("  !ERROR @ MODULE[%s]\n", module->name);
	output_level = 0;
}

SIM_MODULE *
common_module_list_find_name(SIM_MODULE_LIST * module_list, char *name)
{
	SIM_MODULE_LIST *temp;

	if (name == NULL) {
		return NULL;
	}

	temp = module_list;

	while (temp) {
		if (strcmp(temp->module->name, name) == 0) {
			return temp->module;
		}
		temp = temp->next;
	}

	return NULL;
}

void
common_module_list_append(SIM_MODULE_LIST ** phead, SIM_MODULE * module)
{
	SIM_MODULE_LIST * temp;

	if((*phead) == NULL){
		*phead = (SIM_MODULE_LIST *)malloc(sizeof(SIM_MODULE_LIST));
		bzero(*phead, sizeof(SIM_MODULE_LIST));
		(*phead)->module = module;
	} else {
		temp = *phead;

		while(temp->next){
			temp = temp->next;
		}

		temp->next = (SIM_MODULE_LIST *)malloc(sizeof(SIM_MODULE_LIST));
		temp = temp->next;
		temp->next = NULL;
		temp->module = module;
	}
}



void
common_module_list_print(SIM_MODULE_LIST * head)
{
	SIM_MODULE_LIST * temp;

	common_print("+------- module list begin --------\n");
	temp = head;
	while (temp) {
		common_module_print(temp->module);
		temp = temp->next;
	}
	common_print("-------- module list end --------\n");
	return;
}

void
common_module_list_free(SIM_MODULE_LIST * root)
{
	SIM_MODULE_LIST *temp;
	SIM_MODULE_LIST *temp2;
	while (temp) {
		temp2 = temp->next;
		common_module_free(temp->module);
		free(temp);
		temp = temp2;
	}
	return;
}

void
common_module_tree_print(SIM_MODULE_TREE * root)
{
	SIM_MODULE_TREE *temp = root->child;
	static int level = 1;
	int i;
	if (level == 1) {
		common_print("--------------\n");
	}
	i = level;

	common_print("[%6d]", root->id);
	common_print("[%6d]", root->sortid);
	while (i-- > 1) {
		common_print("\t");
	}
	common_print("|---");




	// info
	if(root->module->behavior){
		if(root->module->cycle == 0){
			common_print(" * ");
		}else{
			common_print(" + ");
		}
	}else{
		common_print(" ~ ");
	}
	common_print("%s\n", root->name);



	level++;
	while (temp) {
		common_module_tree_print(temp);
		temp = temp->brother;
	}
	level--;

	if (root->brother == NULL) {
		common_print("\t");
		i = level;
		while (i-- > 1) {
			common_print("\t");
		}
		common_print("`\n");

	}
	if (level == 1) {
		common_print("--------------\n");
	}
	return;
}


/**
 * use module's name if name == NULL
 * return the chlid
 */
SIM_MODULE_TREE *
common_module_tree_add_child(SIM_MODULE_TREE * father, SIM_MODULE * child, char *name)
{
	SIM_MODULE_TREE *temp;
	SIM_MODULE_TREE *node;

	node = (SIM_MODULE_TREE *) malloc(sizeof (SIM_MODULE_TREE));
	bzero(node, sizeof (SIM_MODULE_TREE));
	if(name){
		node->name = name;
	}else{
		node->name = strdup(child->name);
	}
	node->module = child;
	node->father = father;

	temp = father->child;
	if (temp) {
		while (temp->brother) {
			temp = temp->brother;
		}
		temp->brother = node;
	} else {
		father->child = node;
	}
	return node;
}

SIM_MODULE_TREE *
common_module_tree_find_node(SIM_MODULE_TREE * root, char *name)
{
	SIM_MODULE_TREE *temp;
	SIM_MODULE_TREE *result;
	temp = root;
	while (temp) {
		if (strcmp(name, temp->name) == 0) {
			return temp;
		}
		result = common_module_tree_find_node(temp->child, name);
		if (result) {
			return result;
		}
		temp = temp->brother;
	}
	return NULL;
}

SIM_MODULE *
common_module_tree_find_module(SIM_MODULE_TREE * root, char *name)
{
	SIM_MODULE_TREE *temp;
	SIM_MODULE *result;

	for (temp = root; temp; temp = temp->brother) {
		if (strcmp(name, temp->name) == 0) {
			return temp->module;
		}
		result = common_module_tree_find_module(temp->child, name);
		if (result) {
			return result;
		}
	}
	return NULL;
}

SIM_MODULE_TREE *
common_module_tree_find_by_id(SIM_MODULE_TREE * root, int id)
{
	SIM_MODULE_TREE *temp;
	SIM_MODULE_TREE *result;

	for (temp = root; temp; temp = temp->brother) {
		if (temp->id == id) {
			return temp;
		}
		result = common_module_tree_find_by_id(temp->child, id);
		if (result) {
			return result;
		}
	}

	return NULL;
}

SIM_MODULE_TREE *
common_module_tree_find_by_sortid(SIM_MODULE_TREE * root, int id)
{
	SIM_MODULE_TREE *temp;
	SIM_MODULE_TREE *result;

	for (temp = root; temp; temp = temp->brother) {
		if (temp->sortid == id) {
			return temp;
		}
		result = common_module_tree_find_by_sortid(temp->child, id);
		if (result) {
			return result;
		}
	}

	return NULL;
}


void
common_module_tree_free(SIM_MODULE_TREE * root)
{
	SIM_MODULE_TREE *t1;
	SIM_MODULE_TREE *t2;
	t1 = root;
	while (t1) {
		common_module_tree_free(t1->child);
		t2 = t1->brother;
		free(t1);
		t1 = t2;
	}
	return;
}

SIM_LINK *
common_link_list_find_dest(SIM_LINK_LIST * link_list, char * full_name)
{
	SIM_LINK_LIST * list;
	SIM_LINK * link;

	for(list = link_list; list; list = list->next){
		link = list->link;
		if(strcmp(full_name, link->dest_full_name) == 0){
			return link;
		}
	}

	return NULL;
}

void
common_link_print(SIM_LINK * link)
{
	//common_print("+---\n");
	if(link == NULL){
		return;
	}


//	common_print("src_module_node->name:[%s]\n", link->src_module_node->name);
//	common_print("src_module_name:[%s]\n", link->src_module_name);
//	common_print("src_module_type->name:[%s]\n", link->src_module_type->name);
//	common_print("src_port_type->name:[%s]\n", link->src_port_type->name);
//	common_print("src_port_way:[%s]\n", link->src_port_way);

//	common_print("dest_module_node->name:[%s]\n", link->dest_module_node->name);
//	common_print("dest_module_name:[%s]\n", link->dest_module_name);
//	common_print("dest_module_type->name:[%s]\n", link->dest_module_type->name);
//	common_print("dest_port_type->name:[%s]\n", link->dest_port_type->name);
//	common_print("dest_port_way:[%s]\n", link->dest_port_way);




//	common_print("%s.%s", link->src_module_name, link->src_port_type->name);
//	common_print("%s ==>> ", link->src_port_way);
//
//	common_print("%s.%s", link->dest_module_name, link->dest_port_type->name);
//	common_print("%s\n", link->dest_port_way);

//	common_print("|%s . %s", link->src_module_node->name, link->src_port_type->name);
//	common_print("%s  ==>>  ", link->src_port_way);
//
//	common_print("|%s . %s", link->dest_module_node->name, link->dest_port_type->name);
//	common_print("%s\n", link->dest_port_way);
	if(link->type == LINK_CONST){
		common_print("const");
	}
	common_print("%s ==>> %s\n", link->src_full_name, link->dest_full_name);
//	common_print("SRC(%s):", link->src_module_type->behavior?"b":"s");
//	common_print("DEST(%s)\n", link->dest_module_type->behavior?"b":"s");
	//common_print("----\n");
}

SIM_LINK_LIST *
common_link_list_append(SIM_LINK_LIST ** phead, SIM_LINK * link)
{
	SIM_LINK_LIST * new_node;
	SIM_LINK_LIST * temp;
	new_node = (SIM_LINK_LIST *)malloc(sizeof(SIM_LINK_LIST));
	new_node->next = NULL;
	new_node->link = link;

	if(*phead == NULL){
		*phead = new_node;
	}else{
		temp = *phead;
		while(temp->next){
			temp = temp->next;
		}
		temp->next = new_node;
	}

	return new_node;
}

void
common_link_list_remove(SIM_LINK_LIST ** phead, SIM_LINK * link)
{
	SIM_LINK_LIST * temp;
	SIM_LINK_LIST * head;
	if(phead == NULL){
		return;
	}

	if((*phead)->link == link){
		temp = *phead;
		*phead = (*phead)->next;
	}else{
		head = *phead;
		while(head->next){
			if(head->next->link == link){
				temp = head->next;
				head->next = temp->next;
				break;
			}
			head = head->next;
		}
	}

	free(link);
	free(temp);

}

void
common_link_list_print(SIM_LINK_LIST * link_list)
{
	SIM_LINK_LIST * temp;
	for(temp = link_list; temp; temp = temp->next){
		common_link_print(temp->link);
	}
}

static void
common_print(const char * format, ...)
{
	va_list arg;
	if (config_get_verbose() == 0 && output_level == 0) {
		return;
	}

	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}
