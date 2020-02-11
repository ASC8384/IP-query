/**
 * @file 		ip_match.c
 * @brief 		构造 ip.db 数据库
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       2020-02-09
 */
#include "ini_load.h"

/**
 * @brief 将设置保存到文件
 * @param ini ini 设置名
 * @param inipath 保存文件的路径
 */
void iniparser_save(const dictionary *ini, const char *inipath) {
	FILE *fp = fopen(inipath, "w");
	iniparser_dump_ini(ini, fp); // 调用存储函数
	fclose(fp);
}

/**
 * @brief 初始化设置
 * @param ini_name 文件名
 * @param init_string 初始化内容
 * @param ini ini 设置名的地址
 */
void iniparser_init(const char *ini_name, const char *init_string, dictionary **ini) {
	FILE *file_ini = fopen(ini_name, "w");
	fprintf(file_ini,"%s", init_string); // 将初始化的内容写入 ini 文件
	fclose(file_ini);
	// 加载 ini 文件
	*ini = iniparser_load(ini_name);
}

/**
 * @brief 读取设置内容
 * @param ini ini 设置名
 */
void iniparser_get_is_show(dictionary *ini, show_msg *is_show) {
	// 从 ini 文件中读取设置参数
	if(!iniparser_find_entry(ini, "show:country"))
		iniparser_set(ini, "show:country", "1");
	if(!iniparser_find_entry(ini, "show:province"))
		iniparser_set(ini, "show:province", "1");
	if(!iniparser_find_entry(ini, "show:city"))
		iniparser_set(ini, "show:city", "1");
	if(!iniparser_find_entry(ini, "show:isp"))
		iniparser_set(ini, "show:isp", "1");
	// 将值传给 is_show 结构体，来存储显示的信息
	is_show->country  = iniparser_getboolean(ini, "show:country", 1);
	is_show->province = iniparser_getboolean(ini, "show:province", 1);
	is_show->city	 = iniparser_getboolean(ini, "show:city", 1);
	is_show->isp	  = iniparser_getboolean(ini, "show:isp", 1);
}