# Custom DNS Server in C++

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/language-C++-blue.svg)
![Docker](https://img.shields.io/badge/docker-supported-blue.svg)

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Purpose](#purpose)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Experimentation Highlights](#experimentation-highlights)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

This project is a **Custom DNS Server in C++**, designed to provide a hands-on way to understand how DNS works. Built with Boost.Asio for asynchronous networking and containerized with Docker for ease of deployment, it serves as a foundation for learning, experimenting, and exploring DNS functionalities.

---

## Features

- **Asynchronous Networking**: Uses Boost.Asio for handling multiple DNS queries efficiently.
- **Basic DNS Functionality**: Supports A-record queries and includes basic query parsing and response.
- **Extensible Design**: Provides a structure for adding features like caching and forwarding.
- **Dockerized**: Easily run in isolated environments for experimentation or lightweight production use.
- **Logging**: Includes basic logging for observing query processing and responses.

---

## Purpose

This project was created as an experiment to learn the fundamentals of DNS and explore network programming in C++. It is not a production-grade DNS server but a sandbox for:

- Understanding DNS query and response structures.
- Learning asynchronous programming using Boost.Asio.
- Experimenting with extending DNS functionalities, such as handling additional record types.
- Exploring containerization with Docker.

---

## Project Structure

```
custom-dns-server/
├── build/           # Build directory
├── include/         # Header files
├── src/             # Source code
│   ├── main.cpp     # Entry point for the DNS server
│   ├── dns_server.cpp # Core DNS server implementation
├── Dockerfile       # Docker instructions
├── CMakeLists.txt   # CMake build configuration
└── README.md        # Project documentation
```

---

## Installation

### Prerequisites

- **C++ Compiler**: Supporting C++17 (e.g., g++ or clang++)
- **CMake**: Version 3.10 or higher
- **Boost Libraries**: Installed with Boost.Asio
- **Docker**: Optional, for containerized deployment
- **Git**: For cloning the repository

### Steps

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/custom-dns-server.git
    cd custom-dns-server
    ```

2. Install dependencies (example for Ubuntu/Debian):

    ```bash
    sudo apt update
    sudo apt install -y build-essential cmake libboost-all-dev
    ```

3. Build the project:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

---

## Usage

### Running the DNS Server

1. Start the server:

    ```bash
    ./build/dns_server
    ```

2. Test with `dig`:

    ```bash
    dig @127.0.0.1 example.com A
    ```

### Dockerized Deployment

1. Build the Docker image:

    ```bash
    docker build -t custom-dns-server .
    ```

2. Run the container:

    ```bash
    docker run -d -p 5353:5353/udp custom-dns-server
    ```

---

## Experimentation Highlights

This project showcases the following learning experiences:

1. **DNS Internals**:
   - Parsed DNS queries and crafted responses.
   - Experimented with custom DNS records and basic error handling.

2. **Asynchronous Networking**:
   - Leveraged Boost.Asio for concurrent query handling.
   - Explored techniques to improve performance and reduce blocking.

3. **Containerization**:
   - Used Docker to isolate and test the server.
   - Practiced deploying lightweight services.

4. **Debugging and Logging**:
   - Integrated logging to observe query processing.
   - Enhanced debugging skills through structured error handling.

---

## Contributing

Contributions are welcome to enhance features, optimize the code, or improve the documentation. Submit a pull request or open an issue to discuss changes.

---

## License

This project is licensed under the [MIT License](LICENSE).

