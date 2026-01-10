
<h1 align="center">
  <a href="https://github.com/jdecorte-be/ft_ping"><img src="assets/banner.png" alt="ft_ping" ></a>
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


This is a professional `README.md` template for your **ft_ping** project. Since `ft_ping` is a common systems programming project (notably in the 42 Network curriculum), I have filled in the details based on standard requirements for such a tool.

***

# ft_ping

A custom implementation of the standard `ping` command-line utility in C, using raw sockets to send and receive ICMP ECHO_REQUEST packets.

![C](https://img.shields.io/badge/language-C-blue.svg)
![Network](https://img.shields.io/badge/protocol-ICMP-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 📋 Overview

`ft_ping` is a network administration utility used to test the reachability of a host on an Internet Protocol (IP) network. It measures the round-trip time (RTT) for messages sent from the originating host to a destination computer that are echoed back to the source.

This project focuses on:
- Low-level network programming.
- Working with **Raw Sockets**.
- Constructing and parsing **ICMP** (Internet Control Message Protocol) headers.
- Accurate time measurement and statistical calculation.

## ✨ Features

- **DNS Resolution**: Supports both IP addresses and hostnames (e.g., `google.com`).
- **ICMP Protocol**: Custom construction of IP/ICMP packets.
- **Real-time Statistics**: Calculates min/avg/max/mdev RTT.
- **Signal Handling**: Graceful exit and summary report on `Ctrl+C`.
- **Custom Flags**:
  - `-v`: Verbose output.
  - `-h`: Display help/usage information.
- **Dockerized Environment**: Easily test the utility in a controlled containerized environment.

## 🛠 Installation

### Prerequisites
- A C compiler (e.g., `gcc` or `clang`).
- `make` build utility.
- Linux environment (Raw sockets are specific to the OS networking stack).

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ft_ping.git
   cd ft_ping
   ```
2. Compile the source:
   ```bash
   make
   ```

### Permission Note
Since `ft_ping` uses **Raw Sockets**, it requires root privileges to run:
```bash
sudo ./ft_ping google.com
```

## 🚀 Usage

```bash
Usage: ft_ping [options] <destination>

Options:
  -v        Verbose output.
  -h        Display this help message.
```

### Examples
**Standard Ping:**
```bash
sudo ./ft_ping 8.8.8.8
```

**Verbose Mode with Hostname:**
```bash
sudo ./ft_ping -v example.com
```

### Docker
If you prefer to run it inside a container:
```bash
# Using Docker Compose
docker-compose up --build

# Or using Dockerfile directly
docker build -t ft_ping .
docker run --cap-add=NET_RAW -it ft_ping ./ft_ping google.com
```

## 📁 Project Structure

```text
.
├── includes/
│   └── ping.h          # Header file containing structures and defines
├── srcs/
│   ├── main.c          # Entry point and argument parsing
│   └── ping.c          # Core logic (socket creation, send/receive loop)
├── Dockerfile          # Containerization instructions
├── docker-compose.yml  # Multi-container setup for testing
├── Makefile            # Build script
├── tester.sh           # Automated testing script
└── README.md           # Project documentation
```

## 🧪 Testing
The project includes a `tester.sh` script to validate the functionality against various scenarios (valid IPs, invalid hostnames, unreachable networks).

```bash
chmod +x tester.sh
./tester.sh
```

## ⚖️ License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute it.

---
*Note: This project was developed for educational purposes to understand the inner workings of the ICMP protocol and network system calls.*