#ifndef IP_MATCH_H_
#define IP_MATCH_H_

#include "parse_input.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned char  int_1;
typedef unsigned short char_2;

typedef struct IpMessage {
	int	ip_start[5];
	int	ip_end[5];
	char_2 country[23];
	int_1  country_num;
	char_2 province[23];
	int_1  province_num;
	char_2 city[23];
	int_1  city_num;
	char_2 isp[23];
	int_1  isp_num;
} ip_msg;

bool   IS_MATCH(const ip *want, const ip_msg *pos);
ip_msg NEXT_IP(FILE *fp);

ip_msg match_ip(const ip *want, FILE *fp);

#endif