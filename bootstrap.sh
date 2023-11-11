#!/bin/bash

#
# Bootstrap the Ultramat Arduino project, find avr toolchain, configure CMake
#

# TODO: Automate locating
TOOLCHAIN_DIR=/opt/avr8-gnu-toolchain-linux_x86_64
HERE=$(pwd)

function makeTarget()
{
    NAME=$1
    BUILD_DIR=$2

    cmake \
        -B build/$2 \
        -S $HERE \
        -DAVR_TOOLCHAIN_PATH:STRING=$TOOLCHAIN_DIR
}

makeTarget release
