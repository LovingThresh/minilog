cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(Boost 1.85.0 REQUIRED COMPONENTS scope_exit)
find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(exampleGenerics main.cpp)
target_link_libraries(exampleGenerics PRIVATE Boost::scope_exit ${OpenCV_LIBS})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(exampleGenerics PRIVATE -Weffc++)
endif ()

#add test subdir
add_subdirectory(test)