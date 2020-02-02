#ifndef IP_MATCH_H_
#define IP_MATCH_H_

#include "parse_input.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned char  int_1;
typedef unsigned short char_2;
typedef unsigned short int_2;

typedef struct IpMessage {
	// ip[4] == ip[0]
	int_2  ip_start[4];
	int_2  ip_end[4];
	char_2 country[20];
	char_2 province[20];
	char_2 city[20];
	char_2 isp[20];
	int_1  country_num;
	int_1  province_num;
	int_1  city_num;
	int_1  isp_num;
} ip_msg;

bool   IS_MATCH(const ip *want, const ip_msg *pos);
ip_msg NEXT_IP(FILE *fp);

ip_msg match_ip(const ip *want, FILE *fp);

#endif