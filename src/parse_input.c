#include "parse_input.h"

bool IP_LIST_INSERT(ip *item, ip *list) {
	ip *pnew;
	ip *scan = list;

	pnew = calloc(sizeof(*pnew), 1);
	if(pnew == NULL)
		return false;

	for(int i = 0; i <= 3; i++)
		pnew->ip[i] = item->ip[i];
	scan->next = pnew;

	return true;
}

static bool ip_list_has_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor;
	ip *		item = p->item;

	return !(item->next == NULL);
}

static void *ip_list_get_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor;
	ip *		item = p->item;

	p->item = item->next;

	return p->item;
}

char *parse_file(char *des) {
	char *r			 = des;
	r[strlen(r) - 1] = '\0';
	FILE *fp		 = fopen(r + 2, "r");
	fgets(r, 256, fp);
	fclose(fp);
	return des;
}

char parse_input(const char *putin) {
	if(strchr(putin, 'q') && strlen(putin) == 2)
		return 'q';
	if(strchr(putin, 'f') && strlen(putin) > 2)
		return 'f';

	int count_point = 0;
	int count_bar   = 0;
	int count_num   = 0;
	int temp_num	= 0;
	int i			= 0;
	while(*(putin + i)) {
		if(isdigit(putin[i])) {
			if(temp_num == -1)
				temp_num = 0;
			temp_num = temp_num * 10 + putin[i] - '0';
		} else if(putin[i] == '.') {
			if(temp_num > 255 || temp_num == -1)
				return 'w';
			temp_num = -1;
			count_num++;
			count_point++;
		} else if(putin[i] == '-') {
			if(temp_num > 255 || temp_num == -1)
				return 'w';
			temp_num = -1;
			count_num++;
			count_bar++;
		} else if(putin[i] == ' ' || putin[i] == '\t') {
			if(temp_num > 255)
				return 'w';
			if(temp_num != -1)
				count_num++;
			temp_num = -1;
		} else if(putin[i] == '\n') {
			if(temp_num > 255 || temp_num == -1)
				return 'w';
			if(temp_num != -1)
				count_num++;
			temp_num = -1;
		} else if(putin[i] == '*') {
		} else if(putin[i] == '?') {
		} else {
			return 'w';
		}
		i++;
	}
	if(count_bar > 1)
		return 'w';
	if(count_point % 3 != 0 || count_point == 0)
		return 'w';
	if(count_num % 4 != 0 || count_num == 0 || (count_num != 8 && count_bar == 1))
		return 'w';

	return 'y';
}

iterator *parse_ip(char *source) {
	inner_itor *p		= NULL;
	ip *		current = NULL;

	current = calloc(sizeof(*current), 1);
	p		= calloc(sizeof(*p), 1);

	if(p == NULL)
		return NULL;
	p->item = current;

	int count_point = 0;
	int count_bar   = 0;
	int i			= 0;
	while(*(source + i)) {
		if(source[i] == '.') {
			count_point++;
		} else if(source[i] == '-') {
			count_bar++;
		}
		i++;
	}

	// ip: a1.b1.c1.d1-a2.b2.c2.d2
	if(count_bar != 0) {
		ip st, ed;
		sscanf(source, "%hd.%hd.%hd.%hd-%hd.%hd.%hd.%hd", &st.ip[0], &st.ip[1], &st.ip[2],
			   &st.ip[3], &ed.ip[0], &ed.ip[1], &ed.ip[2], &ed.ip[3]);
		if(st.ip[0] > ed.ip[0] || st.ip[1] > ed.ip[1] || st.ip[2] > ed.ip[2] || st.ip[3] > ed.ip[3])
			return NULL;
		while(!(st.ip[0] == ed.ip[0] && st.ip[1] == ed.ip[1] && st.ip[2] == ed.ip[2] &&
				st.ip[3] == ed.ip[3] + 1)) {
			IP_LIST_INSERT(&st, current);
			current = current->next;
			// ip address increase progressively
			st.ip[3]++;
			if(st.ip[3] == 256) {
				st.ip[3] = 0;
				st.ip[2]++;
				if(st.ip[2] == 256) {
					st.ip[2] = 0;
					st.ip[1]++;
					if(st.ip[1] == 256) {
						st.ip[1] = 0;
						st.ip[0]++;
						if(st.ip[0] > 256)
							return NULL;
					}
				}
			}
		}
	} // ip: a.b.c.d[]
	else if(count_point >= 6) {
		ip	node;
		char *result   = NULL;
		char  delims[] = " ";

		result = strtok(source, delims);
		while(result != NULL) {
			sscanf(result, "%hd.%hd.%hd.%hd", &node.ip[0], &node.ip[1], &node.ip[2], &node.ip[3]);
			IP_LIST_INSERT(&node, current);
			current = current->next;
			result  = strtok(NULL, delims);
		}

	} // ip :a.b.c.d
	else {
		ip node;
		sscanf(source, "%hd.%hd.%hd.%hd", &node.ip[0], &node.ip[1], &node.ip[2], &node.ip[3]);
		IP_LIST_INSERT(&node, current);
	}

	return iterator_new((void *)current, (void *)p, ip_list_has_next, ip_list_get_next);
}
