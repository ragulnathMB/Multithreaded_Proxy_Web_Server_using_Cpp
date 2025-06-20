#ifndef COOKIEPARSER_HPP
#define COOKIEPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

class CookieParser {
public:
    virtual ~CookieParser() = default;

    virtual std::vector<std::pair<std::string, std::string>> parse(
        const std::string& cookieHeader,
        std::unordered_map<std::string, std::string>& cookies
    ) = 0;
};

#endif // COOKIEPARSER_HPP
