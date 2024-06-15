cmake_minimum_required(VERSION 3.18)
project(thread_example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(coroutine coroutine_example.cpp)

