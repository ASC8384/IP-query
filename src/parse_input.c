/**
 * @file 		parse_input.c
 * @brief 		解析输入
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       	2020-02-09
 */
#include "parse_input.h"

/**
 * @brief 将 IP 地址插入链表
 * @param item 要插入的 IP 地址
 * @param list 当前链表的尾端
 * @return bool
 */
bool IP_LIST_INSERT(const ip *item, ip *list) {
	ip *pnew; // 要插入的 IP
	ip *scan = list;
	// 申请内存
	pnew = calloc(sizeof(*pnew), 1);
	if(pnew == NULL) // 如果无法申请
		return false;
	// 复制元素
	for(int i = 0; i <= 3; i++)
		pnew->ip[i] = item->ip[i];
	scan->next = pnew;

	return true;
}

/**
 * @brief 将一个连续的 IP 地址插入链表中
 * @param st 开始的 IP 地址
 * @param ed 结束的 IP 地址
 * @param current 当前容器（链表）的尾端的地址
 */
void IP_LIST_INSERT_CONTINUOUS(ip *st, ip *ed, ip **current) {
	while(!(st->ip[0] == ed->ip[0] && st->ip[1] == ed->ip[1] && st->ip[2] == ed->ip[2] &&
			st->ip[3] == ed->ip[3])) { // 判断 IP 地址是否结束
		IP_LIST_INSERT(st, *current);  // 将开始地址插入链表
		*current = (*current)->next;   // 更新链表
		// ip address increase progressively
		st->ip[3]++;				   // 从最末端加起
		if(st->ip[3] >= 256) {		   // 超过范围
			st->ip[3] = 0;			   // 当前归零
			st->ip[2]++;			   // 向上加一
			if(st->ip[2] >= 256) {	 // 超过范围
				st->ip[2] = 0;		   // 当前归零
				st->ip[1]++;		   // 向上加一
				if(st->ip[1] >= 256) { // 超过范围
					st->ip[1] = 0;	 // 当前归零
					st->ip[0]++;	   // // 向上加一，默认 ip[0] <= 256
				}
			}
		}
	}
	IP_LIST_INSERT(ed, *current); // 插入最后结尾 IP
	*current = (*current)->next;  // 更新链表
}

/**
 * @brief 将带有通配符的 IP 地址插入链表中
 * @param result 当前带有通配符的 IP 地址的字符串
 * @param current 当前容器（链表）的尾端的地址
 */
void IP_LIST_INSERT_WILDCARD(const char *result, ip **current) {
	bool has_wildcard_question_mark = false; // 是否具有'?'
	// 复制输入的字符串
	char *copy = malloc(strlen(result));
	strcpy(copy, result);

	int i = 0;
	while(copy[i]) {
		if(copy[i] == '?') { // 具有'?'
			has_wildcard_question_mark = true;
			break;
		}
		if(copy[i] == '*')
			break;
		i++;
	}
	char *   exstring = NULL;
	uint32_t st, ed;
	// 具有'?'
	if(has_wildcard_question_mark) {
		exstring = malloc(strlen(copy) * 10 + 10); // 总共 10 个
		st		 = 0;
		ed		 = 9;
	} // 具有'*'
	else {
		exstring = malloc((strlen(copy) + 2) * 256 + 256); // 总共 256 个
		st		 = 0;
		ed		 = 255;
	}
	exstring[0] = '\0';
	copy[i]		= '\0';
	char numstr[5];
	while(st <= ed) {
		strcat(exstring, copy);
		itoa(st, numstr, 10);
		strcat(exstring, numstr);
		strcat(exstring, copy + i + 1);
		strcat(exstring, " ");
		st++;
	}
	char delims[] = " \t\n";
	copy		  = strtok(exstring, delims);
	while(copy != NULL) {
		ip node;
		sscanf(copy, "%hu.%hu.%hu.%hu", &node.ip[0], &node.ip[1], &node.ip[2], &node.ip[3]);
		IP_LIST_INSERT(&node, *current);
		*current = (*current)->next; // 更新链表
		copy	 = strtok(NULL, delims);
	}
}

/**
 * @brief 判断输入是否合法的具体实现
 * @param putin 输入的字符串
 * @return char
 */
char IS_PUTIN_OK(const char *putin) {
	if(strlen(putin) < 7)
		return 'w';
	// 复制输入的字符串
	char *copy = malloc(strlen(putin));
	strcpy(copy, putin);
	// 分割字符串
	char *result   = NULL;
	char  delims[] = " \t\n"; // 以空格和 Tab 键作为分隔符
	result		   = strtok(copy, delims);
	while(result != NULL) {
		char *   address_result = result; // 保存当前字符串地址
		uint32_t count_point	= 0;	  // '.'的个数
		uint32_t count_bar		= 0;	  // '-'的个数
		uint32_t count_num		= 0;	  // 数字的个数
		int32_t  ip_num			= -1;	 // 当前读入的 IP 字段
		while(*result) {				  // 当前字符串是否处理完
			if(isdigit(*result)) {		  // 当前字符为数字
				if(ip_num == -1)		  // 初始化
					ip_num = 0;
				ip_num = ip_num * 10 + *result - '0'; // 加入当前数字
			} else if(*result == '.') {				  // 当前字符为点
				if(ip_num > 255 || ip_num == -1) // 如果现在的 IP 字段大于 255 或左侧不是数字
					return 'w';
				ip_num = -1;
				count_num++;
				count_point++;
			} else if(*result == '-') {			 // 当前字符为数字
				if(ip_num > 255 || ip_num == -1) // 如果现在的 IP 字段大于 255 或左侧不是数字
					return 'w';
				ip_num = -1;
				count_num++;
				count_bar++;
			} else if(*result == '*') { // 当前字符为'*'通配符
				if(ip_num != -1)		// 如果存在 IP 字段
					return 'w';
				if(*(result + 1)) // 如果不是最后一位
					ip_num = 0;
			} else if(*result == '?') { // 当前字符为'?'通配符
				ip_num = 0;
			} else { // 读入其它字符
				return 'w';
			}
			result++;
		}
		count_num++;			 // 加上最末一位的 IP 字段
		result = address_result; // 将当前字符串状态重置
		if(count_bar > 1)		 // '-'的数量异常
			return 'w';
		if(count_point % 3 != 0 || count_point == 0) // '.' 的数量异常
			return 'w';
		if(count_num != 4 && count_num != 8) // IP 字段数量异常
			return 'w';
		if(count_bar == 1) { // 存在'-'
			ip st, ed;
			sscanf(result, "%hu.%hu.%hu.%hu-%hu.%hu.%hu.%hu", &st.ip[0], &st.ip[1], &st.ip[2],
				   &st.ip[3], &ed.ip[0], &ed.ip[1], &ed.ip[2], &ed.ip[3]); // 读入
			if(ed.ip[0] < st.ip[0] || (ed.ip[0] == st.ip[0] && ed.ip[1] < st.ip[1]) ||
			   (ed.ip[0] == st.ip[0] && ed.ip[1] == st.ip[1] && ed.ip[2] < st.ip[2]) ||
			   (ed.ip[0] == st.ip[0] && ed.ip[1] == st.ip[1] && ed.ip[2] == st.ip[2] &&
				ed.ip[3] < st.ip[3])) // 判断连字符左右地址是否合法
				return 'w';
		}
		result = strtok(NULL, delims); // 继续分割
	}
	return 'y';
}

/**
 * @brief 判断容器（链表）是否还有可返回的元素
 * @param filename 文件名
 * @return char*
 */
static bool ip_list_has_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor; // 取出内建迭代器
	ip *		item = p->item; // 读取内建迭代器所代表的容器元素

	return !(item->next == NULL); // 判断是否还有可返回的元素
}

/**
 * @brief 返回容器（链表）的下一个元素
 * @param filename 文件名
 * @return ip*
 */
static void *ip_list_get_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor; // 取出内建迭代器
	ip *		item = p->item;	// 读取内建迭代器所代表的容器元素
	ip *		next = item->next; // 得到容器的下一个元素的地址
	free(item);					   // 读完即释放内存
	p->item = next;

	return next;
}

/**
 * @brief 解析输入的文本文件
 * @param filename 文件名
 * @return char*
 */
char *parse_file(char *des) {
	char *r = des;
	// 过滤回车/换行
	r[strlen(r) - 1] = '\0';
	FILE *fp		 = fopen(r + 2, "r"); // 从第三个字符开始属于文本文件名
	// 读取文件内容
	fgets(r, 256, fp);
	fclose(fp);
	return des;
}

/**
 * @brief 解析输入，判断输入是否合法以及查询模式
 * @param putin 读入的字符串
 * @return char
 */
char parse_input(const char *putin) {
	if(strchr(putin, 'q') && strlen(putin) == 2) // 退出
		return 'q';
	if(strchr(putin, 'f') && strlen(putin) > 2) // 从文本中读入
		return 'f';
	return IS_PUTIN_OK(putin); // 判断输入是否合法
}

/**
 * @brief 从字符串中解析要查询的 IP
 * @param source 读入的字符串
 * @return iterator*
 */
iterator *parse_ip(char *source) {
	inner_itor *p		= NULL;
	ip *		current = NULL;
	// 申请链表
	current = calloc(sizeof(*current), 1);
	p		= calloc(sizeof(*p), 1);
	if(p == NULL)
		return NULL;
	p->item = current;
	// 分割输入
	char *index	= NULL;
	char *result   = NULL;
	char  delims[] = " \t\n";
	result		   = strtok(source, delims);
	while(result != NULL) {
		index				 = result + strlen(result) + 1;
		bool  has_bar		 = false;  // 是否具有连字符
		bool  has_wildcard   = false;  // 是否具有通配符
		char *address_result = result; // 保存当前字符串地址
		while(*result) {
			if(*result == '-') { // 具有连字符
				has_bar = true;
				break;
			} else if(*result == '*' || *result == '?') { // 具有通配符
				has_wildcard = true;
				break;
			}
			result++;
		}
		result = address_result; // 重置字符串
		// ip: a1.b1.c1.d1-a2.b2.c2.d2
		if(has_bar) {
			ip st, ed;
			sscanf(result, "%hu.%hu.%hu.%hu-%hu.%hu.%hu.%hu", &st.ip[0], &st.ip[1], &st.ip[2],
				   &st.ip[3], &ed.ip[0], &ed.ip[1], &ed.ip[2], &ed.ip[3]);
			IP_LIST_INSERT_CONTINUOUS(&st, &ed, &current);
		}	  // ip: a.b.c.d
		else { // 存在通配符
			if(has_wildcard) {
				IP_LIST_INSERT_WILDCARD(result, &current);
			} else { // 单个查询
				ip node;
				sscanf(result, "%hu.%hu.%hu.%hu", &node.ip[0], &node.ip[1], &node.ip[2],
					   &node.ip[3]);
				IP_LIST_INSERT(&node, current);
				current = current->next; // 更新链表
			}
		}
		// 继续分割
		result = strtok(index, delims);
	}
	// 创建一个迭代器
	return iterator_new((void *)current, (void *)p, ip_list_has_next, ip_list_get_next);
}
