#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <stdbool.h>
#include <stdlib.h>

typedef bool (*HAS_NEXT_HOOK_FUNC)(void *container_instance, void *container_inner_itor);
typedef void *(*GET_NEXT_HOOK_FUNC)(void *container_instance, void *container_inner_itor);

struct iterator {
	void *			   container_instance;
	void *			   container_inner_itor;
	HAS_NEXT_HOOK_FUNC _has_next;
	GET_NEXT_HOOK_FUNC _get_next;
};
typedef struct iterator iterator;

iterator *iterator_new(void *container_instance, void *container_inner_itor, HAS_NEXT_HOOK_FUNC h,
					   GET_NEXT_HOOK_FUNC g);

bool  has_next(iterator *itor);
void *get_next(iterator *itor);
void  iterator_free(iterator *itor);

#endif