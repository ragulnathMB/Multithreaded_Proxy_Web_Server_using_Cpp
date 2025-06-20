#pragma once
#include <string>

enum class LoggerLevel {
    INFO,
    WARNING,
    ERR,
    DEBUG
};

class Logger {
public:
    // Singleton access point
    static Logger& getInstance(const std::string& filename = "ProxyServerLog.log");

    // Logging method
    void log(const std::string& message, LoggerLevel loggerLevel = LoggerLevel::INFO);

private:
    // Private constructor & destructor
    Logger(const std::string& filename);
    ~Logger();

    // Delete copy operations
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Helpers
    std::string getTimeStamp();
    std::string getLevelString(LoggerLevel level);

    // Member
    std::string filename;
};
