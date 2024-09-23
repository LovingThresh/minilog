cmake_minimum_required(VERSION 3.18)
project(thread_example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)
add_executable(thread_example thread_example.cpp)
add_executable(mutex_example mutex_example.cpp)
add_executable(thread_main_example main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(thread_main_example PUBLIC Threads::Threads)
