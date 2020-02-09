#include "ini_load.h"
#include "ip_match.h"
#include "parse_input.h"
#include <stdint.h>
#include <stdio.h>

#if(defined(_WIN32) || defined(_WINDOWS_) || defined(__WINDOWS_))
#define OS_WINDOW
#else
#define OS_LINUX
#endif

#ifdef OS_WINDOW
#include <conio.h>
#include <time.h>
#define gettimeofday(func, NULL) mingw_gettimeofday(func, NULL)
#elif OS_LINUX
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
int getch(void) {
	int			   ch;
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}
#endif

// output colors
#define T_COLOR_NONE "\033[0m"
#define T_COLOR_ORANGE "\033[0;33m" // BROWN
#define T_COLOR_LIGHT_CYAN "\033[1;36m"
#define T_COLOR_LIGHT_RED "\033[1;31m"
#define T_COLOR_WHITE_BACKGROUND "\033[7m"

void PUTOUT_IP_MSG(const show_msg *is_show, const unsigned long cnt, const ip *want,
				   const ip_msg *pos, const unsigned long timer) {
	printf("%s[%lu]%s %s%hu.%hu.%hu.%hu%s ", T_COLOR_ORANGE, cnt, T_COLOR_NONE, T_COLOR_LIGHT_CYAN,
		   want->ip[0], want->ip[1], want->ip[2], want->ip[3], T_COLOR_NONE);
	if(is_show->country)
		printf("%s ", pos->country);
	if(is_show->province)
		printf("%s ", pos->province);
	if(is_show->city)
		printf("%s ", pos->city);
	if(is_show->isp)
		printf("%s ", pos->isp);
	printf("| query time = %.3lf ms\n", timer / 1000.0);
}

// match and print ip massage
void QUERY_IP(ipdb *file_ip, show_msg *is_show, unsigned long *cnt, iterator *itor) {
	struct timeval func_start;
	struct timeval func_end;
	ip *		   want;
	ip_msg		   pos;
	uint32_t	   query_num;
	query_num = 0;

	putchar('\n');
	do {
		query_num++;
		want = (ip *)get_next(itor);
		gettimeofday(&func_start, NULL);
		pos = match_ip(want, file_ip);
		gettimeofday(&func_end, NULL);
		unsigned long timer =
			1000000 * (func_end.tv_sec - func_start.tv_sec) + func_end.tv_usec - func_start.tv_usec;
		PUTOUT_IP_MSG(is_show, ++(*cnt), want, &pos, timer);
		if(query_num % 24 == 0) {
			printf("%s-- More --%s ", T_COLOR_WHITE_BACKGROUND, T_COLOR_NONE);
			char c;
			while(c = getch()) {
				if(c == 'q') {
					printf("\r%*s", 10, " ");
					return;
				}
				if(c == ' ')
					break;
				if(c == 'j') {
					query_num--;
					break;
				}
			}
			putchar('\r');
		}
	} while(has_next(itor));
	iterator_free(itor);
}

ipdb *read_all_file(char *filename) {
	ipdb *text = NULL;
	FILE *fp   = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	unsigned long long file_len;
	file_len = ftell(fp);
	text	 = malloc(file_len);
	rewind(fp);
	fread(text, sizeof(ipdb), file_len, fp);
	fclose(fp);
	return text;
}

int main(int argc, char *argv[]) {
	dictionary *ini		 = NULL;
	ipdb *		file_ip  = NULL;
	char *		ini_name = NULL;
	show_msg	is_show;
	bool		has_command = false;

	// process ini
	ini_name = "ip-query.ini";
	ini		 = iniparser_load(ini_name);
	if(ini == NULL) {
		iniparser_init(ini_name, "[show]\n[sort]\n[file]\n", &ini);
	}
	iniparser_get_is_show(ini, &is_show);

	printf("Welcome to IP-query!");
	file_ip = read_all_file("./src/common/ip.db");

	char putin[256];
	putin[0] = '\0';
	if(argc >= 2) {
		has_command = true;
		for(int i = 1; i < argc; i++) {
			strcat(putin, argv[i]);
			strcat(putin, " ");
		}
	}

	unsigned long cnt = 0;
	do {
		iterator *itor = NULL;
	// get input
	fail_parse_input:
		// process the putin_string
		if(!has_command) {
			printf("\nPlease input the IP address: ");
			fgets(putin, 256, stdin);
		}
		switch(parse_input(putin)) {
		case 'f':
			parse_file(putin);
		case 'y':
			itor = parse_ip(putin);
			QUERY_IP(file_ip, &is_show, &cnt, itor);
			break;
		case 'q':
			goto main_exit;
			break;
		default:
			printf("%sInput error!%s\nPlease check up and input the IP address again: ",
				   T_COLOR_LIGHT_RED, T_COLOR_NONE);
			if(has_command)
				goto main_exit;
			else
				goto fail_parse_input;
		}
	} while(!has_command);

// end of ip-query
main_exit : {
	iniparser_save(ini, ini_name);
	iniparser_freedict(ini);
	free(file_ip);
}
	return 0;
}
