FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y git cmake gcc g++ clang-11 libcrypto++-dev libssl-dev

RUN git clone https://github.com/anbaccar/RSS_ring_ppml.git 

WORKDIR "/RSS_ring_ppml"

RUN ./ssh_keygen.sh

RUN mkdir build