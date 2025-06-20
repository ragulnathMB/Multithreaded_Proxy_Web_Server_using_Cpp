#include "../../include/parser/CookieParserImpl.hpp"
#include <sstream>

CookieParserImpl::CookieParserImpl() = default;
CookieParserImpl::~CookieParserImpl() = default;

std::vector<std::pair<std::string, std::string>> CookieParserImpl::parse(
    const std::string& cookieHeader,
    std::unordered_map<std::string, std::string>& cookies
) {
    std::vector<std::pair<std::string, std::string>> cookieList;

    std::istringstream stream(cookieHeader);
    std::string pair;

    while (std::getline(stream, pair, ';')) {
        size_t equalPos = pair.find('=');
        if (equalPos == std::string::npos) continue;

        std::string key = trim(pair.substr(0, equalPos));
        std::string value = trim(pair.substr(equalPos + 1));

        cookies[key] = value;
        cookieList.emplace_back(key, value);
    }

    return cookieList;
}

std::string CookieParserImpl::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}
