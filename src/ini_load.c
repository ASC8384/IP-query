#include "ini_load.h"

int iniparser_save(const dictionary *ini, const char *inipath) {
	FILE *fp = NULL;

	if(inipath == NULL || ini == NULL) {
		int ret = -1;
		printf("saveConfig error:%d from (filepath == NULL || head == NULL)\n", ret);
		return ret;
	}

	fp = fopen(inipath, "w");
	if(fp == NULL) {
		int ret = -2;
		printf("saveConfig:open file error:%d from %s\n", ret, inipath);
		return ret;
	}

	iniparser_dump_ini(ini, fp);
	fclose(fp);

	return 0;
}

void iniparser_init(const char *ini_name, const char *init_string, dictionary **ini) {
	FILE *file_ini = fopen(ini_name, "w");
	fprintf(file_ini, init_string);
	fclose(file_ini);

	*ini = iniparser_load(ini_name);
}

void iniparser_get_is_show(dictionary *ini, show_msg *is_show) {
	if(!iniparser_find_entry(ini, "show:country"))
		iniparser_set(ini, "show:country", "1");
	if(!iniparser_find_entry(ini, "show:province"))
		iniparser_set(ini, "show:province", "1");
	if(!iniparser_find_entry(ini, "show:city"))
		iniparser_set(ini, "show:city", "1");
	if(!iniparser_find_entry(ini, "show:isp"))
		iniparser_set(ini, "show:isp", "1");

	is_show->country  = iniparser_getboolean(ini, "show:country", 1);
	is_show->province = iniparser_getboolean(ini, "show:province", 1);
	is_show->city	 = iniparser_getboolean(ini, "show:city", 1);
	is_show->isp	  = iniparser_getboolean(ini, "show:isp", 1);
}