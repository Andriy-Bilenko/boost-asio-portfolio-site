#pragma once

#include "clearnet_common.hpp"
#include "clearnet_funcs.hpp"
#include "clearnet_http_headers.hpp"

using namespace boost;
using namespace std;

// forward declarations:
int log2file(const std::string& file_name, const std::string& log_str);

class Session {
    // never called because we serve only GET requests and don't care about the body
    static void readBody(std::shared_ptr<Session> pThis) {
        const int BUFF_SIZE = 1000;
        std::shared_ptr<std::vector<char>> bufptr = std::make_shared<std::vector<char>>(BUFF_SIZE);
        asio::async_read(pThis->socket, boost::asio::buffer(*bufptr, BUFF_SIZE),
                         [pThis](const error_code& e, std::size_t s) {});
    }

    static void readNextLine(std::shared_ptr<Session> pThis) {
        asio::async_read_until(pThis->socket, pThis->m_buff, '\r', [pThis](const error_code& e, std::size_t s) {
            std::string line, ignore;
            std::istream stream{&pThis->m_buff};
            std::getline(stream, line, '\r');
            std::getline(stream, ignore, '\n');
            pThis->m_headers.onReadHeader(line);

            if (line.length() == 0) {
                if (pThis->m_headers.contentLength() == 0) {
                    std::shared_ptr<std::string> str = std::make_shared<std::string>(pThis->m_headers.getResponse());
                    asio::async_write(pThis->socket, boost::asio::buffer(str->c_str(), str->length()),
                                      [pThis, str](const error_code& e, std::size_t s) {
                                          if (!e) {
                                              std::clog << "Wrote to client : OK\r\n";
                                          } else {
                                              std::clog << "Wrote to client : FAIL: " << e.message() << std::endl;
                                          }
                                      });
                } else {
                    pThis->readNextLine(pThis);
                }
            } else {
                pThis->readNextLine(pThis);
            }
        });
    }

    static void readFirstLine(std::shared_ptr<Session> pThis) {
        asio::async_read_until(pThis->socket, pThis->m_buff, '\r', [pThis](const error_code& e, std::size_t s) {
            std::string line, ignore;
            std::istream stream{&pThis->m_buff};
            std::getline(stream, line, '\r');
            std::getline(stream, ignore, '\n');
            pThis->m_headers.onReadRequestLine(line);
            pThis->readNextLine(pThis);
        });
    }

   public:
    asio::ssl::stream<asio::ip::tcp::socket> socket;  // Using ssl::stream instead of tcp::socket

    Session(asio::io_service& io_service, asio::ssl::context& context) : socket(io_service, context) {}

    // static in order to outlive the async function
    static void interact(std::shared_ptr<Session> pThis, std::function<void()> pre_interact) {
        pre_interact();
        readFirstLine(pThis);
    }

   private:
    asio::streambuf m_buff;
    HttpHeaders m_headers;
};

