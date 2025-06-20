#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

class HttpRequestParser {
public:
    virtual ~HttpRequestParser() = default;
    virtual void parseRequest(const std::string& request) = 0;

    virtual std::string getMethod() = 0;
    virtual std::string getPath() = 0;
    virtual std::string getProtocol() = 0;
    virtual std::string getVersion() = 0;

    virtual std::unordered_map<std::string, std::string> getHeaders() const = 0;
    virtual std::string getHeader(const std::string& key) const = 0;

    virtual std::unordered_map<std::string, std::string> getQueryParams() const = 0;
    virtual std::string getQueryParam(const std::string& key) const = 0;

    virtual std::unordered_map<std::string, std::string> getCookies() const = 0;
    virtual std::string getCookie(const std::string& key) const = 0;
    virtual std::vector<std::pair<std::string, std::string>> getCookiesList() const = 0;

    virtual std::unordered_map<std::string, std::string> getBodyParams() const = 0;
    virtual std::string getBodyParam(const std::string& key) const = 0;

    virtual std::string getJsonBody() const = 0;
    virtual std::string getMultipartBodyParts() const = 0;
    virtual std::string getPlainTextBody() const = 0;
    virtual std::string getBinaryBody() const = 0;
};

#endif // HTTPREQUESTPARSER_HPP
