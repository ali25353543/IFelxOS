FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-multilib \
    g++-multilib \
    libc6-dev-i386 \
    qemu-system-x86 \
    xorriso \
    grub-pc-bin \
    grub-common \
    nasm \
    && rm -rf /var/lib/apt/lists/*

COPY . /app

WORKDIR /app

CMD ["make", "all"]
