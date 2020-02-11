/**
 * @file 		main.c
 * @brief 		主函数，构建 IP 地址查询系统
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.6
 * @date       	2020-02-10
 */
#include "ini_load.h"
#include "ip_match.h"
#include "parse_input.h"
#include <stdint.h>
#include <stdio.h>

// 判断系统
#if(defined(_WIN32) || defined(_WINDOWS_) || defined(__WINDOWS_))
#define OS_WINDOW
#else
#define OS_LINUX
#endif

// 跨平台处理
#ifdef OS_WINDOW
#include <conio.h>
#include <time.h>
#include <windows.h>
#define gettimeofday(func, NULL) mingw_gettimeofday(func, NULL)
/**
 * @brief 判断终端高度
 * @return int
 */
int get_terminal_high(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#else
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
/**
 * @brief 读取一个字符，并且不回显
 * @return int
 */
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

/**
 * @brief 判断终端高度
 * @return int
 */
int get_terminal_high(void) {
	struct winsize ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	return ws.ws_row;
}
#endif

// 终端高度
#define TERMINAL get_terminal_high()
// output colors
#define T_COLOR_NONE "\033[0m"
#define T_COLOR_ORANGE "\033[0;33m" // brown
#define T_COLOR_LIGHT_CYAN "\033[1;36m"
#define T_COLOR_LIGHT_RED "\033[1;31m"
#define T_COLOR_WHITE_BACKGROUND "\033[7m"

/**
 * @brief 输出 IP 地址信息
 * @param is_show 判断哪些内容需要输出
 * @param cnt 查询 IP 总数
 * @param want 要查询的 IP
 * @param pos 要查询的 IP 的信息
 * @param timer 查询所消耗的时间
 */
void PUTOUT_IP_MSG(const show_msg *is_show, const unsigned long cnt, const ip *want,
				   const ip_msg *pos, const unsigned long timer) {
	// 要查询的 IP
	printf("%s[%lu]%s %s%hu.%hu.%hu.%hu%s ", T_COLOR_ORANGE, cnt, T_COLOR_NONE, T_COLOR_LIGHT_CYAN,
		   want->ip[0], want->ip[1], want->ip[2], want->ip[3], T_COLOR_NONE);
	if(is_show->country) // 是否输出国家
		printf("%s ", pos->country);
	if(is_show->province) // 是否输出省份
		printf("%s ", pos->province);
	if(is_show->city) // 是否输出城市
		printf("%s ", pos->city);
	if(is_show->isp) // 是否输出 ISP
		printf("%s ", pos->isp);
	printf("| query time = %.3lf ms\n", timer / 1000.0); // 输出查询耗时
}

/**
 * @brief IP 查询
 * @param is_show 判断哪些内容需要输出
 * @param cnt 查询 IP 总数
 * @param want 要查询的 IP
 * @param pos 要查询的 IP 的信息
 * @param timer 查询所消耗的时间
 */
void QUERY_IP(ipdb *file_ip, show_msg *is_show, uint64_t *cnt, iterator *itor) {
	struct timeval func_start, func_end;   // 读取当前时间，计算查询所消耗的时间
	ip *		   want;				   // 要查询的 IP
	ip_msg		   pos;					   // 要查询的 IP 的信息
	uint32_t	   query_num = 0;		   // 本次查询 IP 的数量
	uint32_t terminal_high = TERMINAL - 2; // 终端高度，减去 "more" 和换行符所占行数
	// 开始查询
	putchar('\n');
	do {
		query_num++;
		// 得到下一个要查询的 IP
		want = (ip *)get_next(itor);
		gettimeofday(&func_start, NULL); // 获取当前时间
		pos = match_ip(want, file_ip);   // 匹配 IP 信息
		gettimeofday(&func_end, NULL);   // 获取当前时间
		unsigned long timer =			 // 计算查询时间
			1000000 * (func_end.tv_sec - func_start.tv_sec) + func_end.tv_usec - func_start.tv_usec;
		PUTOUT_IP_MSG(is_show, ++(*cnt), want, &pos, timer); // 输出查询结果
		// 如果本次查询 IP 的数量等于终端高度
		if(query_num % terminal_high == 0) {
			printf("%s-- More --%s ", T_COLOR_WHITE_BACKGROUND, T_COLOR_NONE);
			char c;
			while(c = getch()) { // 不回显
				if(c == 'q') {   // 按'q'退出
					printf("\r%*s", 10, " ");
					return;
				}
				if(c == ' ' || c == 'f') // 按' '或'f'翻页
					break;
				if(c == 'j') { // 按'j'下一行
					query_num--;
					break;
				}
				if(c == 'd') { // 按'd'翻半页
					query_num -= (terminal_high >> 1);
					break;
				}
			}
			putchar('\r'); // 覆盖 "-- More --"
		}
	} while(has_next(itor)); // 判断需要查询的 IP 是否全部查询完
	iterator_free(itor);
}

/**
 * @brief IP 地址查询系统
 */
int main(int argc, char *argv[]) {
	dictionary *ini		= NULL;		 // 设置需要输出的内容
	ipdb *		file_ip = NULL;		 // IP database 解析
	uint64_t	cnt		= 0;		 // 本次运行期间的查询总数
	show_msg	is_show;			 // 存储需要输出的内容的设置
	bool		has_command = false; // 是否是命令行操作

	// process ini
	char *ini_name = NULL; // 设置文件
	ini_name	   = "ip-query.ini";
	ini			   = iniparser_load(ini_name); // 加载设置文件
	if(ini == NULL) {
		iniparser_init(ini_name, "[show]\n", &ini); // 如果不存在，则初始化
	}
	iniparser_get_is_show(ini, &is_show); // 读取设置内容

	// 输出欢迎词
	printf("Welcome to IP-query!");
	file_ip = ipdb_init("./ip.db");
	// 获取输入
	char putin[256];
	putin[0] = '\0';
	if(argc >= 2) { // 如果是从命令行直接执行
		has_command = true;
		for(int i = 1; i < argc; i++) { // 拼接命令
			strcat(putin, argv[i]);
			strcat(putin, " ");
		}
	}

	do {
		iterator *itor = NULL;
	fail_parse_input:	  // 如果解析失败
		if(!has_command) { //
			memset(putin, 0, sizeof(putin));
			printf("\nPlease input the IP address: ");
			fgets(putin, 256, stdin);
		}
		// process the putin_string
		switch(parse_input(putin)) {
		case 'f': // 从文件读入
			parse_file(putin);
		case 'y': // 要查询的IP 地址合法
			itor = parse_ip(putin);
			QUERY_IP(file_ip, &is_show, &cnt, itor);
			break;
		case 'q': // 退出程序
			goto main_exit;
			break;
		default: // 要查询的 IP 地址非法
			printf("%sInput error!%s\nPlease check up and input the IP address again.",
				   T_COLOR_LIGHT_RED, T_COLOR_NONE);
			if(has_command) // 是否是命令行操作
				goto main_exit;
			else
				goto fail_parse_input;
		}
	} while(!has_command); // 判断是否是可交互环境

// end of ip-query
main_exit : {
	iniparser_save(ini, ini_name); // 保存设置
	iniparser_freedict(ini);
	free(file_ip);
}
	return 0;
}
