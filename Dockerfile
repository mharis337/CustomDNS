FROM ubuntu:22.04

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Create working directory
WORKDIR /app

# Copy the entire project
COPY . .

# Remove any existing build directory and create a fresh one
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

# Expose DNS port
EXPOSE 5353/udp

# Run the DNS server
CMD ["./build/dns_server"]