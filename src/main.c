#include "ip_match.h"
#include "parse_input.h"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void putout_ip_msg(ip want, ip_msg pos, unsigned long timer) {
	printf("%d.%d.%d.%d ", want.ip[1], want.ip[2], want.ip[3], want.ip[4]);
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

int main() {
	struct timeval   func_start;
	struct timeval   func_end;
	static iterator *itor;
	unsigned long	timer;

	int	c;
	char   putin[256];
	ip *   want;
	FILE * fp;
	ip_msg pos;

	// fp = fopen("../src/common/ip.txt", "r");
	fp = fopen("C:/Code/IP-query/src/common/ip.txt", "r");
	printf("Welcome to IP-query!\n");
	itor = parse(input_string(putin));
	putchar('\n');

	do {
		want = (ip *)get_next(itor);
		mingw_gettimeofday(&func_start, NULL);
		pos = match_ip(*want, fp);
		mingw_gettimeofday(&func_end, NULL);
		timer =
			1000000 * (func_end.tv_sec - func_start.tv_sec) + func_end.tv_usec - func_start.tv_usec;
		putout_ip_msg(*want, pos, timer);
	} while(has_next(itor));

	iterator_free(itor);
	fclose(fp);

	return 0;
}
