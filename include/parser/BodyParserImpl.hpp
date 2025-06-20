#ifndef BODYPARSERIMPL_HPP
#define BODYPARSERIMPL_HPP

#include "BodyParser.hpp"
#include <string>
#include <unordered_map>

class BodyParserImpl : public BodyParser {
public:
    void parseUrlEncodedBody(const std::string& body, std::unordered_map<std::string, std::string>& output) override;
    void parseJsonBody(const std::string& body, std::string& output) override;
    void parseMultipartBody(const std::string& body, std::string& output) override;
    void parsePlainText(const std::string& body, std::string& output) override;

private:
    std::string decodeURIComponent(const std::string& str);
};

#endif // BODYPARSERIMPL_HPP
