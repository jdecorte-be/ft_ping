FROM debian:wheezy

ENV DEBIAN_FRONTEND=noninteractive

RUN echo "deb http://archive.debian.org/debian/ wheezy main contrib non-free" > /etc/apt/sources.list && \
    echo "deb http://archive.debian.org/debian-security/ wheezy/updates main contrib non-free" >> /etc/apt/sources.list

# -- install dependencies --
RUN apt-get -o Acquire::Check-Valid-Until=false update && \
    apt-get install -y --no-install-recommends --allow-unauthenticated \
        build-essential \
        gcc \
        make \
        wget \
        ca-certificates \
        autoconf \
        automake \
        libtool \
        pkg-config \
        libc6-dev \
        iputils-ping \
        net-tools \
        iproute \
        vim \
        sudo \
        netbase \
        bc \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# -- install inetutils without unneeded tools --
WORKDIR /tmp
RUN wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz && \
    tar xzf inetutils-2.0.tar.gz && \
    cd inetutils-2.0 && \
    ./configure \
        --disable-servers \
        --disable-ifconfig \
        --disable-logger \
        --disable-whois \
        --disable-ftp \
        --disable-talk \
        --disable-traceroute \
        --disable-rlogin \
        --disable-rsh \
        --disable-tftp \
        --disable-telnet \
        --disable-rexec \
        --disable-rcp \
        --disable-syslogd \
    && make -j$(nproc) \
    && make install \
    && cd / && rm -rf /tmp/inetutils-2.0*

# -- copy project files --
WORKDIR /ft_ping
COPY . .

RUN chmod +x tester.sh
RUN make re

CMD ["/bin/bash", "-c", "./tester.sh; exec /bin/bash"]