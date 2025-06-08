# Example C code for using MCP2210 with HIDAPI #

This project  uses HIDAPI for portability between Linux and Windows. 

## Pre-requisites ##
MSYS64 (https://www.msys2.org/) \
HIDAPI Version: hidapi-0.14.0 (https://github.com/libusb/hidapi/releases/tag/hidapi-0.14.0) 

## Building in Windows ##
    ### Using Make ###
    1. Run "# make" in root dir. 
        "make" target
        all = use target build and run (Default)
        build = build program with -g
        clean = delete all .exe's in working directory
        run = run program
        debug = run gdb

    Note: Do not use VSCODE Run & Debug. Use "make" instead.

    ### Using CMake ###
    1. Run "# mkdir build" in root dir.
    2. "# cd ./build/".
    3. "# cmake --build ."
        By default, builds WIN and x64 binary. Has *-win32.exe suffix.

## Building in Linux ##
    ### Using Make ###
    1. TBD

    ### Using CMake ###
    1. TBD


