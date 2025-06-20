#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <winsock2.h>
#include <mutex>
#include "../ThreadPool/ThreadPool.hpp"

class ProxyServer {
public:
    ProxyServer();
    ~ProxyServer();

    // Starts the proxy server on the given port
    void start(unsigned short port);

private:
    // Thread pool
    ThreadPool pool;
    // Core operations
    void acceptClients();
    void handleClient(SOCKET clientSocket);
    std::string extractHostFromRequest(const std::string& request);
    std::string extractIPFromSocket(SOCKET socket);
    bool parseAuth(const std::string& request);

    // Server communication
    bool sendRequestToServer(const std::string& host, const std::string& request, std::string& response);
    void sendResponseToClient(SOCKET clientSocket, const std::string& response);

    // Security features
    bool isIPBlacklisted(const std::string& ipAddress);
    bool isHostBlacklisted(const std::string& hostAddress);
    bool isRateLimited(const std::string& ipAddress);

    // Caching support
    bool getCachedResponse(const std::string& cacheKey, std::string& response);
    void addToCache(const std::string& cacheKey, const std::string& response);

    // Utility
    void logRequest(const std::string& ip, const std::string& host);
    void sendForbiddenResponse(SOCKET clientSocket, const std::string& reason);
    void sendAuthRequiredResponse(SOCKET clientSocket);

    // Networking
    SOCKET serverSocket;

    // Data structures
    std::unordered_set<std::string> blacklistedIPs;
    std::unordered_set<std::string> blacklistedHosts;
    std::unordered_map<std::string, std::string> cache;
    std::unordered_map<std::string, int> requestCount; // for rate limiting

    // Basic auth
    const std::string validUsername = "admin";
    const std::string validPassword = "password";

    // Rate limiting
    const int maxRequestsPerMinute = 60;

    // Thread safety
    std::mutex cacheMutex;
    std::mutex rateLimitMutex;
    std::mutex logMutex;
};
