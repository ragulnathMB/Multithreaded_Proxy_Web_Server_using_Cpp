#include "../../include/Logger/Logger.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <mutex>

static std::mutex loggerMutex;

Logger& Logger::getInstance(const std::string& filename) {
    static Logger instance(filename);
    return instance;
}

Logger::Logger(const std::string& filename) {
    this->filename = filename;
}

Logger::~Logger() {}

void Logger::log(const std::string& message, LoggerLevel loggerLevel) {
    std::lock_guard<std::mutex> lock(loggerMutex);

    std::ofstream logFile(filename, std::ios::app);
    std::string curTime = getTimeStamp();
    std::string levelCategory = getLevelString(loggerLevel);

    std::string fullMessage = "[timeStamp: " + curTime + "][" + levelCategory + "] : " + message;
    std::cout << fullMessage << std::endl;

    if (logFile.is_open()) {
        logFile << fullMessage << std::endl;
    }
}

std::string Logger::getTimeStamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    return std::string(buffer);
}

std::string Logger::getLevelString(LoggerLevel level) {
    switch (level) {
        case LoggerLevel::INFO: return "INFO";
        case LoggerLevel::WARNING: return "WARNING";
        case LoggerLevel::ERR: return "ERROR";
        case LoggerLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}
