#include "ip_match.h"
#include "parse_input.h"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void putout_ip_msg(unsigned long long cnt, ip want, ip_msg pos, unsigned long timer) {
	printf("[%llu]: %d.%d.%d.%d ", cnt, want.ip[1], want.ip[2], want.ip[3], want.ip[4]);
	for(int i = 1; i <= pos.country_num; i++)
		putchar(pos.country[i]);
	putchar(' ');
	for(int i = 1; i <= pos.province_num; i++)
		putchar(pos.province[i]);
	putchar(' ');
	for(int i = 1; i <= pos.city_num; i++)
		putchar(pos.city[i]);
	putchar(' ');
	for(int i = 1; i <= pos.isp_num; i++)
		putchar(pos.isp[i]);
	// putchar('\n');
	printf("| query time = %ld ms\n", timer / 1000);
}

int main(int agrc, char *argv[]) {
	struct timeval func_start;
	struct timeval func_end;

	FILE * fp;
	ip *   want;
	ip_msg pos;

	unsigned long long cnt = 0;

	fp = fopen("C:/Code/IP-query/src/common/ip.txt", "r");
	printf("Welcome to IP-query!");

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
			pos = match_ip(*want, fp);
			mingw_gettimeofday(&func_end, NULL);
			unsigned long timer = 1000000 * (func_end.tv_sec - func_start.tv_sec) +
								  func_end.tv_usec - func_start.tv_usec;
			putout_ip_msg(++cnt, *want, pos, timer);
		} while(has_next(itor));
		iterator_free(itor);
	}

gotohere:
	fclose(fp);

	return 0;
}
