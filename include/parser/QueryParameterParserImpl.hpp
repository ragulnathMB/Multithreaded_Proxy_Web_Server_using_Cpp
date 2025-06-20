#ifndef QUERYPARAMETERPARSERIMPL_HPP
#define QUERYPARAMETERPARSERIMPL_HPP

#include "QueryParameterParser.hpp"

class QueryParameterParserImpl : public QueryParameterParser {
public:
    QueryParameterParserImpl();
    ~QueryParameterParserImpl() override;

    void parse(const std::string& query, std::unordered_map<std::string, std::string>& queryParams) override;
};

#endif // QUERYPARAMETERPARSERIMPL_HPP
