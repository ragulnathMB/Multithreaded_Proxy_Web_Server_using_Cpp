#include "../../include/parser/QueryParameterParserImpl.hpp"

QueryParameterParserImpl::QueryParameterParserImpl() = default;
QueryParameterParserImpl::~QueryParameterParserImpl() = default;

void QueryParameterParserImpl::parse(const std::string& query, std::unordered_map<std::string, std::string>& queryParams) {
    size_t start = 0;
    while (start < query.length()) {
        size_t end = query.find('&', start);
        if (end == std::string::npos) {
            end = query.length();
        }

        std::string param = query.substr(start, end - start);
        size_t eqPos = param.find('=');

        if (eqPos != std::string::npos) {
            std::string key = param.substr(0, eqPos);
            std::string value = param.substr(eqPos + 1);
            queryParams[key] = value;
        }

        start = end + 1;
    }
}
