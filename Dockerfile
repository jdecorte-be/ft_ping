FROM alpine:3.20

# Install lightweight build tools
RUN apk add --no-cache \
    build-base \
    linux-headers \
    tcpdump \
    iputils \
    bash

# Set working directory
WORKDIR /app

# Copy your ping project into the container
COPY . .

# Build your ping (modify if you use custom Makefile)
RUN make || { echo "Build failed"; exit 1; }

CMD ["/bin/sh"]
