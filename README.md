
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


This is a professional `README.md` template for your **ft_ping** project. It is designed to reflect the technical nature of a 42 Network project while remaining clear for any developer.

---

# README.md

```markdown
# ft_ping

![Build Status](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A custom implementation of the classic `ping` network utility. This project focuses on low-level network programming using raw sockets to send ICMP ECHO_REQUEST packets and handle ECHO_RESPONSE replies.

![Banner](assets/banner.png)

## 📝 Overview

`ft_ping` is a command-line utility developed to replicate the core functionality of the system `ping` command. By interacting directly with the IP layer via raw sockets, the tool measures round-trip time (RTT), detects packet loss, and provides network statistics for a given host.

This project was built to explore:
- **Raw Sockets:** Using `socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)`.
- **ICMP Protocol:** Constructing and parsing ICMP headers.
- **Signal Handling:** Managing interruptions (SIGINT) to display final statistics.
- **Network Calculations:** Precise timing and standard deviation (mdev) calculation for RTT.

## ✨ Features

- **IPv4 Support:** Ping hosts via IP address or DNS hostname.
- **Packet Statistics:** Displays transmitted/received packets, percentage of loss, and elapsed time.
- **RTT Metrics:** Calculates Min, Avg, Max, and Mdev (Mean Absolute Deviation).
- **Custom Options:** (Modify as per your implementation)
  - `-v`: Verbose output.
  - `-c <count>`: Stop after sending $N$ packets.
  - `-h`: Help menu.
- **Dockerized Environment:** Includes a Dockerfile and Compose setup for testing in isolated network environments.

## 🚀 Installation

### Prerequisites
- A Linux-based environment.
- `gcc` and `make`.
- **Root privileges:** Required to open raw sockets.

### Build
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ft_ping.git
   cd ft_ping
   ```
2. Compile the project:
   ```bash
   make
   ```

## 🛠 Usage

Basic usage:
```bash
sudo ./ft_ping google.com
```

### Options
| Flag | Description |
|------|-------------|
| `-v` | Verbose mode: display ICMP packets that are not ECHO_REPLY. |
| `-c` | Stop after sending a specific number of ECHO_REQUEST packets. |
| `-h` | Display usage instructions and exit. |

### Running with Docker
If you want to test the application in a containerized environment:
```bash
docker-compose up --build
```

### Running Tests
A `tester.sh` script is provided to validate the output against the system's native ping:
```bash
bash tester.sh
```

## 📁 Project Structure

```text
.
├── assets/               # Media resources (banners, screenshots)
├── includes/             # Header files
│   └── ping.h            # Main project definitions and structures
├── srcs/                 # Source files
│   ├── main.c            # Entry point and argument parsing
│   └── ping.c            # ICMP logic and socket management
├── Dockerfile            # Container definition
├── docker-compose.yml    # Orchestration for testing
├── Makefile              # Compilation instructions
├── tester.sh             # Automated testing script
└── LICENSE               # Project license
```

## ⚖️ License

This project is licensed under the [MIT License](LICENSE) - see the file for details.

---
*Developed as part of the 42 Network curriculum.*
```

### Tips for Customization:
1.  **Banner:** If you don't have a banner yet, you can create one or remove the image tag.
2.  **Flags:** I included standard flags like `-v` and `-c`. If your implementation uses different flags (e.g., `-t` for TTL or `-i` for interval), update the **Usage** table.
3.  **Permissions:** Since raw sockets require `CAP_NET_RAW`, emphasize the use of `sudo` in the usage section.