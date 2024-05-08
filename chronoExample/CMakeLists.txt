cmake_minimum_required(VERSION 3.18)
project(thread_example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(chrono_example chrono_example.cpp)