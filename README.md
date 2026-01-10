This is a professional `README.md` template tailored for your **ft_ping** project. Since the project name suggests a re-implementation of the system's `ping` command (likely for the 42 School curriculum), the content focuses on network programming and raw sockets.

---

# ft_ping

A custom implementation of the standard `ping` utility in C, built to explore the inner workings of the ICMP protocol and network socket programming.

## Overview

`ft_ping` is a network diagnostic tool that sends ICMP `ECHO_REQUEST` packets to network hosts and waits for ICMP `ECHO_REPLY` responses. This project demonstrates how to handle raw sockets, construct packet headers manually, and calculate round-trip times (RTT) with high precision.

> **Note:** Because this program uses raw sockets (`SOCK_RAW`), it typically requires root privileges (sudo) to run.

## Features

- **ICMP Protocol Implementation**: Manually constructs ICMP packets and IP headers.
- **Host Resolution**: Supports both IP addresses and hostnames (DNS resolution).
- **Real-time Statistics**: Tracks packets sent/received, packet loss, and response times.
- **RTT Calculations**: Calculates minimum, maximum, average, and standard deviation (mdev) of round-trip times.
- **Interrupt Handling**: Gracefully handles `Ctrl+C` to display summary statistics before exiting.
- **Dockerized Environment**: Includes a Dockerfile and docker-compose for easy testing in isolated network environments.

## Project Structure

```text
.
├── includes/
│   └── ping.h          # Header file containing structures and defines
├── srcs/
│   ├── main.c          # Entry point and argument parsing
│   └── ping.c          # Core ping logic and packet handling
├── Makefile            # Build instructions
├── Dockerfile          # Container definition
├── docker-compose.yml  # Multi-container orchestration (for testing)
├── tester.sh           # Automated testing script
└── README.md           # Documentation
```

## Installation

### Prerequisites
- A C compiler (e.g., `gcc` or `clang`)
- `make`
- Linux environment (recommended for raw socket compatibility)

### Building the project
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/ft_ping.git
   cd ft_ping
   ```
2. Compile the source code:
   ```bash
   make
   ```

## Usage

### Basic Usage
Run the program followed by a destination host. Root privileges are required:
```bash
sudo ./ft_ping google.com
```

### Options
*(If you have implemented standard flags like -v or -c, list them here)*
- `-v`: Verbose output.
- `-h`: Display help/usage information.

### Running with Docker
If you prefer not to install dependencies locally or wish to test in a clean environment:
```bash
docker-compose up --build
```

### Running Tests
The project includes a `tester.sh` script to verify functionality:
```bash
chmod +x tester.sh
./tester.sh
```

## Technical Details

- **Socket Type**: `AF_INET`, `SOCK_RAW`, `IPPROTO_ICMP`.
- **Packet Construction**: Uses `<netinet/ip_icmp.h>` and `<netinet/ip.h>` to map headers.
- **Checksum Calculation**: Implements the Internet Checksum algorithm as defined in RFC 1071.
- **Timing**: Utilizes `gettimeofday` or `clock_gettime` for microsecond-precision RTT measurement.

## License

This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details. (Note: Replace this with your preferred license).

---
*Developed as part of the 42 School curriculum.*