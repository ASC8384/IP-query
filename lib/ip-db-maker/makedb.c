/**
 * @file 		makedb.c
 * @brief 		构造 ip.db 数据库
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.0.0
 * @date       2020-02-09
 */

#include "map.h"
#include "rbtree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 将一个 IP 字符串转为 10 进制数
 * @param ip 字符串，可接受空格
 * @return uint32_t
 */
uint32_t ip2uint32(const char *ip) {
	int32_t		r = 24; // 偏移量
	uint32_t	i = 0;
	char		buffer[4];
	uint32_t	ipval = 0; // ip 的 10 进制表示
	const char *copy  = ip;

	while(*copy != '\0') {
		if(*copy == ' ') {
			copy++;
			continue;
		}
		if(*copy == '.') {
			// single part length limit
			if(i > 3) {
				ipval = 0;
				break;
			}
			if(r < 0)
				break;

			buffer[i] = '\0';
			ipval |= (atoi(buffer) << r);
			r -= 8;
			i = 0;
		} else {
			buffer[i++] = *copy;
		}

		copy++;
	}

	// append the rest parts
	if(i > 3)
		return 0;
	buffer[i] = '\0';
	ipval |= atoi(buffer);

	return ipval;
}

/**
 * @brief 将一个文本文件以字符串的形式读取到内存中
 * @param filename 文件名
 * @return char*
 */
char *read_all_file(const char *filename) {
	char *text = NULL;
	FILE *fp   = fopen(filename, "rb");
	// get file size
	fseek(fp, 0, SEEK_END);
	size_t file_len;
	file_len = ftell(fp);
	// 申请空间
	text = (char *)malloc(file_len + 1);
	rewind(fp);
	if(fread(text, sizeof(char), file_len, fp) != file_len)
		return NULL;
	// 给字符串终止符
	text[file_len] = '\0';
	fclose(fp);
	return text;
}

/**
 * @brief 获取下一个 IP 地址的信息
 * @param offset 偏移量
 * @param ip_num ip 的 10 进制表示数
 * @param text 所有ip 地址信息的字符串
 * @param ret ip 的起始段
 */
void *get_next_info(uint32_t *offset, uint32_t *ip_num, const char *text, char *ret) {
	// start ip
	uint32_t length = strchr(text + *offset, '|') - text - *offset;
	strncpy(ret, text + *offset, length);
	*ip_num = ip2uint32(ret);
	*offset += length;
	// end ip
	*offset += strchr(text + *offset + 1, '|') - text - *offset;
	// ip info
	length = strchr(text + *offset + 1, '\n') - text - *offset - 1;
	strncpy(ret, text + *offset + 1, length);
	*offset += length + 2;
}

/**
 * @brief 输出索引区
 * 索引区：index block
 * +------------+-----------+
 * | 4bytes		| 4bytes	|
 * |ip_start, data_position(3 bytes) & data_len(1 bytes)
 * +------------+-----------+
 * @param index 地址在记录区的位置
 * @param ip ip 的 10 进制表示
 * @param data_len 地址所占字节数
 * @param fp 要输出的文件的指针
 **/
void putout_ip(const uint32_t index, const uint32_t ip, const unsigned char data_len, FILE *fp) {
	fwrite(&ip, sizeof(ip), 1, fp);
	fwrite(&index, sizeof(index), 1, fp);
	// 舍弃地址的最高位
	fseek(fp, -1, SEEK_CUR);
	fwrite(&data_len, 1, 1, fp);
}

/**
 * @brief 输出记录区
 * 记录区：data
 * +------------+-----------+
 * |  dynamic length		|
 * country, province, city, isp
 * +------------+-----------+
 * @param info 地址的具体内容
 * @param fp 要输出的文件的指针
 */
void putout_info(const char *info, FILE *fp) { fwrite(info, strlen(info), 1, fp); }

/**
 * @brief 制作记录区和索引区
 * @param 参数
 */
void make_info(const char *name_source, const char *name_info, const char *name_ip) {
	FILE *file_info = fopen(name_info, "wb");
	FILE *file_ip   = fopen(name_ip, "wb");

	char *   file_source = read_all_file(name_source); // 读取原始数据
	uint32_t ip			 = 0;
	uint32_t index		 = 0;  // 地址信息位置
	uint32_t offset		 = 0;  // 偏移量
	char	 line[100];		   // 地址内容
	root_t   tree   = RB_ROOT; // 建立红黑树
	uint32_t ip_cnt = 0;	   // 当前构造 IP 总数

	while(++ip_cnt <= 586177) { // 总条数为 586177

		memset(line, 0, sizeof(line)); // 清空上次的地址内容
		get_next_info(&offset, &ip, file_source, line);

		map_t *info = get(&tree, line); // 查询 map
		if(info == NULL) {
			// 获取ip_info文件指针当前位置
			index = ftell(file_info);
			// ip2string
			char str[12];
			itoa(index, str, 10);
			// 存入 map
			put(&tree, line, str);
			// 将记录和索引存入文件
			putout_info(line, file_info);
			putout_ip(index, ip, strlen(line), file_ip);
		} else {
			// 获取索引
			index = atoi(info->val);
			// 将索引存入文件
			putout_ip(index, ip, strlen(line), file_ip);
		}
	}

	free(file_source);
	fclose(file_info);
	fclose(file_ip);
}

/**
 * @brief 拼接记录区和索引区，并输出文件头
 * 文件头：file header(8 bytes)
 * +------------+-----------+
 * | 4 bytes	| 4 bytes   |
 * first index position, last index position
 * +------------+-----------+
 * 记录区：data
 * +------------+-----------+
 * 索引区：index block
 * +------------+-----------+
 * @param name_data
 * @param name_index
 * @param name_db
 */
void makedb(const char *name_data, const char *name_index, const char *name_db) {
	// 获取记录区数据
	FILE *file_db = fopen(name_db, "wb");
	char *data	= read_all_file(name_data);

	// 获取索引区数据
	FILE *file_index = fopen(name_index, "rb");
	fseek(file_index, 0, SEEK_END);
	uint32_t index_len = ftell(file_index);
	rewind(file_index);
	// 因为索引区有为 0 的数据，故新建一个大小为 1B 的结构体来存储
	struct ip_index {
		char c;
	} * index;
	index = malloc(index_len);
	if(fread(index, sizeof(char), index_len, file_index) != index_len)
		return;
	fclose(file_index);

	// 文件头为第一个索引的地址和最后一个索引的地址
	int first_index = strlen(data);
	int last_index  = index_len + first_index;

	// 将文件头、记录区、索引区写入
	fwrite(&first_index, sizeof(first_index), 1, file_db);
	fwrite(&last_index, sizeof(last_index), 1, file_db);
	fwrite(data, strlen(data), 1, file_db);
	fwrite(index, index_len, 1, file_db);
	// 写入数据库创建时间
	char *copyright = "Ceated by ASC_8384 at 2020-02-09";
	fwrite(copyright, strlen(copyright), 1, file_db);

	free(data);
	free(index);
	fclose(file_db);
}

int main() {
	// 文件需要在同目录下
	char *name_source = "./ip.txt";
	char *name_info   = "./ip_info.dat";
	char *name_ip	 = "./ip_simplify.dat";
	char *name_db	 = "./ip.db";

	// 创建索引
	make_info(name_source, name_info, name_ip);
	// 创建数据库
	makedb(name_info, name_ip, name_db);

	return 0;
}