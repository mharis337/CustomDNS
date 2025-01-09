#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <unordered_map>

using boost::asio::ip::udp;

// DNS Header structure
struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

class DNSServer {
public:
    explicit DNSServer(boost::asio::io_context& io_context, uint16_t port = 5353);

private:
    void start_receive();
    void handle_receive(std::size_t bytes_received);
    std::string parse_query_domain(const char* query_start);
    std::vector<char> create_response(const DNSHeader* query_header, const std::string& domain);
    std::vector<uint8_t> ip_to_bytes(const std::string& ip);

    boost::asio::io_context& io_context_;
    udp::socket socket_;
    std::unordered_map<std::string, std::string> dns_records_;
    std::vector<char> recv_buffer_;
    udp::endpoint remote_endpoint_;
};