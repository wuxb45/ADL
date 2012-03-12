#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"


/**
 *  join two char strings, into one string, freeN parameters indicates that if
 *  the corresponding string should be "free()".
 */
char *
util_join(char *string1, int free1, char *string2, int free2)
{
	int size;
	char *result;
//	if(string1 == NULL){
//		string1 = "";
//		free1 = 0;
//	}
//
//	if(string2 == NULL){
//		string2 = "";
//		free2 = 0;
//	}

	size = strlen(string1) + strlen(string2) + 1;
	result = (char *) malloc(sizeof (char) * size);
	strcpy(result, string1);
	strcat(result, string2);
	if (free1) {
		free(string1);
	}

	if (free2) {
		free(string2);
	}

	return result;
}

/**
 *  delete the "space" charactors at the end of string,
 *  and return the pointer of the first non-space charactor.
 */

char *
util_trim(char *buffer)
{
	int i;
	while (isspace(*buffer))
		buffer++;

	i = strlen(buffer);
	i--;

	while (i >= 0) {
		if (isspace(buffer[i])) {
			buffer[i] = 0;
			i--;
		} else {
			break;
		}
	}

	return buffer;
}

char *
util_trim_front(char *buffer)
{
	while (isspace(*buffer) && (*buffer) != '\n') {
		buffer++;
	}
	return buffer;
}

char *
util_split_first(const char *original, char c)
{
	char *split;
	char *dot;

	split = strdup(original);
	dot = strchr(split, c);
	if (dot == NULL) {
		free(split);
		return NULL;
	} else {
		*dot = '\0';
		return split;
	}

}

char *
util_split_last(const char *original, char c)
{
	char *split;
	char *dot;
	char *split2;

	split = strdup(original);
	dot = strrchr(split, c);

	if(dot == NULL){
		return split;
	}else{
		dot++;
		split2 = strdup(dot);
		free(split);
		return split2;
	}
}
