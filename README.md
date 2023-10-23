# Ultramat-Arduino

Arduino Firmware for Ultramat-Project.

## Setup

Before building the source code, you need to get the `avr-gcc` toolchain.
Its located [here](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz).

Extract it to a known location.

> **NOTE**
> If you place it under `/opt/` you need root priviliges, prefix every command with `sudo`

```sh
# Download toolchain and move it to known location
wget -v https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz -P /opt

# Extract sources from archive
tar -xf avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz

# Configure CMake targets:
./bootstrap.sh
```

## Build Firmware

Build the firmware for release using:
```sh
cmake --build build/rls/
```
