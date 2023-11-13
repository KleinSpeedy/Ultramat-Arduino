#!/bin/bash

#
# Bootstrap the Ultramat Arduino project, find avr toolchain, configure CMake
#

# TODO: Automate locating
TOOLCHAIN_DIR=/opt/avr8-gnu-toolchain-linux_x86_64
HERE=$(pwd)

function makeTarget()
{
    DEBUG=$1
    BUILD_DIR=$2

    echo "===================="
    echo "Building target: $BUILD_DIR"
    echo "===================="

    cmake \
        -B build/$2 \
        -S $HERE \
        -DULTRA_BUILD_DEBUG:NUMBER=$DEBUG \
        -DAVR_TOOLCHAIN_PATH:STRING=$TOOLCHAIN_DIR
}

makeTarget 0 release
makeTarget 1 debug
