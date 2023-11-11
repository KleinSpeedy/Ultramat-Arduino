FROM ubuntu:22.04
LABEL Description="Ultramat-Arduino build environment"

ARG DEBIAN_FRONTEND=noninteractive
ARG TOOLCHAIN_AVR=avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64

WORKDIR /ultra-arduino

RUN apt-get update
RUN apt-get install -y --no-install-recommends \
    gcc g++ make cmake \
    git ca-certificates wget tar

RUN wget -v \
    https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/${TOOLCHAIN_AVR}.tar.gz \
    -P /opt

RUN cd /opt && tar xvf ${TOOLCHAIN_AVR}.tar.gz
