/**
 * @file 		ip_match.h
 * @brief 		查询数据库，匹配 IP
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.4
 * @date       	2020-02-10
 */
#ifndef IP_MATCH_H_
#define IP_MATCH_H_

#include "parse_input.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// 读取存储的数据
#define buffer(x) buffer[x].c

/**
 * @brief 为避免文本中间有'\0'而设计的结构体
 * c 该字节存储的数据
 */
typedef struct ipdatabase {
	uint8_t c;
} ipdbc;

/**
 * @brief 数据库解析
 * header_len 文件头长度
 * header_first 索引区开头位置
 * header_last 索引区结尾位置
 * total_block 索引总数
 * index_block_len 单个索引的字节数
 * file 数据库文件
 */
typedef struct ipdb {
	uint32_t header_len;
	uint32_t header_first;
	uint32_t header_last;
	uint32_t total_block;
	uint32_t index_block_len;
	ipdbc *  file;

} ipdb;

/**
 * @brief IP 地址的具体信息
 * country 国家
 * province 省份
 * city 城市
 * isp ISP
 */
typedef struct IpMessage {
	uint8_t country[20];
	uint8_t province[20];
	uint8_t city[20];
	uint8_t isp[20];
} ip_msg;

/**
 * @brief 得到 IP 地址的具体信息
 * @param buffer 数据存放地址
 * @param len 数据长度
 * @return ip_msg
 */
ip_msg GET_IP_REGION(const ipdbc *buffer, uint32_t len);

/**
 * @brief 将一个四个连续的字节内容转为一个32位整形
 * @param buffer 数据存放地址
 * @param offset 偏移量
 * @return uint32_t
 */
uint32_t GET_INT32(const ipdbc *buffer, int32_t offset);

/**
 * @brief 将一个文本文件以结构体（长度为1字节）的形式读取到内存中
 * @param filename 文件名
 * @return ipdbc
 */
ipdbc *READ_ALL_FILE(char *filename);

/**
 * @brief 初始化数据库
 * @param filename 数据库文件名
 * @return ipdb
 */
ipdb *ipdb_init(char *filename);

/**
 * @brief 在数据库中匹配 IP 地址
 * @param want 要查询的 IP
 * @param db 数据库
 * @return ip_msg
 */
ip_msg match_ip(const ip *want, const ipdb *db);

#endif