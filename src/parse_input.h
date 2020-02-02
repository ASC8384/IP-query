#ifndef PAESE_INPUT_H_
#define PAESE_INPUT_H_

#include "iterator.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short int_2;
typedef struct IP {
	// ip[4] == ip[0]
	int_2	  ip[4];
	struct IP *next;
} ip;

typedef struct ip_list_inner_itor {
	ip *item;
} inner_itor;

// attend ip list
bool IP_LIST_INSERT(ip *item, ip *list);

static bool  ip_list_has_next(void *container_instance, void *container_inner_itor);
static void *ip_list_get_next(void *container_instance, void *container_inner_itor);

char *	parse_file(char *des);
char	  parse_input(const char *putin);
iterator *parse_ip(char *source);

#endif