#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

void handle_client(tcp::socket&socket) {
    try {
        char data[1024];

        // 读取客户端消息
        const size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "Received from client: " << std::string(data, length) << std::endl;

        // 发送响应
        std::string message = "Hello, Client!";
        boost::asio::write(socket, boost::asio::buffer(message));

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    try {
        constexpr int port_num = 8080;
        boost::asio::io_context io_context;

        // 创建并打开一个TCP接受器
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port_num));

        while (true) {
            // 等待并接受连接
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // 处理客户端
            handle_client(socket);
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
