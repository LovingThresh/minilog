cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)
project(minilog LANGUAGES CXX)

add_executable(example main.cpp
        minilog.h
)

add_executable(example_uniqueP learning_pointer.cpp
        StrBlob.h
        StrBlob.cpp
        StrBlobPtr.cpp
        StrBlobPtr.h)
add_executable(example_c_string cstyle_string.cpp)

target_include_directories(example PUBLIC .)

add_subdirectory(threadExample)
#add_subdirectory(coreutilsExample)
add_subdirectory(coroutinesExample)
add_subdirectory(chronoExample)
add_subdirectory(roundRobinExample)
add_subdirectory(IOExample)
add_subdirectory(loggerExample)