cmake_minimum_required(VERSION 3.15)
project(IRdecoderLib)

# Set Arduino toolchain file
set(ARDUINO_DEFAULT_BOARD arduino:avr:mega)
set(ARDUINO_DEFAULT_PROGRAMMER arduino)

# Specify additional Arduino libraries if needed
# list(APPEND ARDUINO_LIBS <library1> <library2> ...)

# Add source files for the entire project
set(SOURCE_FILES
    IRdecoder/IRdecoder.cpp
    Buffer/Buffer.cpp
    tests/tests.ino
)

