/**
 * @file 		iterator.c
 * @brief 		一个迭代器的简单实现
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       	2020-02-09
 */
#include "iterator.h"

/**
 * @brief 创建一个迭代器
 * @param container_instance 容器类型
 * @param container_inner_itor 适用于容器的内建迭代器
 * @param h 判断容器是否还有可返回的元素的函数名
 * @param g 得到容器的下一个元素的函数名
 * @return iterator*
 */
iterator *iterator_new(void *container_instance, void *container_inner_itor, HAS_NEXT_HOOK_FUNC h,
					   GET_NEXT_HOOK_FUNC g) {
	iterator *itor = NULL;

	// 申请内存
	itor = calloc(1, sizeof(*itor));
	if(!itor)
		return NULL;

	// 初始化
	itor->container_instance   = container_instance;
	itor->container_inner_itor = container_inner_itor;
	itor->_has_next			   = h;
	itor->_get_next			   = g;

	return itor;
}

/**
 * @brief 判断容器是否还有可返回的元素
 * @param itor 迭代器
 * @return bool
 */
bool has_next(iterator *itor) {
	return itor->_has_next(itor->container_instance, itor->container_inner_itor);
}

/**
 * @brief 得到容器的下一个元素
 * @param itor 迭代器
 */
void *get_next(iterator *itor) {
	return itor->_get_next(itor->container_instance, itor->container_inner_itor);
}

/**
 * @brief 释放迭代器
 * @param itor 迭代器
 */
void iterator_free(iterator *itor) {
	if(itor) {						   // 如果存在迭代器
		if(itor->container_inner_itor) // 如果存在内建迭代器
			free(itor->container_inner_itor);
		free(itor);
	}
}