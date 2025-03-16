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

if [ ! $# -eq 2 ]; then
    echo "Invalid number of arguments" 1>&2
    exit 1
fi

# Locate file
if [ ! -f $FIRMWARE ]; then
    echo "Could not find $FIRMWARE" 1>&2
    exit 1
fi

# Locate port
if [ ! -f $PORT ]; then
    echo "Could not find $PORT" 1>&2
    exit 1
fi

# Flash hexfile using avrdude
avrdude -v -patmega328p -carduino \
    -P$PORT -b115200 \
    -D -Uflash:w:$FIRMWARE:i
