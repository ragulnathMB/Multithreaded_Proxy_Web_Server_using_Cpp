#ifndef HEADERPARSER_HPP
#define HEADERPARSER_HPP

#include <string>
#include <unordered_map>

class HeaderParser {
public:
    virtual ~HeaderParser() = default;

    virtual void parse(const std::string& headerSection, std::unordered_map<std::string, std::string>& headers) = 0;
};

#endif // HEADERPARSER_HPP
