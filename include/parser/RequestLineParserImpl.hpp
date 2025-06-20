#ifndef REQUESTLINEPARSERIMPL_HPP
#define REQUESTLINEPARSERIMPL_HPP

#include "RequestLineParser.hpp"

class RequestLineParserImpl : public RequestLineParser {
public:
    RequestLineParserImpl();
    ~RequestLineParserImpl() override;

    void parse(
        const std::string& requestLine,
        std::string& method,
        std::string& path,
        std::string& protocol,
        std::string& version
    ) override;
};

#endif // REQUESTLINEPARSERIMPL_HPP
