#ifndef PAESE_INPUT_H_
#define PAESE_INPUT_H_

#include "iterator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ip_list_t ip

typedef struct IP {
	int		   ip[5];
	struct IP *next;
} ip;

typedef struct ip_list_inner_itor {
	ip *item;
} inner_itor;

bool IP_LIST_INSERT(ip *list, ip item);

static bool  ip_list_has_next(void *container_instance, void *container_inner_itor);
static void *ip_list_get_next(void *container_instance, void *container_inner_itor);

char *	input_string(char *des);
iterator *parse(const char *source);

#endif