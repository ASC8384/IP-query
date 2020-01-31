#include "ip_match.h"

bool IS_MATCH(const ip *want, const ip_msg *pos) {
	return (pos->ip_start[1] <= want->ip[1] && want->ip[1] <= pos->ip_end[1] &&
			pos->ip_start[2] <= want->ip[2] && want->ip[2] <= pos->ip_end[2] &&
			pos->ip_start[3] <= want->ip[3] && want->ip[3] <= pos->ip_end[3] &&
			pos->ip_start[4] <= want->ip[4] && want->ip[4] <= pos->ip_end[4]);
}

ip_msg NEXT_IP(FILE *fp) {
	int	c;
	ip_msg pos;
	memset(&pos, 0, sizeof(pos));

	for(int i = 1; i <= 4; i++)
		while(isdigit(c = fgetc(fp)))
			pos.ip_start[i] = pos.ip_start[i] * 10 + c - '0';
	for(int i = 1; i <= 4; i++)
		while(isdigit(c = fgetc(fp)))
			pos.ip_end[i] = pos.ip_end[i] * 10 + c - '0';
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

	return pos;
}

ip_msg match_ip(const ip *want, FILE *fp) {
	ip_msg pos;
	char   c;

	fseek(fp, 0L, SEEK_SET);
	do {
		pos = NEXT_IP(fp);
	} while(!IS_MATCH(want, &pos));

	return pos;
}