#ifndef QUERYPARAMETERPARSER_HPP
#define QUERYPARAMETERPARSER_HPP

#include <string>
#include <unordered_map>

class QueryParameterParser {
public:
    virtual ~QueryParameterParser() = default;

    virtual void parse(
        const std::string& query,
        std::unordered_map<std::string, std::string>& queryParams
    ) = 0;
};

#endif // QUERYPARAMETERPARSER_HPP
