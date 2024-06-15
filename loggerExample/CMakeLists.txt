cmake_minimum_required(VERSION 3.18)
project(logger_example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS log)
find_package(spdlog CONFIG REQUIRED)

add_executable(logger_example logger.cpp)
target_link_libraries(logger_example PRIVATE Boost::log spdlog::spdlog)
