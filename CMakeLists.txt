cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)
project(minilog LANGUAGES CXX)
# 在文件顶部设置 CMake 政策为 OLD
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()
# 禁用开发警告
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "Suppress developer warnings")
add_executable(example main.cpp
        minilog.h
        functionBind/main.cpp
)

add_executable(example_uniqueP learning_pointer.cpp
        StrBlob.h
        StrBlob.cpp
        StrBlobPtr.cpp
        StrBlobPtr.h)
add_executable(example_c_string cstyle_string.cpp)

target_include_directories(example PUBLIC .)

add_subdirectory(threadExample)
add_subdirectory(coreutilsExample)
add_subdirectory(coroutinesExample)
add_subdirectory(chronoExample)
add_subdirectory(roundRobinExample)
add_subdirectory(IOExample)
add_subdirectory(loggerExample)
add_subdirectory(FactoryExample)
add_subdirectory(printfExample)
add_subdirectory(functionExample)
add_subdirectory(vectorExample)
add_subdirectory(referenceExample)
add_subdirectory(functionBind)
add_subdirectory(pointerExample)
add_subdirectory(Generics)
add_subdirectory(ExampleOpenCV)
add_subdirectory(ConcurrencyExample)
add_subdirectory(rangesExample)