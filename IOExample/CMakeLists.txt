cmake_minimum_required(VERSION 3.18)
project(io_example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS asio)

add_executable(io_example io_example.cpp)
add_executable(tcp_example tcp_example.cpp)
add_executable(boost_asio_example boost_io_example.cpp)

target_link_libraries(boost_asio_example PRIVATE Boost::asio)

add_executable(boost_tcp_example boost_tcp_example.cpp)
target_link_libraries(boost_tcp_example PRIVATE Boost::asio)

add_executable(chat_client chat_client.cpp chat_message.h)
target_link_libraries(chat_client PRIVATE Boost::asio)

add_executable(chat_server chat_server.cpp chat_message.h)
target_link_libraries(chat_server PRIVATE Boost::asio)
