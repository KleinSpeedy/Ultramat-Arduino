#!/bin/sh

set +eux

# Check all commands exist
for cmd in qemu-system-avr
do
    if ! command -v ${cmd} >/dev/null 2>&1
    then
        echo "Requires $cmd command!"
        exit 1
    fi
done

FIRMWARE="build/UltraArduino.elf"

if [ ! -f $FIRMWARE ]; then
    echo "Could not find $FIRMWARE" 1>&2
    exit 1
fi
# start QEMU with gdb instance waiting
qemu-system-avr -M arduino-uno -bios $FIRMWARE -s -S -d in_asm
