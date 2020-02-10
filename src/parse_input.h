/**
 * @file 		parse_input.h
 * @brief 		解析输入
 * @author 		ASC_8384<ASC_8384@foxmail.com>
 * @version    	1.1.2
 * @date       	2020-02-09
 */
#ifndef PAESE_INPUT_H_
#define PAESE_INPUT_H_

#include "iterator.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 定义 ip 结构体
 * ip[4] ip 的四段
 * *next 指向下一结构体的指针，为链表而设
 */
typedef struct IP {
	// ip[i] == ip_i - 1
	uint16_t   ip[4];
	struct IP *next;
} ip;

/**
 * @brief 为 ip 迭代器设计的内建结构体
 */
typedef struct ip_list_inner_itor {
	ip *item;
} inner_itor;

/**
 * @brief 将 IP 地址插入链表
 * @param item 要插入的 IP 地址
 * @param list 当前链表的尾端
 * @return bool
 */
bool IP_LIST_INSERT(const ip *item, ip *list);

/**
 * @brief 将一个连续的 IP 地址段插入链表中
 * @param st 开始的 IP 地址
 * @param ed 结束的 IP 地址
 * @param current 当前容器（链表）的尾端
 */
void IP_LIST_INSERT_CONTINUOUS(ip *st, ip *ed, ip **current);

/**
 * @brief 将带有通配符的 IP 地址插入链表中
 * @param result 当前带有通配符的 IP 地址的字符串
 * @param current 当前容器（链表）的尾端的地址
 */
void IP_LIST_INSERT_WILDCARD(const char *result, ip **current);

/**
 * @brief 判断输入是否合法的具体实现
 * @param putin 输入的字符串
 * @return char
 */
char IS_PUTIN_OK(const char *putin);

/**
 * @brief 判断容器（链表）是否还有可返回的元素
 * @param filename 文件名
 * @return char*
 */
static bool ip_list_has_next(void *container_instance, void *container_inner_itor);

/**
 * @brief 返回容器（链表）的下一个元素
 * @param filename 文件名
 * @return ip*
 */
static void *ip_list_get_next(void *container_instance, void *container_inner_itor);

/**
 * @brief 解析输入的文本文件
 * @param filename 文件名
 * @return char*
 */
char *parse_file(char *des);

/**
 * @brief 解析输入，判断输入是否合法以及查询模式
 * @param putin 读入的字符串
 * @return char
 */
char parse_input(const char *putin);

/**
 * @brief 从字符串中解析要查询的 IP
 * @param source 读入的字符串
 * @return iterator*
 */
iterator *parse_ip(char *source);

#endif