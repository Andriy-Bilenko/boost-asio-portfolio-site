#pragma once

#include "clearnet_common.hpp"
#include "clearnet_session.hpp"

using namespace boost;
using namespace std;

// declarations:
std::string get_last_line(const std::string& file_name);
int log2file(const std::string& file_name, const std::string& log_str);
std::string get_curr_time();
std::string read_file(const std::string& filename);
std::string generate_http_response(const std::string& content, const std::string& contentType);

std::string get_last_line(const std::string& file_name) {
    std::string ret{""};
    std::ifstream ifs(file_name);
    if (!ifs.is_open()) {
        std::cerr << "error openning file for reading: " << file_name << "\r\n";
        return ret;
    }
    ifs.seekg(0, std::ios::end);
    int fileSize = ifs.tellg();
    if (fileSize <= 0) {
        // file empty
        return ret;
    }
    // Read characters in reverse order
    bool first_new_line_passed = false;
    for (int i = fileSize - 1;; --i) {
        if (i == -1) {
            std::getline(ifs, ret);
            // Discard the last character \r
            if (ret.size() > 0) {
                ret.pop_back();
            }
            return ret;
        }
        // Move the cursor to the desired position
        ifs.seekg(i, std::ios::beg);
        char ch;
        ifs.get(ch);
        if (ch == '\n' && !first_new_line_passed) {
            first_new_line_passed = true;
            continue;
        }
        if (ch == '\n' && first_new_line_passed) {
            ifs.seekg(i + 1, std::ios::beg);
            std::getline(ifs, ret);
            // Discard the last character \r
            if (ret.size() > 0) {
                ret.pop_back();
            }
            return ret;
        }
    }
    return ret;
}

int log2file(const std::string& file_name, const std::string& log_str) {
    std::string last_line = get_last_line(file_name);
    if (last_line == log_str) {
        // duplicate line
    } else {
        // writing to the end of file
        std::ofstream ofs(file_name, std::ios::app);
        if (!ofs.is_open()) {
            std::cerr << "error openning file for writing: " << file_name << "\r\n";
            return -1;
        }
        ofs << log_str << "\r\n";
        return 0;
    }
    return 0;
}

std::string get_curr_time() {
    time_t now = time(0);
    char* date_time = ctime(&now);
    // Remove the newline character
    date_time[strcspn(date_time, "\n")] = '\0';
    std::string date_time_string(date_time);
    return date_time_string;
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string generate_http_response(const std::string& content, const std::string& contentType) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << content;
    return response.str();
}

