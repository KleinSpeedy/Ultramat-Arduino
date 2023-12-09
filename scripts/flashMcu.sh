#!/bin/bash

#
# Flash the Arduino MCU using avrdude flash utility
#

set +eu

echo "===== Flashing firmware onto MCU ====="

# target mcu
MCU=m328p
# used programmer
PROG=arduino
# baudrate
BAUD=9600
# device file
DEV=/dev/ttyACM0

avrdude -p $MCU -c $PROG -b $BAUD -P $DEV -t
