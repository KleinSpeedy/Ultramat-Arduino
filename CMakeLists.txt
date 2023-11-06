cmake_minimum_required(VERSION 3.16)

#
# Project Configurations
#

# source directory
set(ULTRA_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
# include directory
set(ULTRA_INC_DIR ${CMAKE_SOURCE_DIR}/include)
# locate avr toolchain, get from bootstrap script
set(TOOLCHAIN_DIR ${AVR_TOOLCHAIN_PATH} CACHE STRING "User specified toolchain path")
# include avr toolchain script
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/scripts/cmake/toolchain-avr.cmake)
# Include compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Project CPU, TODO: Set differently
set(ULTRA_MCU "atmega328p")

project(UltrAvr C CXX ASM)


#
# Source files
#
include(${ULTRA_SRC_DIR}/Filelist.cmake)


#
# Linker Flags
#
set(ULTRA_LINKER_FLAGS
    "-lc"
    "-lm"
    "-lgcc"
    "-mmcu=${ULTRA_MCU}"
    "-Wl"
)


#
# Compile Flags
#
set(ULTRA_COMPILE_FLAGS
    "-mmcu=${ULTRA_MCU}"
    "-Os"
    "-std=gnu++11"
    "-fno-exceptions"
    "-ffunction-sections"
    "-fdata-sections"
)


#
# Definitions
#
set(ULTRA_DEFINITIONS
    "-DF_CPU=16000000UL"
)


#
# External libraries
#
set(CORE_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/Arduino-Core CACHE STRING "")
set(CORE_WRAPPER_DEFS ${ULTRA_DEFINITIONS} CACHE STRING "")
set(CORE_WRAPPER_OPTS ${ULTRA_COMPILE_FLAGS} CACHE STRING "")

add_subdirectory(ext/Arduino-Core)


#
# Firmware Targets
#

# define all target names
set(ELF_TARGET ${PROJECT_NAME}-${ULTRA_MCU}.elf)
set(HEX_TARGET ${PROJECT_NAME}-${ULTRA_MCU}.hex)
set(MAP_TARGET ${PROJECT_NAME}-${ULTRA_MCU}.map)
set(LST_TARGET ${PROJECT_NAME}-${ULTRA_MCU}.lst)

# add ELF target
add_executable(${ELF_TARGET} ${Ultra_srcs})

# Add target flags
target_compile_options(${ELF_TARGET} PRIVATE ${ULTRA_FLAGS})
target_compile_definitions(${ELF_TARGET} PRIVATE ${ULTRA_DEFINITIONS})

# Add include directories
target_include_directories(${ELF_TARGET} PRIVATE ${ULTRA_INC_DIR})
target_link_libraries(${ELF_TARGET} PRIVATE core-wrapper)

#
# Custom Targets
#

# print size of memory regions
add_custom_command(
    OUTPUT "print-size-${ELF_TARGET}"

    COMMAND
        ${AVR_SIZE} ${ELF_TARGET}

    DEPENDS ${elf_file}
)

# generate the lst file
add_custom_command(
    OUTPUT ${LST_TARGET}
    COMMAND
        ${CMAKE_OBJDUMP} -h -S ${ELF_TARGET} > ${LST_TARGET}
    DEPENDS ${ELF_TARGET}
)

# create hex file
add_custom_command(
    OUTPUT ${HEX_TARGET}
    COMMAND
        ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${ELF_TARGET} ${HEX_TARGET}
    DEPENDS ${ELF_TARGET}
)

# build the intel hex file for the device
add_custom_target(
    ${PROJECT_NAME}
    ALL
    DEPENDS ${ELF_TARGET} ${LST_TARGET} "print-size-${ELF_TARGET}"
)
