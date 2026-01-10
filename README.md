
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


# ft_ping

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)

A custom implementation of the system `ping` utility, developed in C. This project focuses on network programming using raw sockets to send and receive ICMP (Internet Control Message Protocol) packets.

![Banner](assets/banner.png)

## 📝 Overview

`ft_ping` is a command-line tool that verifies the reachability of a host on an Internet Protocol (IP) network. It measures the round-trip time (RTT) for messages sent from the originating host to a destination computer that are echoed back to the source.

This project replicates the core functionality of the standard `iputils-ping`, including packet construction, checksum calculation, and statistical analysis.

## ✨ Features

- **ICMP Echo Requests:** Sends standard ICMP Type 8 packets.
- **DNS Resolution:** Supports both IP addresses and hostnames.
- **RTT Statistics:** Calculates minimum, average, maximum, and standard deviation (mdev) of round-trip times.
- **Signal Handling:** Gracefully handles `SIGINT` (Ctrl+C) to display final statistics.
- **Verbose Mode:** Optional verbose output for debugging and detailed packet information.
- **Docker Support:** Includes Dockerfile and Docker Compose for testing in a controlled network environment.

## 🛠 Installation

### Prerequisites

- A Linux-based operating system.
- `gcc` compiler and `make`.
- Root privileges (required for opening **RAW Sockets**).

### Build

Clone the repository and compile the project using the provided `Makefile`:

```bash
git clone https://github.com/yourusername/ft_ping.git
cd ft_ping
make
```

### Environment (Docker)

If you prefer to run the project in a containerized environment:

```bash
docker-compose up --build
```

## 🚀 Usage

The binary requires root privileges to handle raw sockets.

```bash
sudo ./ft_ping <destination> [options]
```

### Examples

**Basic Ping:**
```bash
sudo ./ft_ping google.com
```

**Verbose Output:**
```bash
sudo ./ft_ping 8.8.8.8 -v
```

### Help
```text
Usage: ft_ping [DESTINATION] [-v]
Options:
  <destination>     dns name or ip address
  -v, --verbose     verbose output
```

## 📂 Project Structure

```text
.
├── assets/             # Project media and banners
├── includes/           # Header files
│   └── ping.h          # Main definitions and structures
├── srcs/               # Source files
│   ├── main.c          # Entry point and argument parsing
│   └── ping.c          # ICMP logic and socket handling
├── Dockerfile          # Container definition
├── docker-compose.yml  # Container orchestration
├── Makefile            # Build instructions
├── tester.sh           # Automated testing script
└── LICENSE             # Project license
```

## 🧪 Testing

A `tester.sh` script is provided to validate the program against various scenarios (valid hosts, invalid hosts, unreachable networks).

```bash
chmod +x tester.sh
sudo ./tester.sh
```

## 📄 License

This project is licensed under the [LICENSE](LICENSE) - see the file for details.

---
*This project was developed as part of a systems programming curriculum to understand the depths of the OSI Model and Network Layer protocols.*