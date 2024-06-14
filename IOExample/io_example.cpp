#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

// 链接时需要链接Ws2_32.lib库
#pragma comment(lib, "Ws2_32.lib")

void handle_client(const SOCKET client_socket) {
    char buffer[1024];

    // 接收消息
    if (const int result = recv(client_socket, buffer, sizeof(buffer), 0); result > 0) {
        buffer[result] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // 发送响应
        const auto response = "Hello, Client!";
        send(client_socket, response, strlen(response), 0);
    } else if (result == 0) {
        std::cout << "Connection closing..." << std::endl;
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // 关闭客户端套接字
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    SOCKET listen_socket = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;
    struct sockaddr_in server_addr;

    // 初始化Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // 创建套接字
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET) {
        std::cerr << "Error opening socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8500);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字
    result = bind(listen_socket, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // 监听连接
    result = listen(listen_socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error listening on socket: " << WSAGetLastError() << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 8500..." << std::endl;

    while (true) {
        // 接受客户端连接
        client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            closesocket(listen_socket);
            WSACleanup();
            return 1;
        }

        // 处理客户端连接
        handle_client(client_socket);
    }

    // 关闭监听套接字
    closesocket(listen_socket);
    WSACleanup();
    return 0;
}
