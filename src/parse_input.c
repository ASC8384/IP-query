#include "parse_input.h"

static bool ip_list_has_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor;
	ip *		item = p->item;

	return !(item->next == NULL);
}

static void *ip_list_get_next(void *container_instance, void *container_inner_itor) {
	inner_itor *p	= (inner_itor *)container_inner_itor;
	ip *		item = p->item;

	// p->item = IP_LIST_NEXT(item);
	p->item = item->next;

	return p->item;
}

char *input_string(char *des) {
	char *r = des;
	scanf("%s", r);
	return des;
}

iterator *parse(const char *source) {
	inner_itor *p		= NULL;
	ip *		head	= NULL;
	ip *		current = NULL;

	p		= calloc(sizeof(*p), 1);
	head	= calloc(sizeof(*head), 1);
	current = calloc(sizeof(*current), 1);

	if(!p)
		return NULL;
	p->item = head;

	if(!strchr(source, 0x2D)) {
		sscanf(source, "%d.%d.%d.%d", &current->ip[1], &current->ip[2], &current->ip[3],
			   &current->ip[4]);
		head->next = current;
	} else {
		ip st, ed;
		sscanf(source, "%d.%d.%d.%d-%d.%d.%d.%d", &st.ip[1], &st.ip[2], &st.ip[3], &st.ip[4],
			   &ed.ip[1], &ed.ip[2], &ed.ip[3], &ed.ip[4]);
	}

	return iterator_new((void *)head, (void *)p, ip_list_has_next, ip_list_get_next);
}