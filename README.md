# Example C code for using MCP2210 with HIDAPI #

This project  uses HIDAPI for portability between Linux and Windows. 

## Pre-requisites ##
- MSYS64 (https://www.msys2.org/) \
&nbsp;  &nbsp; Run UCRT64 and install the following: \
&nbsp;  &nbsp;  &nbsp; &nbsp; pacman -S mingw-w64-ucrt-x86_64-toolchain \
&nbsp;  &nbsp;  &nbsp; &nbsp;&nbsp;  &nbsp;  &nbsp; &nbsp; select "All" and "Yes" 

- HIDAPI Version: hidapi-0.14.0 (https://github.com/libusb/hidapi/releases/tag/hidapi-0.14.0)

## Building in Windows ##
    ### Using Make ###
    1. Run "# mingw32-make" in root dir. 
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
    3. Ctrl+Shift+P -> CMake: Select kits -> GCC (ucrt64)
    3. "# cmake --build ."


## Building in Linux ##
    ### Using Make ###
    1. TBD

    ### Using CMake ###
    1. TBD


