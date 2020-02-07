#include "ip_match.h"
#define buffer(x) buffer[x].c

bool IS_MATCH(const ip *want, const ip_msg *pos) {
	return (pos->ip_start[1] <= want->ip[1] && want->ip[1] <= pos->ip_end[1] &&
			pos->ip_start[2] <= want->ip[2] && want->ip[2] <= pos->ip_end[2] &&
			pos->ip_start[3] <= want->ip[3] && want->ip[3] <= pos->ip_end[3] &&
			pos->ip_start[0] <= want->ip[0] && want->ip[0] <= pos->ip_end[0]);
}

ip_msg NEXT_IP(ipdb *buffer, unsigned int len) {
	int	c;
	ip_msg pos;
	memset(&pos, 0, sizeof(pos));

	int cnt = -1;

	while((c = buffer(++cnt)) != 0x7C)
		pos.country[pos.country_num++] = c;

	c = buffer(++cnt);
	c = buffer(++cnt);

	while((c = buffer(++cnt)) != 0x7C)
		pos.province[pos.province_num++] = c;
	while((c = buffer(++cnt)) != 0x7C)
		pos.city[pos.city_num++] = c;
	while((c = buffer(++cnt)) != 0x0D && cnt < len - 1)
		pos.isp[pos.isp_num++] = c;

	return pos;
}

unsigned int get_int(const ipdb *buffer, int offset) {
	return (((buffer(offset)) & 0x000000FF) | ((buffer(offset + 1) << 8) & 0x0000FF00) |
			((buffer(offset + 2) << 16) & 0x00FF0000) | ((buffer(offset + 3) << 24) & 0xFF000000));
}

ip_msg match_ip(const ip *want, const ipdb *fp) {
	ip_msg		 pos;
	char		 c;
	unsigned int ipval =
		(want->ip[0] << 24) + (want->ip[1] << 16) + (want->ip[2] << 8) + (want->ip[3]);
	unsigned int first_index = get_int(fp, 0) + 8;
	unsigned int last_index  = get_int(fp, 4) + 8;
	unsigned int total_block = (last_index - first_index) / 8 + 1;

	unsigned int left  = 0;
	unsigned int right = total_block;
	unsigned int data  = 0;
	ipdb *		 buffer;
	while(left <= right) {
		unsigned int mid = (left + right) / 2;
		unsigned int pos = first_index + mid * 8;
		buffer			 = fp + pos;

		unsigned int cur_ip = get_int(buffer, 0);

		if(ipval < cur_ip) {
			right = mid - 1;
		} else {
			unsigned int cur_ip_next = get_int(buffer, 8);
			if(ipval < cur_ip_next) {
				data = get_int(buffer, 4);
				break;
			} else {
				left = mid + 1;
			}
		}
	}
	// ip >= 224.0.0.0
	if(data == 0)
		data = get_int(fp + first_index + 8, 0);
	unsigned int data_pos = data & 0x00ffffff;
	unsigned int data_len = (data >> 24) & 0x000000ff;
	buffer				  = fp + data_pos + 8;

	pos = NEXT_IP(buffer, data_len);

	return pos;
}