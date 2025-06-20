#include "../../include/parser/HeaderParserImpl.hpp"
#include <sstream>
#include <stdexcept>

HeaderParserImpl::HeaderParserImpl() = default;
HeaderParserImpl::~HeaderParserImpl() = default;

void HeaderParserImpl::parse(const std::string& headerSection, std::unordered_map<std::string, std::string>& headers) {
    std::istringstream stream(headerSection);
    std::string line;

    while (std::getline(stream, line)) {
        // Remove trailing \r if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Split key and value
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            throw std::runtime_error("Invalid header format: " + line);
        }

        std::string key = trim(line.substr(0, colonPos));
        std::string value = trim(line.substr(colonPos + 1));

        headers[key] = value;
    }
}

std::string HeaderParserImpl::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}
