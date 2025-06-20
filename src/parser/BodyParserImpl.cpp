#include "../../include/parser/BodyParserImpl.hpp"
#include <sstream>
#include <string>

void BodyParserImpl::parseUrlEncodedBody(const std::string& body, std::unordered_map<std::string, std::string>& output) {
    std::stringstream ss(body);
    std::string pair;
    while (std::getline(ss, pair, '&')) {
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string key = decodeURIComponent(pair.substr(0, eq));
            std::string value = decodeURIComponent(pair.substr(eq + 1));
            output[key] = value;
        }
    }
}

void BodyParserImpl::parseJsonBody(const std::string& body, std::string& output) {
    output = body;
}

void BodyParserImpl::parseMultipartBody(const std::string& body, std::string& output) {
    output = body;
}

void BodyParserImpl::parsePlainText(const std::string& body, std::string& output) {
    output = body;
}

std::string BodyParserImpl::decodeURIComponent(const std::string& str) {
    std::string result;
    char ch;
    int ii;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            result += ch;
            i += 2;
        }
        else if (str[i] == '+') {
            result += ' ';
        }
        else {
            result += str[i];
        }
    }
    return result;
}
