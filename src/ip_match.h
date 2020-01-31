#ifndef IP_MATCH_H_
#define IP_MATCH_H_

#include "parse_input.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

struct IpMessage {
	int ip_start[5];
	int ip_end[5];
	int country[23];
	int country_num;
	int province[23];
	int province_num;
	int city[23];
	int city_num;
	int isp[23];
	int isp_num;
};
typedef struct IpMessage ip_msg;

bool   IS_MATCH(const ip *want, const ip_msg *pos);
ip_msg NEXT_IP(FILE *fp);

ip_msg match_ip(const ip *want, FILE *fp);

#endif