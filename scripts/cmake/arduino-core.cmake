# Path to Arduino AVR toolchain
set(ARDUINO_CORE_PATH
    "${CMAKE_SOURCE_DIR}/ext/ArduinoCore-avr/cores/arduino"
    CACHE STRING "Path to the core Arduino library")
set(ARDUINO_VARIANT_PATH
    "${CMAKE_SOURCE_DIR}/ext/ArduinoCore-avr/variants/standard"
    CACHE STRING "Path to the specific microcontroller variant")

# Include Arduino core and libraries
include_directories(${ARDUINO_CORE_PATH})
include_directories(${ARDUINO_VARIANT_PATH})

add_library(ArduinoCore STATIC
    ${ARDUINO_CORE_PATH}/main.cpp
    ${ARDUINO_CORE_PATH}/abi.cpp
    ${ARDUINO_CORE_PATH}/HardwareSerial.cpp
    ${ARDUINO_CORE_PATH}/HardwareSerial0.cpp
    ${ARDUINO_CORE_PATH}/Print.cpp

    ${ARDUINO_CORE_PATH}/wiring_digital.c
    ${ARDUINO_CORE_PATH}/wiring.c
    ${ARDUINO_CORE_PATH}/hooks.c
)
