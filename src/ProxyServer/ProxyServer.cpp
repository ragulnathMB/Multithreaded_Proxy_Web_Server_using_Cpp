#include "../../include/ProxyServer/ProxyServer.hpp"
#include "../../include/Logger/Logger.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <ctime>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

ProxyServer::ProxyServer():pool(10) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        Logger::getInstance().log("Proxy Server failed to initialize Winsock!", LoggerLevel::ERR);
        exit(EXIT_FAILURE);
    }
    serverSocket = INVALID_SOCKET;
}

ProxyServer::~ProxyServer() {
    Logger::getInstance().log("Proxy Server has been shut down.", LoggerLevel::INFO);
    closesocket(serverSocket);
    WSACleanup();
}

void ProxyServer::start(unsigned short port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        Logger::getInstance().log("Server socket creation failed.", LoggerLevel::ERR);
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::getInstance().log("Bind failed on port: " + std::to_string(port), LoggerLevel::ERR);
        closesocket(serverSocket);
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        Logger::getInstance().log("Listen failed.", LoggerLevel::ERR);
        closesocket(serverSocket);
        return;
    }

    Logger::getInstance().log("Proxy server started on port: " + std::to_string(port), LoggerLevel::INFO);
    acceptClients();
}

void ProxyServer::acceptClients() {
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            pool.enqueue([this, clientSocket]() {
                this->handleClient(clientSocket);
            });

        } else {
            Logger::getInstance().log("Failed to accept client connection.", LoggerLevel::WARNING);
        }
    }
}

void ProxyServer::handleClient(SOCKET clientSocket) {
    char buffer[8192];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';
    std::string request(buffer);
    std::string ip = extractIPFromSocket(clientSocket);

    Logger::getInstance().log("Received request from IP: " + ip, LoggerLevel::INFO);

    if (isIPBlacklisted(ip)) {
        Logger::getInstance().log("Blocked request from blacklisted IP: " + ip, LoggerLevel::WARNING);
        sendForbiddenResponse(clientSocket, "IP blacklisted");
        closesocket(clientSocket);
        return;
    }

    std::string host = extractHostFromRequest(request);
    if (isHostBlacklisted(host)) {
        Logger::getInstance().log("Blocked request to blacklisted host: " + host + " from IP: " + ip, LoggerLevel::WARNING);
        sendForbiddenResponse(clientSocket, "Host blacklisted");
        closesocket(clientSocket);
        return;
    }

    if (!parseAuth(request)) {
        Logger::getInstance().log("Unauthorized request from IP: " + ip, LoggerLevel::WARNING);
        sendAuthRequiredResponse(clientSocket);
        closesocket(clientSocket);
        return;
    }

    std::string cacheKey = "GET " + host + request.substr(request.find(" ") + 1, request.find(" HTTP") - request.find(" ") - 1);
    std::string response;

    if (getCachedResponse(cacheKey, response)) {
        Logger::getInstance().log("Cache hit for host: " + host + " (IP: " + ip + ")", LoggerLevel::INFO);
        sendResponseToClient(clientSocket, response);
    } else {
        Logger::getInstance().log("Cache miss for host: " + host + " (IP: " + ip + ")", LoggerLevel::INFO);

        if (sendRequestToServer(host, request, response)) {
            Logger::getInstance().log("Fetched response from server: " + host, LoggerLevel::DEBUG);
            addToCache(cacheKey, response);
            Logger::getInstance().log("Cached response for host: " + host, LoggerLevel::INFO);
            sendResponseToClient(clientSocket, response);
        } else {
            Logger::getInstance().log("Failed to fetch response from host: " + host, LoggerLevel::ERR);
        }
    }

    logRequest(ip, host);
    closesocket(clientSocket);
}

std::string ProxyServer::extractHostFromRequest(const std::string& request) {
    size_t hostPos = request.find("Host: ");
    if (hostPos == std::string::npos) return "";
    size_t start = hostPos + 6;
    size_t end = request.find("\r\n", start);
    return request.substr(start, end - start);
}

std::string ProxyServer::extractIPFromSocket(SOCKET socket) {
    sockaddr_in addr;
    int addrLen = sizeof(addr);
    getpeername(socket, (SOCKADDR*)&addr, &addrLen);
    char* ipStr = inet_ntoa(addr.sin_addr);
    return std::string(ipStr ? ipStr : "unknown");
}

bool ProxyServer::parseAuth(const std::string& request) {
    return request.find("Authorization: Basic ") != std::string::npos;
}

bool ProxyServer::sendRequestToServer(const std::string& host, const std::string& request, std::string& response) {
    addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) {
        return false;
    }

    SOCKET remoteSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (remoteSocket == INVALID_SOCKET || connect(remoteSocket, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        freeaddrinfo(res);
        closesocket(remoteSocket);
        return false;
    }

    send(remoteSocket, request.c_str(), (int)request.size(), 0);

    char buffer[8192];
    int bytes;
    response.clear();

    while ((bytes = recv(remoteSocket, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytes);
    }

    closesocket(remoteSocket);
    freeaddrinfo(res);
    return true;
}

void ProxyServer::sendResponseToClient(SOCKET clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), (int)response.size(), 0);
}

bool ProxyServer::isIPBlacklisted(const std::string& ipAddress) {
    return blacklistedIPs.count(ipAddress);
}

bool ProxyServer::isHostBlacklisted(const std::string& hostAddress) {
    return blacklistedHosts.count(hostAddress);
}

bool ProxyServer::isRateLimited(const std::string& ipAddress) {
    std::lock_guard<std::mutex> lock(rateLimitMutex);
    return ++requestCount[ipAddress] > maxRequestsPerMinute;
}

bool ProxyServer::getCachedResponse(const std::string& key, std::string& response) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    auto it = cache.find(key);
    if (it != cache.end()) {
        response = it->second;
        return true;
    }
    return false;
}

void ProxyServer::addToCache(const std::string& key, const std::string& response) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache[key] = response;
}

void ProxyServer::logRequest(const std::string& ip, const std::string& host) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::time_t now = std::time(nullptr);
    std::cout << "[" << std::ctime(&now) << "] " << ip << " -> " << host << std::endl;
}

void ProxyServer::sendForbiddenResponse(SOCKET clientSocket, const std::string& reason) {
    std::string response = "HTTP/1.1 403 Forbidden\r\nContent-Length: " + std::to_string(reason.length()) +
        "\r\nContent-Type: text/plain\r\n\r\n" + reason;
    sendResponseToClient(clientSocket, response);
}

void ProxyServer::sendAuthRequiredResponse(SOCKET clientSocket) {
    std::string response = "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm=\"Proxy\"\r\nContent-Length: 0\r\n\r\n";
    sendResponseToClient(clientSocket, response);
}
