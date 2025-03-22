# Example C code for using MCP2210 with HIDAPI #

This project  uses HIDAPI for portability between Linux and Windows. 
## Pre-requisites ##
MSYS64 (https://www.msys2.org/) \
HIDAPI Version: hidapi-0.14.0 (https://github.com/libusb/hidapi/releases/tag/hidapi-0.14.0) 

## Building in Windows ##
    "make" target
    all = use target build and run (Default)
    build = build program with -g
    clean = delete all .exe's in working directory
    run = run program
    debug = run gdb

    Note: Do not use VSCODE Run & Debug. Use "make" instead.