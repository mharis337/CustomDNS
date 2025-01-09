#include "dns_server.h"
#include <iostream>
#include <iomanip>
#include <cstring>

DNSServer::DNSServer(boost::asio::io_context& io_context, uint16_t port)
    : io_context_(io_context),
      socket_(io_context, udp::endpoint(udp::v4(), port)),
      recv_buffer_(512) {
    
    dns_records_["example.com"] = "93.184.216.34";
    dns_records_["localhost"] = "127.0.0.1";
    
    start_receive();
}

void DNSServer::start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_received) {
            if (!ec) {
                handle_receive(bytes_received);
            }
            start_receive();
        });
}

void DNSServer::handle_receive(std::size_t bytes_received) {
    if (bytes_received < sizeof(DNSHeader)) {
        std::cerr << "Received invalid DNS packet (too small)" << std::endl;
        return;
    }

    std::cout << "Received DNS query of " << bytes_received << " bytes" << std::endl;
    
    const DNSHeader* query_header = reinterpret_cast<const DNSHeader*>(recv_buffer_.data());
    
    // Parse query
    std::string domain = parse_query_domain(recv_buffer_.data() + sizeof(DNSHeader));
    std::cout << "Received query for domain: " << domain << " (Query ID: " << ntohs(query_header->id) << ")" << std::endl;
    
    // Prepare response
    std::vector<char> response = create_response(query_header, domain);
    
    // Send response
    socket_.async_send_to(
        boost::asio::buffer(response),
        remote_endpoint_,
        [](boost::system::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Error sending response: " << ec.message() << std::endl;
            }
        });
}

std::string DNSServer::parse_query_domain(const char* query_start) {
    std::string domain;
    const char* ptr = query_start;
    
    while (*ptr != 0) {
        int length = static_cast<unsigned char>(*ptr++);
        domain.append(ptr, length);
        ptr += length;
        if (*ptr != 0) {
            domain += '.';
        }
    }
    
    return domain;
}

std::vector<char> DNSServer::create_response(const DNSHeader* query_header, const std::string& domain) {
    // Calculate the exact size needed for the response
    const size_t header_size = sizeof(DNSHeader);
    
    // Calculate query section length
    size_t query_section_length = 0;
    const char* query_ptr = recv_buffer_.data() + header_size;
    
    // Count the domain name part
    const char* domain_ptr = query_ptr;
    while (*domain_ptr != 0) {
        query_section_length++;  
        const int label_length = static_cast<unsigned char>(*domain_ptr);
        query_section_length += label_length;
        domain_ptr += label_length + 1;
    }
    query_section_length++;  
    query_section_length += sizeof(uint16_t) * 2; 

    // Answer section size: compressed name pointer(2) + type(2) + class(2) + ttl(4) + length(2) + ip(4)
    const size_t answer_section_size = 16;

    // Create response buffer
    const size_t total_size = header_size + query_section_length + answer_section_size;
    std::vector<char> response(total_size, 0);  // Initialize with zeros

    std::cout << "Response size breakdown:" << std::endl
              << "- Header: " << header_size << " bytes" << std::endl
              << "- Query section: " << query_section_length << " bytes" << std::endl
              << "- Answer section: " << answer_section_size << " bytes" << std::endl
              << "Total: " << total_size << " bytes" << std::endl;
    
    // Set up response header
    DNSHeader* response_header = reinterpret_cast<DNSHeader*>(response.data());
    response_header->id = query_header->id;
    response_header->flags = htons(0x8180);  
    response_header->qdcount = htons(1);
    response_header->ancount = htons(1);
    response_header->nscount = htons(0);
    response_header->arcount = htons(0);
    
    std::cout << "Sending response with ID: " << ntohs(query_header->id) 
              << " (Response size: " << total_size << " bytes)" << std::endl;

    char* answer_ptr = response.data() + header_size;
    
    std::memcpy(answer_ptr, recv_buffer_.data() + header_size, query_section_length);
    answer_ptr += query_section_length;

    char* rdata_ptr = answer_ptr;
    
    // Write answer section in network byte order
    uint16_t* answer = reinterpret_cast<uint16_t*>(rdata_ptr);
    *answer++ = htons(0xC00C);  
    *answer++ = htons(1);       
    *answer++ = htons(1);       
    
    // TTL (4 bytes)
    uint32_t* ttl = reinterpret_cast<uint32_t*>(answer);
    *ttl = htonl(300);        
    answer = reinterpret_cast<uint16_t*>(ttl + 1);
    
    *answer++ = htons(4);
    
    // Convert IP address to bytes and copy
    auto ip_str = dns_records_[domain];
    if (ip_str.empty()) {
        ip_str = "0.0.0.0";
    }
    
    std::vector<uint8_t> ip_bytes = ip_to_bytes(ip_str);
    std::memcpy(answer, ip_bytes.data(), 4);
    
    std::cout << "Response hex dump:" << std::endl;
    
    // Print header
    std::cout << "Header (" << header_size << " bytes):" << std::endl;
    for(size_t i = 0; i < header_size; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(static_cast<unsigned char>(response[i])) << " ";
    }
    std::cout << std::dec << std::endl;

    // Print query section
    std::cout << "Query section (" << query_section_length << " bytes):" << std::endl;
    for(size_t i = header_size; i < header_size + query_section_length; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(static_cast<unsigned char>(response[i])) << " ";
    }
    std::cout << std::dec << std::endl;

    // Print answer section
    std::cout << "Answer section (" << answer_section_size << " bytes):" << std::endl;
    for(size_t i = header_size + query_section_length; i < total_size; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(static_cast<unsigned char>(response[i])) << " ";
    }
    std::cout << std::dec << std::endl << std::flush;
    
    return response;
}

std::vector<uint8_t> DNSServer::ip_to_bytes(const std::string& ip) {
    std::vector<uint8_t> bytes(4);
    std::sscanf(ip.c_str(), "%hhu.%hhu.%hhu.%hhu", 
                &bytes[0], &bytes[1], &bytes[2], &bytes[3]);
    return bytes;
}