#include "../../include/parser/HttpRequestParserImpl.hpp"
#include "../../include/parser/RequestLineParserImpl.hpp"
#include "../../include/parser/HeaderParserImpl.hpp"
#include "../../include/parser/BodyParserImpl.hpp"
#include "../../include/parser/CookieParserImpl.hpp"
#include "../../include/parser/QueryParameterParserImpl.hpp"

#include <iostream>

/*
Sample HTTP Request:
---------------------------------------------------------
POST /search?q=something&lang=en HTTP/1.1\r\n
Host: www.example.com\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n
Accept: text/html,application/xhtml+xml\r\n
Accept-Language: en-US,en;q=0.9\r\n
Accept-Encoding: gzip, deflate, br\r\n
Connection: keep-alive\r\n
Cookie: sessionId=abc123; theme=dark; lang=en\r\n
Content-Type: application/x-www-form-urlencoded\r\n
Content-Length: 27\r\n
\r\n
username=john&password=1234
---------------------------------------------------------
*/

HttpRequestParserImpl::HttpRequestParserImpl() {
    requestLineParser = std::make_unique<RequestLineParserImpl>();
    headerParser = std::make_unique<HeaderParserImpl>();
    bodyParser = std::make_unique<BodyParserImpl>();
    cookieParser = std::make_unique<CookieParserImpl>();
    queryParameterParser = std::make_unique<QueryParameterParserImpl>();
}

void HttpRequestParserImpl::parseRequest(const std::string& request) {
    std::string requestLine = request.substr(0, request.find("\r\n"));
    requestLineParser->parse(requestLine, method, path, protocol, version);

    std::string headersSection = request.substr(
        request.find("\r\n") + 2,
        request.find("\r\n\r\n") - request.find("\r\n") - 2
    );
    headerParser->parse(headersSection, headers);

    if (headers.find("Cookie") != headers.end()) {
        cookiesList = cookieParser->parse(headers["Cookie"], cookies);
    }

    size_t qMark = path.find('?');
    if (qMark != std::string::npos) {
        std::string queryString = path.substr(qMark + 1);
        path = path.substr(0, qMark);
        queryParameterParser->parse(queryString, queryParams);
    }

    std::string bodySection = request.substr(request.find("\r\n\r\n") + 4);
    if (headers.find("Content-Type") != headers.end()) {
        std::string contentType = headers["Content-Type"];
        if (contentType == "application/x-www-form-urlencoded") {
            bodyParser->parseUrlEncodedBody(bodySection, bodyParams);
        } else if (contentType == "application/json") {
            bodyParser->parseJsonBody(bodySection, jsonBody);
        } else if (contentType.find("multipart/form-data") != std::string::npos) {
            bodyParser->parseMultipartBody(bodySection, multipartBodyParts);
        } else if (contentType == "text/plain") {
            plainTextBody = bodySection;
        } else if (contentType == "application/octet-stream") {
            binaryBody = bodySection;
        }
    }
}

// Getters
std::string HttpRequestParserImpl::getMethod() { return method; }
std::string HttpRequestParserImpl::getPath() { return path; }
std::string HttpRequestParserImpl::getProtocol() { return protocol; }
std::string HttpRequestParserImpl::getVersion() { return version; }

std::unordered_map<std::string, std::string> HttpRequestParserImpl::getHeaders() const { return headers; }
std::string HttpRequestParserImpl::getHeader(const std::string& key) const {
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}

std::unordered_map<std::string, std::string> HttpRequestParserImpl::getQueryParams() const { return queryParams; }
std::string HttpRequestParserImpl::getQueryParam(const std::string& key) const {
    auto it = queryParams.find(key);
    return (it != queryParams.end()) ? it->second : "";
}

std::unordered_map<std::string, std::string> HttpRequestParserImpl::getCookies() const { return cookies; }
std::string HttpRequestParserImpl::getCookie(const std::string& key) const {
    auto it = cookies.find(key);
    return (it != cookies.end()) ? it->second : "";
}

std::vector<std::pair<std::string, std::string>> HttpRequestParserImpl::getCookiesList() const {
    return cookiesList;
}

std::unordered_map<std::string, std::string> HttpRequestParserImpl::getBodyParams() const { return bodyParams; }
std::string HttpRequestParserImpl::getBodyParam(const std::string& key) const {
    auto it = bodyParams.find(key);
    return (it != bodyParams.end()) ? it->second : "";
}

std::string HttpRequestParserImpl::getJsonBody() const { return jsonBody; }
std::string HttpRequestParserImpl::getMultipartBodyParts() const { return multipartBodyParts; }
std::string HttpRequestParserImpl::getPlainTextBody() const { return plainTextBody; }
std::string HttpRequestParserImpl::getBinaryBody() const { return binaryBody; }
