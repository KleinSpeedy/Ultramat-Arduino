#!/bin/sh

set +eux

# Check all commands exist
for cmd in avrdude
do
    if ! command -v ${cmd} >/dev/null 2>&1
    then
        echo "Requires $cmd command!"
        exit 1
    fi
done

FIRMWARE=${1-unset}
PORT=${2-unset}

help()
{
    echo "Usage for $(basename $0) script:"
    echo "./$(basename $0) [ Firmware File] [ Serial Port ]"
}

error()
{
    echo "Error: $1" 1>&2
    if [ $2 ]; then
        help
    fi
    exit 1
}

if [ ! $# -eq 2 ]; then
    error "Invalid number of arguments" 1
fi

# Locate file
if [ ! -f $FIRMWARE ]; then
    error "Could not find $FIRMWARE" 1
fi

# Locate port
if [ ! -c $PORT ]; then
    error "Could not open or find $PORT" 1
fi

# Flash hexfile using avrdude
avrdude -v -patmega328p -carduino \
    -P$PORT -b115200 \
    -D -Uflash:w:$FIRMWARE:i
