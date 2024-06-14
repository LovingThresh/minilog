#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

// 链接时需要链接Ws2_32.lib库
#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sockfd = INVALID_SOCKET;
    struct sockaddr_in server_addr;

    // 初始化Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error opening socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8500);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 连接服务器
    result = connect(sockfd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error connecting: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // 发送消息
    const char* message = "Hello, Server, I am Client!";
    result = send(sockfd, message, strlen(message), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // 接收响应
    char buffer[1024];
    result = recv(sockfd, buffer, sizeof(buffer), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error receiving response: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    buffer[result] = '\0';
    std::cout << "Received from server: " << buffer << std::endl;

    // 关闭套接字
    closesocket(sockfd);
    WSACleanup();
    return 0;
}

