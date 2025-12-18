FROM debian:wheezy

ENV DEBIAN_FRONTEND=noninteractive

# Use archive repos and install build tools
RUN echo "deb http://archive.debian.org/debian/ wheezy main contrib non-free" > /etc/apt/sources.list && \
    echo "deb http://archive.debian.org/debian-security/ wheezy/updates main contrib non-free" >> /etc/apt/sources.list && \
    apt-get -o Acquire::Check-Valid-Until=false update && \
    apt-get install -y --no-install-recommends --allow-unauthenticated \
        build-essential gcc make iputils-ping net-tools iproute vim sudo libc6-dev pkg-config \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


COPY . ./ft_ping

CMD ["/bin/bash"]
