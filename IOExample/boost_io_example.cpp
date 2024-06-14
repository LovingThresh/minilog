#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

void session(tcp::socket& sock) {
    try {
        // 发送消息
        std::string message = "Hello, Server!";
        boost::asio::write(sock, boost::asio::buffer(message));

        // 接收响应
        char buffer[1024];
        const auto length = sock.read_some(boost::asio::buffer(buffer));
        std::cout << "Received from server: " << std::string(buffer, length) << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    try {
        constexpr int port_num = 8080;
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        const auto endpoints = resolver.resolve("127.0.0.1", std::to_string(port_num));
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        session(socket);

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
