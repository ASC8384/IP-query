#ifndef PAESE_INPUT_H_
#define PAESE_INPUT_H_

#include <stdio.h>

struct IP {
	int ip[5];
};
typedef struct IP ip;

char *input_string(char *des);

ip parse(const char *source);

#endif