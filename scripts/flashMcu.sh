#!/bin/sh

#
# Flash the Arduino MCU using avrdude flash utility
#

set +eux


help()
{
    echo "Usage of $(basename $0) flash script"
    echo "\t" "-h" "\t" "prints this message"
    echo "\t" "-d" "\t" "flash debug build of arduino target"
    echo "\t" "-r" "\t" "flash release build of arduino target"
}

flash()
{
    # target mcu
    MCU=m328p
    # used programmer
    PROG=arduino
    # baudrate
    BAUD=115200
    # device file
    DEV=/dev/ttyACM0

    echo "===== Flashing $1 firmware ====="
    # TODO: path handling of firmware target
    avrdude -p $MCU -c $PROG -b $BAUD -P $DEV -U flash:w:build/$1/UltrAvr-atmega328p.elf
}

while getopts ":hrd" opt; do
    case $opt in
        h) # display Help
            help
            exit;;
        r) # build release
            flash "release"
            exit;;
        d) # build debug
            flash "debug"
            exit;;
    esac
done

echo "No option specified, try ./$(basename $0) -h"
