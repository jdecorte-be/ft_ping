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


### Overview
`ft_ping` is a high-performance C-based implementation of the classic network utility `ping`. Developed as a systems programming project, it leverages raw sockets to send ICMP `ECHO_REQUEST` packets and process `ECHO_REPLY` responses. The project focuses on low-level network communication, DNS resolution, and precise timing to calculate network latency and reliability metrics.

This implementation mimics the behavior of the standard `iputils` and `inetutils` suites, providing detailed RTT (Round Trip Time) statistics and robust error handling.

### Key Features
*   **ICMP Protocol Implementation:** Manually constructs ICMP headers and handles network-level communication using raw sockets.
*   **Precision Latency Tracking:** Calculates RTT for every packet with microsecond precision, providing statistics for minimum, average, maximum, and standard deviation (`mdev`).
*   **DNS Resolution:** Integrated hostname-to-IP resolution using `getaddrinfo`, supporting both IPv4 addresses and domain names.
*   **Comprehensive Statistics:** Generates a final report upon termination (`SIGINT`), including packet loss percentage and total execution time.
*   **Verbose Mode:** Includes a `-v` (verbose) flag for detailed output of received packet headers and internal logic.
*   **Robust Signal Handling:** Captures `Ctrl+C` to ensure resources are freed and final statistics are printed before exiting.
*   **Containerized Environment:** Includes a Dockerized setup (Debian Wheezy based) to ensure consistent behavior across different environments and to provide necessary network permissions.

### Tech Stack
*   **Language:** C (C99/C11 standards)
*   **Protocols:** ICMP (Internet Control Message Protocol), IPv4
*   **System APIs:** BSD Sockets (`sys/socket.h`), Arp Argument Parsing (`argp.h`), Raw Sockets (`IPPROTO_ICMP`)
*   **Libraries:** `libm` (for mathematical calculations like standard deviation)
*   **Infrastructure:** Docker, Docker Compose, GNU Make

### Project Structure
```text
.
├── Dockerfile              # Environment configuration for testing
├── Makefile                # Build system (all, clean, fclean, re)
├── docker-compose.yml      # Multi-container orchestration for testing
├── includes/
│   └── ping.h              # Core structures and function prototypes
├── srcs/
│   ├── main.c              # Entry point, argument parsing, and stats logic
│   └── ping.c              # Core ICMP logic and socket management
└── tester.sh               # Automated test suite
```

### Installation

#### Prerequisites
*   A C compiler (GCC or Clang)
*   Make
*   `sudo` privileges (required for opening raw sockets)
*   *Optional:* Docker and Docker Compose

#### Build via Native Makefile
1.  Clone the repository:
    ```bash
    git clone https://github.com/username/ft_ping.git
    cd ft_ping
    ```
2.  Compile the project:
    ```bash
    make
    ```

#### Build via Docker
To run `ft_ping` in a controlled environment without worrying about local dependencies:
```bash
docker-compose up --build
```

### Usage
The binary requires root privileges to interact with raw sockets.

```bash
sudo ./ft_ping [OPTIONS] HOST
```

#### Arguments
*   `HOST`: The destination hostname or IP address (e.g., `google.com` or `8.8.8.8`).
*   `-v`, `--verbose`: Enable detailed output.
*   `-?`, `--help`: Display the help menu.

#### Example
```bash
$ sudo ./ft_ping google.com
FT_PING google.com (142.250.190.46): 56 data bytes
64 bytes from 142.250.190.46: icmp_seq=1 ttl=116 time=12.431 ms
64 bytes from 142.250.190.46: icmp_seq=2 ttl=116 time=11.102 ms
^C
--- google.com ping statistics ---
2 packets transmitted, 2 packets received, 0% packet loss
round-trip min/avg/max/stddev = 11.102/11.766/12.431/0.664 ms
```

### Technical Details
*   **Socket Type:** Uses `SOCK_RAW` with `IPPROTO_ICMP`, requiring the process to have `CAP_NET_RAW` capabilities or be run as root.
*   **RTT Calculation:** Employs `gettimeofday` to capture timestamps before sending and after receiving, utilizing `timersub` for precise delta calculation.
*   **Standard Deviation:** The `mdev` (mean deviation) is calculated using the variance formula: `sqrt((sum_sq / n) - (avg * avg))`.
*   **Memory Management:** Strictly follows C best practices to prevent leaks, especially during DNS resolution and signal interrupts.