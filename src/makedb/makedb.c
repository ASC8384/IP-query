#include "map.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int getUnsignedInt(const char *buffer, int offset) {
	return (((buffer[offset]) & 0x000000FF) | ((buffer[offset + 1] << 8) & 0x0000FF00) |
			((buffer[offset + 2] << 16) & 0x00FF0000) | ((buffer[offset + 3] << 24) & 0xFF000000));
}

unsigned int ip2long(const char *ip) {
	int			 i = 0, p = 24;
	char		 buffer[4];
	const char * cs	= ip;
	unsigned int ipval = 0;

	while(*cs != '\0') {
		if(*cs == ' ') {
			cs++;
			continue;
		}
		if(*cs == '.') {
			// single part length limit
			if(i > 3) {
				ipval = 0;
				break;
			}

			if(p < 0)
				break;
			buffer[i] = '\0';
			ipval |= (atoi(buffer) << p);
			p -= 8;
			i = 0;
		} else {
			buffer[i++] = *cs;
		}

		cs++;
	}

	// append the rest parts
	if(i > 3)
		return 0;
	buffer[i] = '\0';
	ipval |= atoi(buffer);

	return ipval;
}

char *read_all_file(const char *filename) {
	char *text = NULL;
	FILE *fp   = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	size_t file_len;
	file_len = ftell(fp);
	// text	 = calloc(file_len, sizeof(unsigned char));
	text = (char *)malloc(file_len + 1);
	rewind(fp);
	if(fread(text, sizeof(char), file_len, fp) != file_len)
		return NULL;
	text[file_len] = '\0';
	fclose(fp);
	return text;
}

void *get_next_info(unsigned int *offset, unsigned int *ip_num, const char *text, char *ret) {
	// start ip
	unsigned int length = strchr(text + *offset, '|') - text - *offset;
	// char ret[40];
	strncpy(ret, text + *offset, length);
	*ip_num = ip2long(ret);
	*offset += length;
	// end ip
	*offset += strchr(text + *offset + 1, '|') - text - *offset;
	//
	length = strchr(text + *offset + 1, '\n') - text - *offset - 1;
	strncpy(ret, text + *offset + 1, length);
	*offset += length + 2;
	// return ret;
}

/*
 * 索引区：index block
 * +------------+-----------+
 * | 4bytes		| 4bytes	|
 * |ip_start, data_position(3 bytes) & data_len(1 bytes)
 * +------------+-----------+
 */
void putout_ip(const unsigned int index, const unsigned int ip, const unsigned char data_len,
			   FILE *fp) {
	fwrite(&ip, sizeof(ip), 1, fp);
	fwrite(&index, sizeof(index), 1, fp);
	fseek(fp, -1, SEEK_CUR);
	fwrite(&data_len, 1, 1, fp);
}

/*
 * 记录区：data
 * +------------+-----------+
 * |  dynamic length		|
 * country, province, city, isp
 * +------------+-----------+
 */
void putout_info(const char *info, FILE *fp) { fwrite(info, strlen(info), 1, fp); }

void make_info(const char *name_source, const char *name_info, const char *name_ip) {
	FILE *file_info = fopen(name_info, "wb");
	FILE *file_ip   = fopen(name_ip, "wb");

	unsigned int index  = 0;
	unsigned int offset = 0;
	char		 line[100];
	char *		 file_source = read_all_file(name_source);
	root_t		 tree		 = RB_ROOT;
	unsigned int ip			 = 0;
	size_t		 len		 = strlen(file_source);
	unsigned int tmptmp		 = 0;
	int			 size		 = 0;

	while(offset < len) {
		tmptmp++;
		if(tmptmp > 586177)
			break;

		memset(line, 0, sizeof(line));
		get_next_info(&offset, &ip, file_source, line);
		map_t *info = get(&tree, line);

		if(info == NULL) {
			size++;
			// 获取ip_info文件指针当前位置
			index = ftell(file_info);
			char str[12];
			itoa(index, str, 10);
			put(&tree, line, str);
			putout_info(line, file_info);
			putout_ip(index, ip, strlen(line), file_ip);
		} else {
			index = atoi(info->val);
			putout_ip(index, ip, strlen(line), file_ip);
		}
	}

	free(file_source);
	fclose(file_info);
	fclose(file_ip);
	map_free(tree);
}

/*
 * 文件头：file header(8 bytes)
 * +------------+-----------+
 * | 4 bytes	| 4 bytes   |
 * first index position, last index position
 * +------------+-----------+
 *
 * 记录区：data
 * +------------+-----------+
 * |  dynamic length		|
 * country, province, city, isp
 * +------------+-----------+
 *
 * 索引区：index block
 * +------------+-----------+
 * | 4bytes		| 4bytes	|
 * |ip_start, data_position(3 bytes) & data_len(1 bytes)
 * +------------+-----------+
 */
void makedb(const char *name_data, const char *name_index, FILE *fp) {
	char *data = read_all_file(name_data);

	struct ip_index {
		char c;
	} * index;

	FILE *file_index = fopen(name_index, "rb");
	fseek(file_index, 0, SEEK_END);
	unsigned int index_len = ftell(file_index);
	rewind(file_index);

	index = malloc(index_len);
	if(fread(index, sizeof(char), index_len, file_index) != index_len)
		return;
	fclose(file_index);

	int first_index = strlen(data);
	int last_index  = index_len + first_index;
	fwrite(&first_index, sizeof(first_index), 1, fp);
	fwrite(&last_index, sizeof(last_index), 1, fp);
	fwrite(data, strlen(data), 1, fp);
	fwrite(index, index_len, 1, fp);
	free(data);
	free(index);
	fclose(fp);
}

int main() {
	char *name_source = "../common/ip.txt";
	char *name_info   = "../common/ip_info";
	char *name_ip	 = "../common/ip_simplify";
	char *name_db	 = "../common/ip.db";

	make_info(name_source, name_info, name_ip);
	FILE *file_db = fopen(name_db, "wb");
	makedb(name_info, name_ip, file_db);

	return 0;
}