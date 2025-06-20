#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include <string>
#include <unordered_map>

class BodyParser {
public:
    virtual ~BodyParser() = default;

    virtual void parseUrlEncodedBody(const std::string& body, std::unordered_map<std::string, std::string>& output) = 0;
    virtual void parseJsonBody(const std::string& body, std::string& output) = 0;
    virtual void parseMultipartBody(const std::string& body, std::string& output) = 0;
    virtual void parsePlainText(const std::string& body, std::string& output) = 0;
};

#endif // BODYPARSER_HPP
