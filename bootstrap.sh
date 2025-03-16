#!/bin/bash

#
# Bootstrap the Ultramat Arduino project, find avr toolchain, configure CMake
#

# TODO: Automate locating
TOOLCHAIN_DIR=/opt/avr8-gnu-toolchain-linux_x86_64

cmake -S . -B build \
    -DTOOLCHAIN_DIR:STRING=$TOOLCHAIN_DIR \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/scripts/cmake/toolchain-avr.cmake
