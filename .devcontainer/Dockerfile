FROM ubuntu:noble

RUN apt update && apt install -y \
    cmake \
    fish \
    git \
    g++\
    g++-aarch64-linux-gnu \
    binutils \
    binutils-aarch64-linux-gnu

RUN chsh -s /usr/bin/fish

WORKDIR /workspace