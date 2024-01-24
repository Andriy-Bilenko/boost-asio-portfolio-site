#include "clearnet/clearnet.hpp"

using namespace boost;

void accept_and_run(asio::ip::tcp::acceptor& acceptor, asio::io_service& io_service);

int main(int argc, const char* argv[]) {
    const uint16_t PORT = 80;
    while (true) {
        try {
            asio::io_service io_service;
            asio::ip::tcp::endpoint endpoint{asio::ip::tcp::v4(), PORT};
            asio::ip::tcp::acceptor acceptor{io_service, endpoint};

            acceptor.listen();
            accept_and_run(acceptor, io_service);

            io_service.run();

        } catch (const boost::system::system_error& e) {
            std::stringstream ss;
            ss << "Boost.Asio Exception: " << e.what() << " at " << get_curr_time();
            std::cerr << ANSI_RED << ss.str() << ANSI_RESET << "\r\n";
            log2file("exceptions.log", ss.str());
        } catch (const std::exception& e) {
            std::stringstream ss;
            ss << "Standard Exception: " << e.what() << " at " << get_curr_time();
            std::cerr << ANSI_RED << ss.str() << ANSI_RESET << "\r\n";
            log2file("exceptions.log", ss.str());
        } catch (...) {
            std::stringstream ss;
            ss << "=== Unknown Exception ==="
               << " at " << get_curr_time();
            std::cerr << ANSI_RED << ss.str() << ANSI_RESET << "\r\n";
            log2file("exceptions.log", ss.str());
        }
    }
    return 0;
}

void accept_and_run(asio::ip::tcp::acceptor& acceptor, asio::io_service& io_service) {
    auto sesh = std::make_shared<Session>(io_service);

    acceptor.async_accept(sesh->socket, [sesh, &acceptor, &io_service](const error_code& accept_error) {
        accept_and_run(acceptor, io_service);
        if (!accept_error) {
            std::clog << "Connection established!" << std::endl;
            // Continue accepting new connections
            accept_and_run(acceptor, io_service);
            // interact after connecting
            Session::interact(sesh, [&]() {  // passing pre_interact function
                // logging successful session:
                std::stringstream ss;
                auto ip_addr = sesh->socket.remote_endpoint().address();
                ss << "successful session on " << ip_addr << " at " << get_curr_time();
                std::clog << ANSI_GREEN << ss.str() << ANSI_RESET << "\r\n";
                // logging to file:
                log2file("successful_sessions.log", ss.str());
            });
        } else {
            std::cerr << "Accept error: " << accept_error.message() << std::endl;
            // logging unsuccessful session:
            std::stringstream ss;
            auto ip_addr = sesh->socket.lowest_layer().remote_endpoint().address();
            ss << "error session on " << ip_addr << " at " << get_curr_time();
            std::clog << ANSI_YELLOW << ss.str() << ANSI_RESET << "\r\n";
            // logging to file:
            log2file("error_sessions.log", ss.str());
            // try again
            accept_and_run(acceptor, io_service);
        }
    });
}
