# Toolchain specific settings

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSS_COMPILING true)

# AVR Compiler & Tools
set(CMAKE_C_COMPILER "${TOOLCHAIN_DIR}/bin/avr-gcc")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/avr-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_DIR}/bin/avr-gcc)
set(CMAKE_LINKER ${TOOLCHAIN_DIR}/bin/avr-ld)
set(CMAKE_AR ${TOOLCHAIN_DIR}/bin/avr-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/avr-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIR}/bin/avr-objdump)
set(CMAKE_RANLIB ${TOOLCHAIN_DIR}/bin/avr-ranlib)
set(AVR_SIZE ${TOOLCHAIN_DIR}/bin/avr-size)
