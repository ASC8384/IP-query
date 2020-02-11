# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (IP-query)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/./bin)

#设置版本号
set (Demo_VERSION_MAJOR 1.1)
set (Demo_VERSION_MINOR 6)

# 头文件
include_directories(./lib/iniparser)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)

# 添加子目录
add_subdirectory(./lib/iniparser)

# 指定生成目标
add_executable(ipquery ${DIR_SRCS})

# 添加链接库
target_link_libraries(ipquery iniparser)