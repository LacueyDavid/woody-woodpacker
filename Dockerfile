FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    gdb \
    nasm \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

CMD ["bash"]