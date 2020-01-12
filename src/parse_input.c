#include "parse_input.h"

char *input_string(char *des) {
	char  c;
	char *r = des;
	scanf("%s", r);
	return des;
}

ip parse(const char *source) {
	ip ret;
	sscanf(source, "%d.%d.%d.%d", &ret.ip[1], &ret.ip[2], &ret.ip[3],
		   &ret.ip[4]);
	return ret;
}
