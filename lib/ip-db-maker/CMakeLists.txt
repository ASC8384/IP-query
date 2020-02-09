# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (IPdb-make)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/./bin)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(ipdbmaker ${DIR_SRCS})