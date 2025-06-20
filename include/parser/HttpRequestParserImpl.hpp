#ifndef HTTPREQUESTPARSERIMPL_HPP
#define HTTPREQUESTPARSERIMPL_HPP

#include "HttpRequestParser.hpp"
#include "RequestLineParser.hpp"
#include "HeaderParser.hpp"
#include "BodyParser.hpp"
#include "CookieParser.hpp"
#include "QueryParameterParser.hpp"

#include <memory>

class HttpRequestParserImpl : public HttpRequestParser {
public:
    HttpRequestParserImpl();
    void parseRequest(const std::string& request) override;

    std::string getMethod() override;
    std::string getPath() override;
    std::string getProtocol() override;
    std::string getVersion() override;

    std::unordered_map<std::string, std::string> getHeaders() const override;
    std::string getHeader(const std::string& key) const override;

    std::unordered_map<std::string, std::string> getQueryParams() const override;
    std::string getQueryParam(const std::string& key) const override;

    std::unordered_map<std::string, std::string> getCookies() const override;
    std::string getCookie(const std::string& key) const override;
    std::vector<std::pair<std::string, std::string>> getCookiesList() const override;

    std::unordered_map<std::string, std::string> getBodyParams() const override;
    std::string getBodyParam(const std::string& key) const override;

    std::string getJsonBody() const override;
    std::string getMultipartBodyParts() const override;
    std::string getPlainTextBody() const override;
    std::string getBinaryBody() const override;

private:
    std::unique_ptr<RequestLineParser> requestLineParser;
    std::unique_ptr<HeaderParser> headerParser;
    std::unique_ptr<BodyParser> bodyParser;
    std::unique_ptr<CookieParser> cookieParser;
    std::unique_ptr<QueryParameterParser> queryParameterParser;

    std::string method, path, protocol, version;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> queryParams;
    std::unordered_map<std::string, std::string> cookies;
    std::vector<std::pair<std::string, std::string>> cookiesList;
    std::unordered_map<std::string, std::string> bodyParams;

    std::string jsonBody;
    std::string multipartBodyParts;
    std::string plainTextBody;
    std::string binaryBody;
};

#endif // HTTPREQUESTPARSERIMPL_HPP
