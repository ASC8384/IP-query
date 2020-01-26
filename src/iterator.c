#include "iterator.h"

iterator *iterator_new(void *container_instance, void *container_inner_itor, HAS_NEXT_HOOK_FUNC h,
					   GET_NEXT_HOOK_FUNC g) {
	iterator *itor = NULL;

	itor = calloc(1, sizeof(*itor));
	if(!itor)
		return NULL;

	itor->container_instance   = container_instance;
	itor->container_inner_itor = container_inner_itor;
	itor->_has_next			   = h;
	itor->_get_next			   = g;

	return itor;
}

void iterator_free(iterator *itor) {
	if(itor) {
		if(itor->container_inner_itor)
			free(itor->container_inner_itor);
		free(itor);
	}
}

bool has_next(iterator *itor) {
	return itor->_has_next(itor->container_instance, itor->container_inner_itor);
}

void *get_next(iterator *itor) {
	return itor->_get_next(itor->container_instance, itor->container_inner_itor);
}
