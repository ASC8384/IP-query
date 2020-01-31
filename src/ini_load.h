#ifndef INI_LOAD_H_
#define INI_LOAD_H_

#include "iniparser.h"
#include <stdbool.h>

typedef struct show_msg {
	bool country, province, city, isp;
} show_msg;

int  iniparser_save(const dictionary *ini, const char *inipath);
void iniparser_init(const char *ini_name, const char *init_string, dictionary **ini);
void iniparser_get_is_show(dictionary *ini, show_msg *is_show);

#endif