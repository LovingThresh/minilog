cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(unifex CONFIG REQUIRED)
find_package(stdexec CONFIG REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE unifex::unifex STDEXEC::stdexec)