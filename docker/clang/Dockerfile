FROM ubuntu:latest
RUN apt-get update && apt-get install -y \
    clang \
    wget \
    make \
    git
RUN wget -qO- "https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local