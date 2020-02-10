/**
 * @file 		iterator.h
 * @brief 		迭代器的简单实现
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       	2020-02-09
 */
#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <stdbool.h>
#include <stdlib.h>

typedef bool (*HAS_NEXT_HOOK_FUNC)(void *container_instance, void *container_inner_itor);
typedef void *(*GET_NEXT_HOOK_FUNC)(void *container_instance, void *container_inner_itor);

/**
 * @brief 定义迭代器
 * container_instance  容器类型
 * container_inner_itor 适用于容器的内建迭代器
 * _has_next 判断容器是否还有可返回的元素的函数
 * _get_next 得到容器的下一个元素的函数
 */
typedef struct iterator {
	void *			   container_instance;
	void *			   container_inner_itor;
	HAS_NEXT_HOOK_FUNC _has_next;
	GET_NEXT_HOOK_FUNC _get_next;
} iterator;

/**
 * @brief 创建一个迭代器
 * @param container_instance 容器类型
 * @param container_inner_itor 适用于容器的内建迭代器
 * @param h 判断容器是否还有可返回的元素的函数名
 * @param g 得到容器的下一个元素的函数名
 * @return iterator*
 */
iterator *iterator_new(void *container_instance, void *container_inner_itor, HAS_NEXT_HOOK_FUNC h,
					   GET_NEXT_HOOK_FUNC g);

/**
 * @brief 判断容器是否还有可返回的元素
 * @param itor 迭代器
 * @return bool
 */
bool has_next(iterator *itor);

/**
 * @brief 得到容器的下一个元素
 * @param itor 迭代器
 */
void *get_next(iterator *itor);

/**
 * @brief 释放迭代器
 * @param itor 迭代器
 */
void iterator_free(iterator *itor);

#endif