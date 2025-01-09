#include "dns_server.h"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        const uint16_t port = 5353;
        DNSServer server(io_context, port);
        
        std::cout << "DNS Server started on port " << port << "..." << std::endl;
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}