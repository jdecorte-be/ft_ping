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


![Banner](assets/banner.png)

## Overview

**ft_ping** is a custom implementation of the standard `ping` network utility, written in C. The project focuses on low-level network programming, specifically the use of raw sockets to send and receive ICMP (Internet Control Message Protocol) packets. It serves as a tool to verify the reachability of a host and measure round-trip time (RTT) between the source and destination.

This implementation handles the construction of IP and ICMP headers, packet serialization, and the precise timing required for network diagnostics.

## Features

- **ICMP Echo Requests:** Sends `ECHO_REQUEST` packets and listens for `ECHO_REPLY` responses.
- **Round-Trip Timing:** High-precision calculation of minimum, maximum, average, and standard deviation (mdev) of RTT.
- **Signal Handling:** Graceful termination via `SIGINT` (Ctrl+C) with a summary of statistics.
- **Packet Customization:** Support for various command-line options such as verbose output and custom counts (depending on implementation specifics).
- **Docker Integration:** A containerized environment for consistent testing across different network configurations.
- **Automated Testing:** Includes a `tester.sh` script to validate functionality against the standard `ping` utility.

## Installation

### Prerequisites

- A C compiler (e.g., `gcc` or `clang`).
- Standard C libraries.
- Linux environment (required for raw socket access).
- **Docker** (optional, for containerized execution).

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/username/ft_ping.git
   cd ft_ping
   ```

2. Compile the source code using the provided `Makefile`:
   ```bash
   make
   ```

## Usage

**Note:** Since the program utilizes raw sockets, it must be executed with root privileges or `CAP_NET_RAW` capabilities.

### Basic Command
```bash
sudo ./ft_ping google.com
```

### Options
The implementation follows the standard `ping` syntax:
- `destination`: The hostname or IP address to ping.
- `-v`: Enable verbose output (detailed ICMP errors).
- `-?`: Display help and usage information.

### Using with Docker
To run the utility inside a controlled Docker environment:
```bash
docker-compose up --build
```

## Project Structure

```text
.
├── assets/              # Visual assets and project banners
├── includes/            # Header files
│   └── ping.h           # Core definitions and structures
├── srcs/                # Source files
│   ├── main.c           # Entry point and argument parsing
│   └── ping.c           # Logic for ICMP communication and timing
├── Dockerfile           # Docker configuration for the environment
├── docker-compose.yml   # Orchestration for containerized testing
├── Makefile             # Build automation script
├── tester.sh            # Automated validation script
└── .gitignore           # Version control ignore rules
```