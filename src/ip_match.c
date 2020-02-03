#include "ip_match.h"

bool IS_MATCH(const ip *want, const ip_msg *pos) {
	return (pos->ip_start[1] <= want->ip[1] && want->ip[1] <= pos->ip_end[1] &&
			pos->ip_start[2] <= want->ip[2] && want->ip[2] <= pos->ip_end[2] &&
			pos->ip_start[3] <= want->ip[3] && want->ip[3] <= pos->ip_end[3] &&
			pos->ip_start[0] <= want->ip[0] && want->ip[0] <= pos->ip_end[0]);
}

ip_msg NEXT_IP(char *fp, unsigned long long *cnt) {
	int	c;
	ip_msg pos;
	memset(&pos, 0, sizeof(pos));

	for(int i = 0; i <= 3; i++)
		while(isdigit(c = fp[++(*cnt)]))
			pos.ip_start[i] = pos.ip_start[i] * 10 + c - '0';
	for(int i = 0; i <= 3; i++)
		while(isdigit(c = fp[++(*cnt)]))
			pos.ip_end[i] = pos.ip_end[i] * 10 + c - '0';
	while((c = fp[++(*cnt)]) != 0x7C)
		pos.country[++pos.country_num] = c;
	c = fp[++(*cnt)];
	c = fp[++(*cnt)];
	while((c = fp[++(*cnt)]) != 0x7C)
		pos.province[++pos.province_num] = c;
	while((c = fp[++(*cnt)]) != 0x7C)
		pos.city[++pos.city_num] = c;
	while((c = fp[++(*cnt)]) != 0x0D && c != 0x0A)
		pos.isp[++pos.isp_num] = c;
	c = fp[++(*cnt)];

	return pos;
}

ip_msg match_ip(const ip *want, char *fp) {
	ip_msg pos;
	char   c;

	// fseek(fp, 0L, SEEK_SET);
	unsigned long long i = -1;
	do {
		pos = NEXT_IP(fp, &i);
	} while(!IS_MATCH(want, &pos));

	return pos;
}