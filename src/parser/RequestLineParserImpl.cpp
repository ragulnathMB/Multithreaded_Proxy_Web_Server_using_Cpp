#include "../../include/parser/RequestLineParserImpl.hpp"
#include <stdexcept>

RequestLineParserImpl::RequestLineParserImpl() = default;
RequestLineParserImpl::~RequestLineParserImpl() = default;

void RequestLineParserImpl::parse(
    const std::string& requestLine,
    std::string& method,
    std::string& path,
    std::string& protocol,
    std::string& version
) {
    size_t firstSpace = requestLine.find(' ');
    if (firstSpace == std::string::npos) {
        throw std::runtime_error("Invalid request line: missing method");
    }

    method = requestLine.substr(0, firstSpace);

    size_t secondSpace = requestLine.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos) {
        throw std::runtime_error("Invalid request line: missing path or version");
    }

    path = requestLine.substr(firstSpace + 1, secondSpace - firstSpace - 1);

    std::string protocolVersion = requestLine.substr(secondSpace + 1);
    size_t slashPos = protocolVersion.find('/');
    if (slashPos == std::string::npos) {
        throw std::runtime_error("Invalid protocol/version format");
    }

    protocol = protocolVersion.substr(0, slashPos);
    version = protocolVersion.substr(slashPos + 1);
}
