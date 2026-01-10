
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


This is a professional `README.md` template for your **ft_ping** project, based on the files provided.

***

# ft_ping

![Banner](assets/banner.png)

## 📝 Overview
**ft_ping** is a custom implementation of the classic network utility `ping`. Developed in C, this project aims to replicate the core functionality of the system's `ping` command by using **ICMP (Internet Control Message Protocol)** to test the reachability of a host on an IP network.

This project involves low-level network programming, specifically the use of **Raw Sockets**, packet construction, and checksum calculation.

## ✨ Features
- **ICMP ECHO_REQUEST:** Sends ICMP packets to network hosts.
- **DNS Resolution:** Supports both IP addresses and hostnames (e.g., `google.com`).
- **Real-time Statistics:** Tracks packets sent, received, packet loss, and round-trip time (RTT) min/avg/max/mdev.
- **Verbose Mode:** Optional detailed output for debugging and packet inspection.
- **TTL Handling:** Manages Time-To-Live for packets.
- **Signal Handling:** Graceful exit and summary report upon receiving `SIGINT` (Ctrl+C).

## 🚀 Installation

### Prerequisites
- A Linux environment.
- `gcc` compiler and `make`.
- **Root privileges:** Required because the program uses Raw Sockets.

### Building the project
Clone the repository and compile using the provided `Makefile`:

```bash
git clone https://github.com/your-username/ft_ping.git
cd ft_ping
make
```

### Running with Docker
If you prefer an isolated environment, use the included Docker configuration:

```bash
docker-compose up --build
```

## 🛠 Usage
Since `ft_ping` uses raw sockets, it must be run with `sudo`:

```bash
sudo ./ft_ping <destination> [options]
```

### Examples
```bash
# Ping a hostname
sudo ./ft_ping google.com

# Ping an IP address
sudo ./ft_ping 8.8.8.8

# Run the automated tester
./tester.sh
```

### Options
*(Note: Adjust based on your actual implementation in `main.c`)*
- `-v`: Verbose output.
- `-h`: Display help message.

## 📁 Project Structure
```text
.
├── includes/
│   └── ping.h          # Header file containing structures and prototypes
├── srcs/
│   ├── main.c          # Entry point and argument parsing
│   └── ping.c          # Core logic (socket creation, sending, receiving)
├── assets/
│   └── banner.png      # Project visual
├── Dockerfile          # Container definition
├── docker-compose.yml  # Docker orchestration
├── Makefile            # Build instructions
├── tester.sh           # Automated testing script
└── README.md           # Project documentation
```

## 🛡 License
This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.
*(Alternatively: Distributed under the Peer-to-Peer License of 42 School.)*

---
*Created as part of the 42 School Curriculum.*