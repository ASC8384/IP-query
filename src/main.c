#include "ip_match.h"
#include "parse_input.h"
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
	struct timeval func_start;
	struct timeval func_end;
	unsigned long  timer;

	char   putin[256];
	int	c;
	ip	 want;
	FILE * fp;
	ip_msg pos;

	fp = fopen("common/ip.txt", "r");
	printf("Welcome to IP-query!\n");
	want = parse(input_string(putin));

	mingw_gettimeofday(&func_start, NULL);
	pos = match_ip(want, fp);
	mingw_gettimeofday(&func_end, NULL);

	timer = 1000000 * (func_end.tv_sec - func_start.tv_sec) + func_end.tv_usec -
			func_start.tv_usec;
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
	putchar('\n');
	printf("query time = %ld ms\n", timer / 1000);

	fclose(fp);

	return 0;
}
