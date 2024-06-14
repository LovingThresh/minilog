#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// 链接时需要链接Ws2_32.lib库
#pragma comment(lib, "Ws2_32.lib")

HANDLE shutdown_event;

void handle_client(const SOCKET client_socket) {
    char buffer[1024];

    // 接收消息
    if (const int result = recv(client_socket, buffer, sizeof(buffer), 0); result > 0) {
        buffer[result] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // 发送响应
        const char* response = "Hello, Client, I am Server!";
        send(client_socket, response, strlen(response), 0);
    } else if (result == 0) {
        std::cout << "Connection closing..." << std::endl;
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // 关闭客户端套接字
    closesocket(client_socket);
}

DWORD WINAPI server_thread(LPVOID lpParam) {
    const SOCKET listen_socket = reinterpret_cast<SOCKET>(lpParam);
    SOCKET client_socket = INVALID_SOCKET;

    while (WaitForSingleObject(shutdown_event, 0) == WAIT_TIMEOUT) {
        // 接受客户端连接
        client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 处理客户端连接
        handle_client(client_socket);
    }

    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET listen_socket = INVALID_SOCKET;
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

    // 创建一个事件对象来通知服务器关闭
    shutdown_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (shutdown_event == nullptr) {
        std::cerr << "CreateEvent failed: " << GetLastError() << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // 创建服务器线程
    const HANDLE server_thread_handle = CreateThread(nullptr, 0, server_thread,
        reinterpret_cast<LPVOID>(listen_socket), 0, nullptr);
    if (server_thread_handle == nullptr) {
        std::cerr << "CreateThread failed: " << GetLastError() << std::endl;
        CloseHandle(shutdown_event);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Press Enter to stop the server..." << std::endl;
    std::cin.get();

    // 通知服务器线程停止
    SetEvent(shutdown_event);

    // 等待服务器线程退出
    WaitForSingleObject(server_thread_handle, INFINITE);

    // 清理资源
    CloseHandle(server_thread_handle);
    CloseHandle(shutdown_event);
    closesocket(listen_socket);
    WSACleanup();

    return 0;
}
