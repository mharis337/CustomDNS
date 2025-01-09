cmake_minimum_required(VERSION 3.10)
project(dns_server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add executable
add_executable(dns_server 
    src/main.cpp
    src/dns_server.cpp
)

# Link Boost libraries
target_link_libraries(dns_server PRIVATE
    Boost::system
    pthread
)