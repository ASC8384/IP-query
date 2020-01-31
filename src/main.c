#include "ini_load.h"
#include "ip_match.h"
#include "iterator.h"
#include "parse_input.h"
#include <stdio.h>
#include <time.h>

void putout_ip_msg(const show_msg *is_show, const unsigned long cnt, const ip *want,
				   const ip_msg *pos, const unsigned long timer) {
	printf("[%lu]: %d.%d.%d.%d ", cnt, want->ip[1], want->ip[2], want->ip[3], want->ip[4]);
	if(is_show->country) {
		for(int i = 1; i <= pos->country_num; i++)
			putchar(pos->country[i]);
		putchar(' ');
	}
	if(is_show->province) {
		for(int i = 1; i <= pos->province_num; i++)
			putchar(pos->province[i]);
		putchar(' ');
	}
	if(is_show->city) {
		for(int i = 1; i <= pos->city_num; i++)
			putchar(pos->city[i]);
		putchar(' ');
	}
	if(is_show->isp) {
		for(int i = 1; i <= pos->isp_num; i++)
			putchar(pos->isp[i]);
		putchar(' ');
	}
	printf("| query time = %ld ms\n", timer / 1000);
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

	// process ini
	ini_name = "ip-query.ini";
	ini		 = iniparser_load(ini_name);
	if(ini == NULL) {
		iniparser_init(ini_name, "[show]\n[sort]\n[file]\n", &ini);
	}
	iniparser_get_is_show(ini, &is_show);

	printf("Welcome to IP-query!");
	file_ip = fopen("C:/Code/IP-query/src/common/ip.txt", "r");

	unsigned long cnt = 0;
	while(1) {
		printf("\nPlease input the IP address: ");

		// process the putin_string
		char	  putin[256];
		iterator *itor = NULL;
	fail_parse_input:
		// get input
		fgets(putin, 256, stdin);
		switch(parse_input(putin)) {
		case 'y':
			itor = parse_ip(putin);
			break;
		case 'f':
			parse_file(putin);
			itor = parse_ip(putin);
			break;
		case 'q':
			goto main_exit;
			break;
		default:
			printf("Input error!\nPlease check up and input the IP address again: ");
			goto fail_parse_input;
		}

		// match and print ip massage
		putchar('\n');
		do {
			want = (ip *)get_next(itor);
			mingw_gettimeofday(&func_start, NULL);
			pos = match_ip(want, file_ip);
			mingw_gettimeofday(&func_end, NULL);
			unsigned long timer = 1000000 * (func_end.tv_sec - func_start.tv_sec) +
								  func_end.tv_usec - func_start.tv_usec;
			putout_ip_msg(&is_show, ++cnt, want, &pos, timer);
		} while(has_next(itor));
		iterator_free(itor);
	}

// end of ip-query
main_exit : {
	iniparser_save(ini, ini_name);
	iniparser_freedict(ini);
	fclose(file_ip);
}
	return 0;
}
