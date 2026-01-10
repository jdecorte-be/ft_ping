<h1 align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="assets/banner.png" alt="ft_ping" ></a>
  <br>
  ft_ping
</h1>

<p align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="https://shields.io/badge/Platform-Linux-blue?logo=linux" alt="Platform"></a>
  <a href="https://github.com/jdecorte-be/ft_ping/stargazers"><img src="https://shields.io/github/stars/jdecorte-be/ft_ping" alt="Stars"></a>
  <a href="https://github.com/jdecorte-be/ft_ping/issues"><img src="https://shields.io/github/issues/jdecorte-be/ft_ping" alt="Issues"></a>
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="https://shields.io/github/license/jdecorte-be/ft_ping" alt="License"></a>
</p>

<p align="center">
  <a href="#key-features">Key Features</a> • <a href="#installation">Installation</a> • <a href="#usage">Usage</a>
</p>


## Overview
`ft_ping` is a custom implementation of the classic network diagnostic tool `ping`. This project focuses on low-level network programming, utilizing raw sockets to send and receive ICMP ECHO_REQUEST and ECHO_REPLY packets. It provides real-time feedback on host reachability and calculates precise round-trip time (RTT) statistics.

## Key Features
- **ICMP Protocol**: Direct interaction with the Internet Control Message Protocol using raw sockets.
- **DNS Resolution**: Resolves hostnames to IPv4 addresses using modern `getaddrinfo` interfaces.
- **RTT Analysis**: Calculates minimum, average, maximum, and mean deviation (stddev) of round-trip times.
- **Signal Handling**: Graceful termination and statistics reporting upon receiving `SIGINT` (Ctrl+C).
- **Verbosity Support**: Detailed output mode for packet inspection and troubleshooting.
- **Math Precision**: Utilizes `math.h` and `sys/time.h` for high-precision latency calculations.

## Tech Stack
- **Language**: C (C99 standard)
- **Libraries**: `argp` (parsing), `math` (statistics), `libc` (sockets)
- **Networking**: Raw Sockets (`SOCK_RAW`), IPv4, ICMP
- **Containerization**: Docker & Docker Compose for isolated network testing
- **Build System**: GNU Make

## Installation

### Prerequisites
- A Linux environment (Raw sockets require specific capabilities or root access).
- GCC and Make.

### Building the project
```bash
make
```

### Using Docker
To run the project in a controlled environment with all dependencies:
```bash
docker-compose up --build
```

## Usage
Note: Because this tool uses raw sockets, it must be run with `sudo` or the `CAP_NET_RAW` capability.

```bash
sudo ./ft_ping <hostname_or_ip> [-v]
```

### Options
- `HOST`: The target IPv4 address or hostname to ping.
- `-v, --verbose`: Enable verbose output (displays ICMP headers and extra details).
- `-?, --help`: Show the help menu.