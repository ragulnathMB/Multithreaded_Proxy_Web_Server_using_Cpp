#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void makeRequest(int clientId, const std::string& host, const std::string& path) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[Client " << clientId << "] Socket creation failed.\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Proxy Server Port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Proxy IP

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[Client " << clientId << "] Connection failed.\n";
        closesocket(sock);
        return;
    }

    // Create HTTP GET request
    std::string request =
        "GET " + path + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Connection: close\r\n"
        "Authorization: Basic YWRtaW46cGFzc3dvcmQ=\r\n" // base64(admin:password)
        "\r\n";

    send(sock, request.c_str(), static_cast<int>(request.size()), 0);

    // Receive and print response
    char buffer[4096];
    int bytesReceived;
    std::cout << "[Client " << clientId << "] Response:\n";

    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }

    std::cout << "\n[Client " << clientId << "] Done.\n";

    closesocket(sock);
    WSACleanup();
}

int main() {
    std::thread t1(makeRequest, 1, "youtube.com", "/");
    std::thread t2(makeRequest, 2, "example.com", "/");
    std::thread t3(makeRequest, 3, "example.com", "/");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
