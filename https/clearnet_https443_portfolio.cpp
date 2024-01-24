#include "clearnet/clearnet.hpp"

using namespace boost;

void accept_and_run(asio::ip::tcp::acceptor& acceptor, asio::io_service& io_service, asio::ssl::context& context);

int main(int argc, const char* argv[]) {
    const uint16_t PORT = 443;
    while (true) {
        try {
            asio::io_service io_service;
            asio::ssl::context context(asio::ssl::context::sslv23);
            context.set_options(asio::ssl::context::default_workarounds | asio::ssl::context::no_sslv2 |
                                asio::ssl::context::single_dh_use);
            // Load the server certificate and private key
            context.use_certificate_chain_file("fullchain1.pem");
            context.use_private_key_file("privkey1.pem", asio::ssl::context::pem);

            asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT));
            // accept sessions and run interractions:
            accept_and_run(acceptor, io_service, context);
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

void accept_and_run(asio::ip::tcp::acceptor& acceptor, asio::io_service& io_service, asio::ssl::context& context) {
    auto sesh = std::make_shared<Session>(io_service, context);

    acceptor.async_accept(
        sesh->socket.lowest_layer(), [sesh, &acceptor, &io_service, &context](const std::error_code& error) {
            if (!error) {
                sesh->socket.async_handshake(
                    asio::ssl::stream_base::server,
                    [sesh, &acceptor, &io_service, &context](const std::error_code& handshake_error) {
                        if (!handshake_error) {
                            // Your session logic here
                            std::clog << "Connection established!" << std::endl;
                            // Continue accepting new connections
                            accept_and_run(acceptor, io_service, context);
                            // interact after connecting
                            Session::interact(sesh, [&]() {  // passing pre_interact function
                                // logging successful session:
                                std::stringstream ss;
                                auto ip_addr = sesh->socket.lowest_layer().remote_endpoint().address();
                                ss << "successful session on " << ip_addr << " at " << get_curr_time();
                                std::clog << ANSI_GREEN << ss.str() << ANSI_RESET << "\r\n";
                                // logging to file:
                                log2file("successful_sessions.log", ss.str());
                            });
                        } else {
                            std::cerr << "Handshake error: " << handshake_error.message() << std::endl;
                            // logging unsuccessful session:
                            std::stringstream ss;
                            auto ip_addr = sesh->socket.lowest_layer().remote_endpoint().address();
                            ss << "error session on " << ip_addr << " at " << get_curr_time();
                            std::clog << ANSI_YELLOW << ss.str() << ANSI_RESET << "\r\n";
                            // logging to file:
                            log2file("error_sessions.log", ss.str());
                            // try again
                            accept_and_run(acceptor, io_service, context);
                        }
                    });
            } else {
                std::cerr << "Accept error: " << error.message() << std::endl;
            }
        });
}
