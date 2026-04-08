################################################################
# This is the test application for the MCP2210 HIDAPI
# 
#  * This program is free software; you can redistribute it and/or modify it
#  * under the terms of the GNU General Public License as published by the Free
#  * Software Foundation; either version 2 of the License, or (at your option)
#  * any later version.
#  *
#  * This program is distributed in the hope that it will be useful, but WITHOUT
#  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  * more details.
#  *
#  * You should have received a copy of the GNU General Public License along
#  * with this program; If not, see <https://www.gnu.org/licenses/>.
# 
# Author: Joemel John Diente <JoemelJohn.Diente@microchip.com>
################################################################
cmake_minimum_required(VERSION 3.10)

## Project Name
project(mcp2210-hidapi-test-code)

message (STATUS "[message] Project name: ${CMAKE_PROJECT_NAME}")

# Passing CMAKE variables to source code.
configure_file(main.h.in main.h)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setting variables
option (IS_WIN "Building for Windows [ON], otherwise, its Linux [OFF]" ON)
option (IS_64 "Building for 64-bit [ON], otherwise 32-bit [OFF]" ON)

if (IS_WIN) ## Windows Executable ## 
    if (IS_64)
        set(ARCH "x64")
    else ()
        set(ARCH "x86")
    endif()

    message (STATUS "[message] Building for Windows ${ARCH}")
    # Add libraries in src/ directory
    add_subdirectory(src)

    # Output file
    add_executable(${CMAKE_PROJECT_NAME}-win32 main.c)

    # Copy .dll to where executable will be built
    configure_file(${CMAKE_SOURCE_DIR}/include/hidapi/${ARCH}/hidapi.dll ${PROJECT_BINARY_DIR} COPYONLY)

    # Add Library src/mcp2210-hidapi to this target
    target_link_libraries(${CMAKE_PROJECT_NAME}-win32 PUBLIC mcp2210-hidapi)

    # Add DLL
    target_link_libraries(${CMAKE_PROJECT_NAME}-win32 PRIVATE ${CMAKE_SOURCE_DIR}/include/hidapi/${ARCH}/hidapi.lib)

    # Include headers
    target_include_directories(${CMAKE_PROJECT_NAME}-win32 PUBLIC
    "${CMAKE_SOURCE_DIR}/include/mcp2210-hidapi"
    "${PROJECT_SOURCE_DIR}/include/hidapi/include"
    )

else () ## Linux Executable ## 

    message (STATUS "Building for Linux")
    message (STATUS "Linux is not yet implemented!!")
    add_executable(${CMAKE_PROJECT_NAME}-linux main.c)
    
    ## Future Plans ##
    # Check for cross compiler
    
endif()





