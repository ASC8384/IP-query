#ifndef IP_MATCH_H_
#define IP_MATCH_H_

#include "parse_input.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned char  int_1;
typedef unsigned short int_2;

typedef struct ipdatabase {
	unsigned char c;
} ipdb;

typedef struct IpMessage {
	// ip[4] == ip[0]
	int_2		  ip_start[4];
	int_2		  ip_end[4];
	unsigned char country[20];
	unsigned char province[20];
	unsigned char city[20];
	unsigned char isp[20];
	int_1		  country_num;
	int_1		  province_num;
	int_1		  city_num;
	int_1		  isp_num;
} ip_msg;

bool   IS_MATCH(const ip *want, const ip_msg *pos);
ip_msg NEXT_IP(const ipdb *buffer, unsigned int len);

ip_msg match_ip(const ip *want, const ipdb *fp);

#endif