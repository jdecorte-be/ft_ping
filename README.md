
<h1 align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="-b" alt="ft_ping" ></a>
  ft_ping
  <br>
</h1>

<p align="center">
  <a href="https://github.com/jdecorte-be/ft_ping">
    <img src="https://shields.io/badge/ft_ping-Security%20Research-critical?logoColor=white&labelColor=000000&color=8B0000"
         alt="ft_ping Security Research">
  </a>
  <a href="https://github.com/jdecorte-be/ft_ping">
    <img src="https://shields.io/badge/Platform-Linux%20x86__64-blue?logo=linux&logoColor=white&labelColor=000000"
         alt="Linux x86_64 Platform">
  </a>
  <a href="https://github.com/jdecorte-be/ft_ping/stargazers">
    <img src="https://shields.io/github/stars/jdecorte-be/ft_ping?logo=star&logoColor=white&labelColor=000000&color=yellow"
         alt="GitHub Stars">
  </a>
</p>

<p align="center">
  <a href="https://github.com/jdecorte-be/ft_ping/issues">
    <img src="https://shields.io/github/issues/jdecorte-be/ft_ping?logoColor=white&labelColor=000000&color=orange"
         alt="Open Issues">
  </a>
  <a href="https://github.com/jdecorte-be/ft_ping">
    <img src="https://shields.io/github/repo-size/jdecorte-be/ft_ping?logo=database&logoColor=white&labelColor=000000&color=purple"
         alt="Repository Size">
  </a>
  <a href="https://github.com/jdecorte-be/ft_ping">
    <img src="https://shields.io/github/languages/top/jdecorte-be/ft_ping?logo=code&logoColor=white&labelColor=000000&color=green"
         alt="Top Language">
  </a>
</p>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#technical-details">Technical Details</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#license">License</a>
</p>


This is a professional `README.md` template for your **ft_ping** project, tailored to the files provided and the typical requirements of this 42 Network project.

---

# ft_ping

A custom implementation of the standard network utility `ping` written in C. This project focuses on low-level network programming, raw sockets, and the ICMP protocol.

![C](https://img.shields.io/badge/language-C-blue.svg)
![Network](https://img.shields.io/badge/network-ICMP-orange.svg)
![Docker](https://img.shields.io/badge/docker-ready-brightgreen.svg)

## 📝 Overview

`ft_ping` is a command-line utility used to test the reachability of a host on an Internet Protocol (IP) network. It measures the round-trip time (RTT) for messages sent from the originating host to a destination computer that are echoed back to the source.

The project replicates the core functionality of the system's `ping` command by:
- Creating and managing **Raw Sockets** (`SOCK_RAW`).
- Constructing **ICMP Echo Request** packets.
- Handling **ICMP Echo Reply** packets.
- Calculating precise timing and packet loss statistics.

## ✨ Features

- **DNS Resolution**: Supports both IP addresses and hostnames.
- **ICMP Protocol**: Manual construction of ICMP headers.
- **Verbose Mode**: Detailed output including IP header information.
- **Statistics reporting**: Summary of packets sent/received, packet loss percentage, and RTT (min/avg/max/mdev).
- **Graceful handling**: Responds to `SIGINT` (Ctrl+C) to display final statistics.
- **Customizable**: Support for common flags (e.g., `-v` for verbose).

## 🚀 Installation

### Prerequisites
- A Linux-based environment.
- `gcc` and `make`.
- **Root privileges**: Since the program uses raw sockets, it must be run with `sudo` or have the `CAP_NET_RAW` capability.

### Building the project
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/ft_ping.git
   cd ft_ping
   ```
2. Compile using the Makefile:
   ```bash
   make
   ```

### Using Docker
If you prefer an isolated environment, use the provided Docker configuration:
```bash
docker-compose up --build
```

## 🛠 Usage

```bash
sudo ./ft_ping [options] <destination>
```

### Options
- `<destination>`: The hostname or IP address to ping.
- `-v`: Verbose output (displays ICMP packets that are not Echo Replies).
- `-h`: Display help/usage information.

### Example
```bash
$ sudo ./ft_ping google.com
PING google.com (142.250.184.238): 56 data bytes
64 bytes from 142.250.184.238: icmp_seq=1 ttl=117 time=14.221 ms
64 bytes from 142.250.184.238: icmp_seq=2 ttl=117 time=13.845 ms
^C
--- google.com ping statistics ---
2 packets transmitted, 2 packets received, 0% packet loss
round-trip min/avg/max/stddev = 13.845/14.033/14.221/0.188 ms
```

## 📁 Project Structure

| File/Folder | Description |
| :--- | :--- |
| `srcs/main.c` | Entry point, argument parsing, and signal handling. |
| `srcs/ping.c` | Core logic: packet construction, sending/receiving, and timing. |
| `includes/ping.h`| Structures, macros, and function prototypes. |
| `Makefile` | Build instructions (all, clean, fclean, re). |
| `Dockerfile` | Container configuration for testing. |
| `tester.sh` | Automated test suite to verify functionality and edge cases. |

## 🧪 Testing
The project includes a `tester.sh` script to validate the implementation against the system's native ping and check for memory leaks or crashes.

```bash
chmod +x tester.sh
sudo ./tester.sh
```

## 📜 License
This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details. (Note: Replace this with your preferred license).

---
*This project was developed as part of the curriculum at 42 Network.*