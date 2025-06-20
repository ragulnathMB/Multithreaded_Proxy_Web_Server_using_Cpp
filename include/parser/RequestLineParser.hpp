#ifndef REQUESTLINEPARSER_HPP
#define REQUESTLINEPARSER_HPP

#include <string>

class RequestLineParser {
public:
    virtual ~RequestLineParser() = default;

    virtual void parse(
        const std::string& requestLine,
        std::string& method,
        std::string& path,
        std::string& protocol,
        std::string& version
    ) = 0;
};

#endif // REQUESTLINEPARSER_HPP
