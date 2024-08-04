cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(factorySample factoryExample.cpp)
add_executable(salesSample SalesData.cpp)
add_executable(screenSample Screen.cpp)
add_executable(hasPtrSample HasPtrValue.cpp)
add_executable(StrVecSample StrVec.hpp main_str.cpp)
add_executable(MessageSample Message.hpp main.cpp)