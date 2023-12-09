# Toolchain specific settings

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/avr-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/avr-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_DIR}/bin/avr-gcc)
set(CMAKE_LINKER ${TOOLCHAIN_DIR}/bin/avr-ld)
set(CMAKE_AR ${TOOLCHAIN_DIR}/bin/avr-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/avr-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIR}/bin/avr-objdump)
set(CMAKE_RANLIB ${TOOLCHAIN_DIR}/bin/avr-ranlib)
set(AVR_SIZE ${TOOLCHAIN_DIR}/bin/avr-size)

set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSS_COMPILING true)

macro(add_avr_executable target_name avr_mcu)

    set(elf_file ${target_name}-${avr_mcu}.elf)
    set(map_file ${target_name}-${avr_mcu}.map)
    set(hex_file ${target_name}-${avr_mcu}.hex)
    set(lst_file ${target_name}-${avr_mcu}.lst)

    # create elf file
    add_executable(${elf_file} ${Ultra_srcs})

    target_compile_options(${elf_file} PUBLIC ${ULTRA_COMPILE_FLAGS} ${ULTRA_LINK_FLAGS})
    target_compile_definitions(${elf_file} PRIVATE ${ULTRA_DEFINITIONS})
    target_include_directories(${elf_file} PRIVATE ${ULTRA_INC_DIR})
    target_link_libraries(${elf_file} PRIVATE core-wrapper accelstepper-wrapper)

    # generate the lst file
    add_custom_command(
        OUTPUT ${lst_file}

        COMMAND
            ${CMAKE_OBJDUMP} -h -S ${elf_file} > ${lst_file}

        DEPENDS ${elf_file}
    )

    # create hex file
    add_custom_command(
        OUTPUT ${hex_file}

        COMMAND
            ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}

        DEPENDS ${elf_file}
    )

    add_custom_command(
        OUTPUT "print-size-${elf_file}"

        COMMAND
            ${AVR_SIZE} ${elf_file}

        DEPENDS ${elf_file}
    )

    # build the intel hex file for the device
    add_custom_target(
        ${target_name}
        ALL
        DEPENDS ${hex_file} ${lst_file} "print-size-${elf_file}"
    )

    set_target_properties(
        ${target_name}

        PROPERTIES
            OUTPUT_NAME ${elf_file}
    )
endmacro(add_avr_executable)
