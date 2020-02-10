/**
 * @file 		ini_load.h
 * @brief 		解析 ini 文件
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       	2020-02-09
 */
#ifndef INI_LOAD_H_
#define INI_LOAD_H_

#include "iniparser.h"
#include <stdbool.h>

/**
 * @brief 定义显示信息的结构体
 * country, province, city, isp 依次为国家、省份、城市、ISP
 */
typedef struct show_msg {
	bool country, province, city, isp;
} show_msg;

/**
 * @brief 将设置保存到文件
 * @param ini ini 设置名
 * @param inipath 保存文件的路径
 */
void iniparser_save(const dictionary *ini, const char *inipath);

/**
 * @brief 初始化设置
 * @param ini_name 文件名
 * @param init_string 初始化内容
 * @param ini ini 设置名的地址
 */
void iniparser_init(const char *ini_name, const char *init_string, dictionary **ini);

/**
 * @brief 读取设置内容
 * @param ini ini 设置名
 */
void iniparser_get_is_show(dictionary *ini, show_msg *is_show);

#endif