#ifndef HEADERPARSERIMPL_HPP
#define HEADERPARSERIMPL_HPP

#include "HeaderParser.hpp"
#include <string>
#include <unordered_map>

class HeaderParserImpl : public HeaderParser {
public:
    HeaderParserImpl();
    ~HeaderParserImpl() override;

    void parse(const std::string& headerSection, std::unordered_map<std::string, std::string>& headers) override;

private:
    std::string trim(const std::string& s);
};

#endif // HEADERPARSERIMPL_HPP
