# IP-query

## 概述

**IP地址查询系统**

一个IP地址段的数据库（文本文件），用户可以在交互环境中，输入ip地址，系统给出地址对应的国家，身份，城市，ISP等信息。

## Todo List

* [x] 支持查询形式为 a.b.c.d
* [x] 支持查询形式为 a1.b1.c1.d1-a2.b2.c2.d2
* [x] 支持查询形式为一次性多组数据
* [x] 输入非法时，能提示用户
* [x] 支持显示指定的信息，比如只显示国家或者中国，省份之类的
* [x] 支持文件操作（从文件输入）
* [x] 支持通配符
* [x] 支持命令行
* [x] 更快的查询速度
* [x] 适配 Linux
* [ ] 小内存情况下查询处理
* [ ] 支持显示排序，比如按省份排序之类的

## Change Log

### 1.0.0 (2020-02-08)

* 适配 Linux（Ubuntu 18.04.2 LTS 64位）

### 0.9.0 (2020-02-07)

* 加快查询速度（加快至少 1000 倍）

* 优化内存占用

### 0.8.2 (2020-02-04)

* 优化查询时的内存占用

### 0.8.1 (2020-02-03)

* 优化查询速度（加快10倍）

* 优化内存占用

### 0.8.0 (2020-02-03)

* 支持命令行

### 0.7.0 (2020-02-02)

* 支持通配符（ `?` 、 `*` ）

* 修复无法输出 x.x.x.0-x.x.x.255 的错误

### 0.6.1 (2020-02-01)

* 在 ANSI 兼容的终端有彩色显示

* 优化内存占用

### 0.6.0 (2020-01-31)

* 支持文件方式输入想查询的ip地址列表，输入 `f ` 后接文件地址

* 优化内存占用

### 0.5.0 (2020-01-29)

* 实现支持显示指定的信息，比如只显示国家或者中国，省份之类的

* 增加 ip-query.ini 配置文件

### 0.4.2 (2020-01-27)

* 修复输入非法提示的错误

### 0.4.1 (2020-01-27)

* 实现多次查询，输入 `q` 退出

### 0.4.0 (2020-01-27)

* 实现输入非法提示

### 0.3.0 (2020-01-26)

* 实现一次性多组数据的单次查询

### 0.2.0 (2020-01-26)

* 实现 a1.b1.c1.d1-a2.b2.c2.d2 形式的单次查询

### 0.1.1 (2020-01-25)

* 实现对 *迭代器(iterator)* 的支持

### 0.1.0 (2020-01-11)

* 实现 a.b.c.d 形式的单次查询

## 感谢与第三方库

1.[iniparser](https://github.com/ndevilla/iniparser)

    - Aother: Nicolas Devillard @ ndevilla，Emmanuel Leblond @TouilleMan

    - License: MIT

2.rbtree

    - Aother: (C) 1999  Andrea Arcangeli <andrea@suse.de>, (C) 2002  David Woodhouse <dwmw2@infradead.org>

    - License: GPL

3.[map](https://github.com/Broadroad/map)

    - Aother: [Broadroad](https://github.com/Broadroad), Andrea Arcangeli <andrea@suse.de>

    - License: GPL (grab from kernel)

