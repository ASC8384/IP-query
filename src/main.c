#include "iniparser.h"
#include "ip_match.h"
#include "iterator.h"
#include "parse_input.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct show_msg {
	int country, province, city, isp;
} show_msg;

void putout_ip_msg(show_msg *is_show, unsigned long cnt, ip want, ip_msg pos, unsigned long timer) {
	printf("[%lu]: %d.%d.%d.%d ", cnt, want.ip[1], want.ip[2], want.ip[3], want.ip[4]);
	if(is_show->country) {
		for(int i = 1; i <= pos.country_num; i++)
			putchar(pos.country[i]);
		putchar(' ');
	}
	if(is_show->province) {
		for(int i = 1; i <= pos.province_num; i++)
			putchar(pos.province[i]);
		putchar(' ');
	}
	if(is_show->city) {
		for(int i = 1; i <= pos.city_num; i++)
			putchar(pos.city[i]);
		putchar(' ');
	}
	if(is_show->isp) {
		for(int i = 1; i <= pos.isp_num; i++)
			putchar(pos.isp[i]);
		putchar(' ');
	}
	printf("| query time = %ld ms\n", timer / 1000);
}

int iniparser_save(dictionary *d, const char *inipath) {
	int   ret = 0;
	FILE *fp  = NULL;

	if(inipath == NULL || d == NULL) {
		ret = -1;
		printf("saveConfig error:%d from (filepath == NULL || head == NULL)\n", ret);
		return ret;
	}

	fp = fopen(inipath, "w");
	if(fp == NULL) {
		ret = -2;
		printf("saveConfig:open file error:%d from %s\n", ret, inipath);
		return ret;
	}

	iniparser_dump_ini(d, fp);

	fclose(fp);

	return 0;
}

int main(int agrc, char *argv[]) {
	struct timeval func_start;
	struct timeval func_end;
	dictionary *   ini		= NULL;
	char *		   ini_name = NULL;
	FILE *		   file_ip  = NULL;
	ip *		   want		= NULL;
	ip_msg		   pos;
	show_msg	   is_show;

	ini_name = "ip-query.ini";
	ini		 = iniparser_load(ini_name);
	if(ini == NULL) {
		FILE *file_ini = fopen(ini_name, "w");
		fprintf(file_ini, "[show]");
		fclose(file_ini);
		ini = iniparser_load(ini_name);
	}
	if(!iniparser_find_entry(ini, "show:country"))
		iniparser_set(ini, "show:country", "1");
	if(!iniparser_find_entry(ini, "show:province"))
		iniparser_set(ini, "show:province", "1");
	if(!iniparser_find_entry(ini, "show:city"))
		iniparser_set(ini, "show:city", "1");
	if(!iniparser_find_entry(ini, "show:isp"))
		iniparser_set(ini, "show:isp", "1");
	is_show.country  = iniparser_getboolean(ini, "show:country", 1);
	is_show.province = iniparser_getboolean(ini, "show:province", 1);
	is_show.city	 = iniparser_getboolean(ini, "show:city", 1);
	is_show.isp		 = iniparser_getboolean(ini, "show:isp", 1);

	printf("Welcome to IP-query!");
	file_ip = fopen("C:/Code/IP-query/src/common/ip.txt", "r");

	unsigned long cnt = 0;
	while(1) {
		char putin[256];

		printf("\nPlease input the IP address: ");
		input_string(putin);
		if(strchr(putin, 'q'))
			goto gotohere;
		iterator *itor = parse(putin);
		while(itor == NULL) {
			printf("Input error!\nPlease check up and input the IP address again: ");
			input_string(putin);
			if(strchr(putin, 'q'))
				goto gotohere;
			itor = parse(putin);
		}

		putchar('\n');
		do {
			want = (ip *)get_next(itor);
			mingw_gettimeofday(&func_start, NULL);
			pos = match_ip(*want, file_ip);
			mingw_gettimeofday(&func_end, NULL);
			unsigned long timer = 1000000 * (func_end.tv_sec - func_start.tv_sec) +
								  func_end.tv_usec - func_start.tv_usec;
			putout_ip_msg(&is_show, ++cnt, *want, pos, timer);
		} while(has_next(itor));
		iterator_free(itor);
	}

gotohere : {
	iniparser_save(ini, ini_name);
	iniparser_freedict(ini);
	fclose(file_ip);
}

	return 0;
}
