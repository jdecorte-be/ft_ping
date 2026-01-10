
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


This is a professional `README.md` template tailored for your **ft_ping** project. Since `ft_ping` is a classic system programming project (often associated with the 42 school curriculum), this documentation focuses on network programming, raw sockets, and the ICMP protocol.

***

# ft_ping

![Banner](assets/banner.png)

## 📝 Overview
**ft_ping** is a custom implementation of the standard `ping` utility. It is designed to test the reachability of a host on an Internet Protocol (IP) network and to measure the round-trip time (RTT) for messages sent from the originating host to a destination computer.

This project focuses on **network programming** using **C**, specifically utilizing **Raw Sockets** to construct and send ICMP (Internet Control Message Protocol) packets and parse the received responses.

## ✨ Features
- **ICMP Protocol:** Manual construction of ICMP ECHO_REQUEST packets.
- **DNS Resolution:** Support for both IP addresses and hostnames (e.g., `google.com`).
- **Statistical Analysis:** Calculates and displays:
  - Packet loss percentage.
  - Round-trip time (min/avg/max/mdev).
- **Custom Options:**
  - `-v`: Verbose output.
  - `-h`: Display help/usage.
  - (Add any other flags you implemented, e.g., `-c` for count or `-t` for TTL).
- **Signal Handling:** Graceful termination and final statistics display on `Ctrl+C`.
- **Docker Integration:** Easy testing in a controlled environment.

## 🛠 Installation

### Prerequisites
- A Linux-based environment.
- `gcc` compiler.
- `make` build tool.
- **Root privileges** (required for using Raw Sockets).

### Build
Clone the repository and compile the project:
```bash
git clone https://github.com/your-username/ft_ping.git
cd ft_ping
make
```

### Docker (Alternative)
If you prefer to run it inside a container to avoid polluting your host system:
```bash
docker-compose up --build
```

## 🚀 Usage
Because the program uses **Raw Sockets**, it must be executed with root privileges (sudo):

```bash
sudo ./ft_ping <destination> [options]
```

### Examples
**Basic usage:**
```bash
sudo ./ft_ping google.com
```

**Verbose mode:**
```bash
sudo ./ft_ping 8.8.8.8 -v
```

### Options
| Flag | Description |
| :--- | :--- |
| `-v` | Verbose output (displays ICMP headers, etc.) |
| `-h` | Display usage information |

## 📂 Project Structure
```text
.
├── includes/          # Header files (.h)
│   └── ping.h
├── srcs/              # Source files (.c)
│   ├── main.c         # Entry point and argument parsing
│   └── ping.c         # Core logic (socket creation, send/recv)
├── assets/            # Project images and banners
├── Dockerfile         # Docker configuration
├── docker-compose.yml # Docker Compose orchestration
├── Makefile           # Compilation rules
├── tester.sh          # Automated testing script
└── LICENSE            # Project license
```

## 🧪 Testing
A testing script is provided to validate the program against different network scenarios:
```bash
chmod +x tester.sh
sudo ./tester.sh
```

## 📜 License
This project is licensed under the terms of the [LICENSE](LICENSE) file.

---
*Developed as part of a system programming curriculum.*