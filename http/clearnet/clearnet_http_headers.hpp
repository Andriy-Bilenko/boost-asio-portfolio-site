#pragma once

#include "clearnet_common.hpp"
#include "clearnet_funcs.hpp"

using namespace boost;

// forward declarations:
std::string get_curr_time();
std::string read_file(const std::string& filename);
std::string generate_http_response(const std::string& content, const std::string& content_type);

class HttpHeaders {
   public:
    std::string getResponse() {
        std::string ret{""};
        std::string file{""};
        std::string file_prefix = "res/";
        if (m_url == "/") {
            file = read_file(file_prefix + "index.html");
            if (!file.empty()) {
                ret = generate_http_response(file, "text/html");
            }
        }
        if (m_url.find(".css") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "text/css");
            }
        }
        if (m_url.find(".js") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "application/javascript");
            }
        }
        if (m_url.find(".png") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "image/png");
            }
        }
        if (m_url.find(".svg") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "image/svg+xml");
            }
        }
        if (m_url.find(".jpg") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "image/jpg");
            }
        }
        if (m_url.find(".gif") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "image/gif");
            }
        }
        if (m_url.find(".ico") != std::string::npos) {
            file = read_file(file_prefix + m_url);
            if (!file.empty()) {
                ret = generate_http_response(file, "image/vnd.microsoft.icon");
            }
        }
        // return error response if we are here:
        if (ret.empty()) {
            file = read_file(file_prefix + "404.html");
            ret = generate_http_response(file, "text/html");
        }
        return ret;
    }

    int contentLength() {
        auto request = m_headers.find("content-length");
        if (request != m_headers.end()) {
            std::stringstream ss_length(request->second);
            int content_length;
            ss_length >> content_length;
            return content_length;
        }
        return 0;
    }

    void onReadHeader(std::string line) {
        std::stringstream ss_header(line);
        std::string header_name;
        std::getline(ss_header, header_name, ':');

        std::string value;
        std::getline(ss_header, value);
        m_headers[header_name] = value;
    }

    void onReadRequestLine(std::string line) {
        std::stringstream ss_request_line(line);
        ss_request_line >> m_method;
        ss_request_line >> m_url;
        ss_request_line >> m_version;

        std::clog << "request for resource: " << m_url << std::endl;
    }

   private:
    std::string m_method;
    std::string m_url;
    std::string m_version;
    std::map<std::string, std::string> m_headers;
};
