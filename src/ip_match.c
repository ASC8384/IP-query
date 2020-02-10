/**
 * @file 		ip_match.c
 * @brief 		查询数据库，匹配 IP
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.4
 * @date       	2020-02-10
 */
#include "ip_match.h"

/**
 * @brief 得到 IP 地址的具体信息
 * @param buffer 数据存放地址
 * @param len 数据长度
 * @return ip_msg
 */
ip_msg GET_IP_REGION(const ipdbc *buffer, uint32_t len) {
	uint32_t c;		  // 暂存单个字符
	ip_msg   pos;	 // IP 地址的具体信息
	uint32_t pos_num; // 当前读入的数据数量
	int32_t  cnt;	 // 作为一个迭代器，遍历 buffer 内容
	// 初始化
	memset(&pos, 0, sizeof(pos));
	cnt		= -1;
	pos_num = 0;
	while((c = buffer(++cnt)) != '|') // 过滤'|'
		pos.country[pos_num++] = c;   // 储存国家信息
	c		= buffer(++cnt);		  // 过滤'0'
	c		= buffer(++cnt);		  // 过滤'|'
	pos_num = 0;
	while((c = buffer(++cnt)) != '|') // 过滤'|'
		pos.province[pos_num++] = c;  // 储存省份信息
	pos_num = 0;
	while((c = buffer(++cnt)) != '|') // 过滤'|'
		pos.city[pos_num++] = c;	  // 储存城市信息
	pos_num = 0;
	while((c = buffer(++cnt)) != 0x0D && cnt < len - 1) // 过滤'\r\n'
		pos.isp[pos_num++] = c;							// 储存ISP信息

	return pos;
}

/**
 * @brief 将一个四个连续的字节内容转为一个32位整形
 * @param buffer 数据存放地址
 * @param offset 偏移量
 * @return uint32_t
 */
inline uint32_t GET_INT32(const ipdbc *buffer, int32_t offset) {
	return (((buffer(offset)) & 0x000000FF) |			// 读取最低位
			((buffer(offset + 1) << 8) & 0x0000FF00) |  // 读取次低位
			((buffer(offset + 2) << 16) & 0x00FF0000) | // 读取次高位
			((buffer(offset + 3) << 24) & 0xFF000000)); // 读取最高位
}

/**
 * @brief 将一个文本文件以结构体（长度为1字节）的形式读取到内存中
 * @param filename 文件名
 * @return ipdbc
 */
ipdbc *READ_ALL_FILE(char *filename) {
	ipdbc *text = NULL;
	FILE * fp   = fopen(filename, "rb");
	// 判断文本总长度
	fseek(fp, 0, SEEK_END);
	uint64_t file_len;
	file_len = ftell(fp);
	// 申请内存
	text = malloc(file_len);
	// 回到文本开头
	rewind(fp);
	// 写入内存
	fread(text, sizeof(ipdbc), file_len, fp);
	fclose(fp);
	return text;
}

/**
 * @brief 初始化数据库
 * @param filename 数据库文件名
 * @return ipdb
 */
ipdb *ipdb_init(char *filename) {
	// 申请内存
	ipdb *db = malloc(sizeof(ipdb));

	db->header_len		= 8; // 文件头长度
	db->index_block_len = 8; // 单个索引的字节数

	db->file		 = READ_ALL_FILE(filename);					// 读取数据库文件
	db->header_first = GET_INT32(db->file, 0) + db->header_len; // 索引区开头位置
	db->header_last  = GET_INT32(db->file, 4) + db->header_len; // 索引区结尾位置
	db->total_block = (db->header_last - db->header_first) / db->index_block_len + 1; // 索引总数

	return db;
}

/**
 * @brief 在数据库中匹配 IP 地址
 * @param want 要查询的 IP
 * @param db 数据库
 * @return ip_msg
 */
ip_msg match_ip(const ip *want, const ipdb *db) {
	ip_msg   pos;	 // 要查询的 IP 的具体信息内容
	uint32_t ip_val = // 要查询的IP 的 10 进制数
		(want->ip[0] << 24) + (want->ip[1] << 16) + (want->ip[2] << 8) + (want->ip[3]);
	uint32_t first_index	 = db->header_first;	// 索引区开头位置
	uint32_t last_index		 = db->header_last;		// 索引区结尾位置
	uint32_t total_block	 = db->total_block;		// 索引总数
	uint32_t index_block_len = db->index_block_len; // 单个索引的字节数
	uint32_t header_len		 = db->header_len;		// 文件头长度
	ipdbc *  file			 = db->file;			// 数据库内容

	const ipdbc *buffer;			  // 当前读到的数据
	uint32_t	 data  = 0;			  // 匹配到的数据
	uint32_t	 left  = 0;			  // 左边界
	uint32_t	 right = total_block; // 右边界
	// 二分查找
	while(left <= right) {
		uint32_t mid = (left + right) / 2;
		uint32_t pos = first_index + mid * index_block_len; // 当前查询点（偏移量）
		buffer		 = file + pos;

		uint32_t cur_ip = GET_INT32(buffer, 0); // 当前的 IP 的 10 进制数

		if(ip_val < cur_ip) {
			right = mid - 1; // 查询位置偏大
		} else {
			uint32_t cur_ip_next = // 获取下一个区域的 IP 的 10 进制数
				GET_INT32(buffer, index_block_len);
			if(ip_val < cur_ip_next) {		 // 匹配成功
				data = GET_INT32(buffer, 4); // 读取数据
				break;
			} else { // 查询位置偏小
				left = mid + 1;
			}
		}
	}
	// ip >= 224.0.0.0
	if(data == 0) // 匹配第一个 IP 段
		data = GET_INT32(file + first_index + header_len, 0);
	// parse data
	uint32_t data_pos = data & 0x00ffffff;			  // 数据存放的地址偏移量
	uint32_t data_len = (data >> 24) & 0x000000ff;	// 数据总长度
	buffer			  = file + data_pos + header_len; // 数据存放的具体地址

	pos = GET_IP_REGION(buffer, data_len);

	return pos;
}