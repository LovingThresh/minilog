cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(Boost 1.85.0 REQUIRED COMPONENTS scope_exit)
find_package(OpenCV REQUIRED)
find_package(absl CONFIG REQUIRED)

# 打印 OpenCV 版本信息
message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")

# 添加包含目录
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Sharpen main.cpp)
target_link_libraries(Sharpen PRIVATE Boost::scope_exit ${OpenCV_LIBS} absl::base)