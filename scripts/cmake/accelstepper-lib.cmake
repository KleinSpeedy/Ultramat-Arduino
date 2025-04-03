# Path to AccelStepper source directory
set(ACCELSTEPPER_SOURCE_DIR
    "${CMAKE_SOURCE_DIR}/ext/AccelStepper/src"
    CACHE STRING "Path to the Accelstepper library sources")

# Add accelstepper header files
include_directories(${ACCELSTEPPER_SOURCE_DIR})

add_library(libAccelstepper STATIC
    ${ACCELSTEPPER_SOURCE_DIR}/AccelStepper.cpp
    ${ACCELSTEPPER_SOURCE_DIR}/MultiStepper.cpp
)
