#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

struct IP {
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

int main() {
	struct timeval func_start;
	struct timeval func_end;
	unsigned long  timer;

	char putin[1024];
	int  ip[5];

	printf("Welcome to IP-query!\n");
	scanf("%s", putin);
	sscanf(putin, "%d.%d.%d.%d", ip + 1, ip + 2, ip + 3, ip + 4);

	FILE *fp;
	fp = fopen("common/ip.txt", "r");

	mingw_gettimeofday(&func_start, NULL);
	struct IP pos;
	int		  c;
	memset(&pos, 0, sizeof(pos));
	for(int i = 1; i <= 4; i++)
		while(isdigit(c = fgetc(fp))) {
			pos.ip_start[i] = pos.ip_start[i] * 10 + c - '0';
		}
	for(int i = 1; i <= 4; i++)
		while(isdigit(c = fgetc(fp))) {
			pos.ip_end[i] = pos.ip_end[i] * 10 + c - '0';
		}
	while((c = fgetc(fp)) != 0x7C)
		pos.country[++pos.country_num] = c;
	c = fgetc(fp);
	c = fgetc(fp);
	while((c = fgetc(fp)) != 0x7C)
		pos.province[++pos.province_num] = c;
	while((c = fgetc(fp)) != 0x7C)
		pos.city[++pos.city_num] = c;
	while((c = fgetc(fp)) != 0x0D && c != 0x0A)
		pos.isp[++pos.isp_num] = c;
	c = fgetc(fp);

	while((pos.ip_start[1] <= ip[1] && ip[1] <= pos.ip_end[1] &&
		   pos.ip_start[2] <= ip[2] && ip[2] <= pos.ip_end[2] &&
		   pos.ip_start[3] <= ip[3] && ip[3] <= pos.ip_end[3] &&
		   pos.ip_start[4] <= ip[4] && ip[4] <= pos.ip_end[4]) == 0) {
		memset(&pos, 0, sizeof(pos));
		for(int i = 1; i <= 4; i++)
			while(isdigit(c = fgetc(fp))) {
				pos.ip_start[i] = pos.ip_start[i] * 10 + c - '0';
			}
		for(int i = 1; i <= 4; i++)
			while(isdigit(c = fgetc(fp))) {
				pos.ip_end[i] = pos.ip_end[i] * 10 + c - '0';
			}
		while((c = fgetc(fp)) != 0x7C)
			pos.country[++pos.country_num] = c;
		c = fgetc(fp);
		c = fgetc(fp);
		while((c = fgetc(fp)) != 0x7C)
			pos.province[++pos.province_num] = c;
		while((c = fgetc(fp)) != 0x7C)
			pos.city[++pos.city_num] = c;
		while((c = fgetc(fp)) != 0x0D && c != 0x0A)
			pos.isp[++pos.isp_num] = c;
		c = fgetc(fp);
	}
	printf("%d.%d.%d.%d ", ip[1], ip[2], ip[3], ip[4]);
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

	mingw_gettimeofday(&func_end, NULL);
	fclose(fp);
	timer = 1000000 * (func_end.tv_sec - func_start.tv_sec) + func_end.tv_usec -
			func_start.tv_usec;
	printf("query time = %ld ms\n", timer / 1000);

	return 0;
}
