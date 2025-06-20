#ifndef COOKIEPARSERIMPL_HPP
#define COOKIEPARSERIMPL_HPP

#include "CookieParser.hpp"

class CookieParserImpl : public CookieParser {
public:
    CookieParserImpl();
    ~CookieParserImpl() override;

    std::vector<std::pair<std::string, std::string>> parse(
        const std::string& cookieHeader,
        std::unordered_map<std::string, std::string>& cookies
    ) override;

private:
    std::string trim(const std::string& s);
};

#endif // COOKIEPARSERIMPL_HPP
