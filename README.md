<h1 align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="assets/banner.png" alt="ft_ping" ></a>
  <br>
  ft_ping
</h1>

<p align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="https://shields.io/badge/Platform-Linux-blue?logo=linux" alt="Platform"></a>
  <a href="https://github.com/jdecorte-be/ft_ping/stargazers"><img src="https://shields.io/github/stars/jdecorte-be/ft_ping" alt="Stars"></a>
  <a href="https://github.com/jdecorte-be/ft_ping/issues"><img src="https://shields.io/github/issues/jdecorte-be/ft_ping" alt="Issues"></a>
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="https://shields.io/github/repo-size/jdecorte-be/ft_ping" alt="Size"></a>
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="https://shields.io/github/languages/top/jdecorte-be/ft_ping" alt="Language"></a>
</p>

<p align="center">
  <a href="#features">Features</a> • <a href="#installation">Installation</a> • <a href="#usage">Usage</a>
</p>


## Overview

**ft_ping** is a custom implementation of the standard `ping` system utility, developed in C. The project focuses on network programming using raw sockets to send ICMP (Internet Control Message Protocol) Echo Request packets and handle Echo Reply responses. 

This implementation replicates the core functionality of the original utility, including domain name resolution, packet sequencing, and detailed statistical reporting upon termination.

## Features

- **ICMP Protocol:** Manual construction and parsing of ICMP headers and data.
- **DNS Resolution:** Support for both IP addresses and hostnames.
- **Real-time Monitoring:** Displays sequence numbers, TTL (Time to Live), and round-trip time (RTT).
- **Statistics Summary:** Provides a detailed report including packets transmitted/received, packet loss percentage, and RTT metrics (min/avg/max/mdev).
- **Signal Handling:** Graceful termination via `SIGINT` (Ctrl+C) to display final statistics.
- **Docker Integration:** Pre-configured environment for testing and deployment to ensure consistency across different network configurations.

## Installation

### Prerequisites

- A C compiler (e.g., `gcc` or `clang`).
- `make` build utility.
- Root or sudo privileges (required for opening raw sockets).

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ft_ping.git
   cd ft_ping
   ```

2. Compile the source code:
   ```bash
   make
   ```

### Using Docker

Alternatively, you can build and run the project using the provided Docker configuration:

```bash
docker-compose up --build
```

## Usage

The executable must be run with root privileges to interact with raw sockets.

```bash
sudo ./ft_ping <destination> [options]
```

### Arguments
- `<destination>`: The hostname (e.g., `google.com`) or IP address (e.g., `8.8.8.8`) to ping.

### Example
```bash
sudo ./ft_ping google.com
```

### Options
- `-v`: Verbose mode (displays more detailed information about received packets and errors).
- `-h`: Display help/usage information.

## Project Structure

The project follows a modular structure for clarity and maintainability:

```text
.
├── includes/
│   └── ping.h          # Header file containing macros, structures, and prototypes
├── srcs/
│   ├── main.c          # Entry point and argument parsing
│   └── ping.c          # Core logic (socket management, ICMP handling, statistics)
├── assets/
│   └── banner.png      # Project documentation assets
├── Dockerfile          # Container configuration
├── docker-compose.yml  # Multi-container orchestration for testing
├── Makefile            # Build instructions
├── tester.sh           # Automated testing script
├── LICENSE             # Project license
└── README.md           # Project documentation
```

- **srcs/main.c**: Handles the initialization of the program, command-line argument validation, and DNS resolution.
- **srcs/ping.c**: Contains the main execution loop, checksum calculations, and the logic for sending/receiving packets via raw sockets.
- **tester.sh**: A script designed to validate the output and behavior of `ft_ping` against the system's native utility.