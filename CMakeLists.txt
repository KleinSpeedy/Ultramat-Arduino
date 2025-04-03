cmake_minimum_required(VERSION 3.16)

# Use the AVR toolchain
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_SOURCE_DIR}/scripts/cmake/toolchain-avr.cmake"
        CACHE STRING "Toolchain file")
endif()

project(UltraArduino LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Arduino Core
include(${CMAKE_SOURCE_DIR}/scripts/cmake/arduino-core.cmake)

# Include AccelStepper library
include(${CMAKE_SOURCE_DIR}/scripts/cmake/accelstepper-lib.cmake)

# source directory
set(ULTRA_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
# include directory
set(ULTRA_INC_DIR ${CMAKE_SOURCE_DIR}/include)
# generate clang files
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Used mcu variant
set(MCU atmega328p)
# Compiler Flags
set(ULTRA_FLAGS
    "-Os"
    "-mmcu=${MCU}"
    "-Wall"
    "-Wextra"
    "-fno-exceptions"
    #"-fno-rtti"
    CACHE STRING "Compiler options")
# Compiler definitions
set(ULTRA_DEFINITIONS
    "-DF_CPU=16000000UL"
    "-DARDUINO=100"
    CACHE STRING "Compiler defines")
# Linker Flags
set(ULTRA_LINKER_FLAGS
    "-Wl,--gc-sections"
    "-mmcu=${MCU}"
    CACHE STRING "Linker options")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${ARDUINO_CORE_PATH})
include_directories(${ARDUINO_VARIANT_PATH})
include_directories(${CMAKE_BINARY_DIR})

# Source files
set(Ultra_srcs
    ${ULTRA_SRC_DIR}/util/ErrorHandling.cpp

    ${ULTRA_SRC_DIR}/main.cpp
    ${ULTRA_SRC_DIR}/Commands.cpp
)

# Firmware
add_executable(${PROJECT_NAME}.elf ${Ultra_srcs})

target_compile_options(${PROJECT_NAME}.elf PRIVATE ${ULTRA_FLAGS})
target_compile_options(ArduinoCore PRIVATE ${ULTRA_FLAGS})
target_compile_options(libAccelstepper PRIVATE ${ULTRA_FLAGS})

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${ULTRA_DEFINITIONS})
target_compile_definitions(ArduinoCore PRIVATE ${ULTRA_DEFINITIONS})
target_compile_definitions(libAccelstepper PRIVATE ${ULTRA_DEFINITIONS})

target_link_options(${PROJECT_NAME}.elf PRIVATE ${ULTRA_LINKER_FLAGS})
target_link_options(ArduinoCore PRIVATE ${ULTRA_LINKER_FLAGS})
target_link_options(libAccelstepper PRIVATE ${ULTRA_LINKER_FLAGS})

# Link Arduino core libraries and accelstepper library
target_link_libraries(${PROJECT_NAME}.elf PRIVATE ArduinoCore libAccelstepper)

# Post-build: Convert ELF to HEX for Arduino
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Converting ELF to HEX for Arduino upload"
)
# Generate version header file with project version
find_package(Git)
add_custom_target(version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/scripts/cmake/ultra_version.h.in
    -D DST=${CMAKE_BINARY_DIR}/ultra_version.h
    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
    -P ${CMAKE_SOURCE_DIR}/scripts/cmake/version-header.cmake
)
add_dependencies(${PROJECT_NAME}.elf version)
