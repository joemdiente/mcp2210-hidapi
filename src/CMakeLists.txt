################################################################
# MCP2210 HIDAPI Lbirary CMakeLists.txt
#
# Author: Joemel John Diente <joemdiente@gmail.com>
################################################################
cmake_minimum_required(VERSION 3.10)

add_library(mcp2210-hidapi
        mcp2210-hidapi-gpio.c
        mcp2210-hidapi-misc.c
        mcp2210-hidapi-spi.c
        mcp2210-hidapi.c
)
# Add library 
target_link_libraries(mcp2210-hidapi PRIVATE ${CMAKE_SOURCE_DIR}/${ARCH}/hidapi.lib)

# Include mcp2210-hidapi and hidapi headers
target_include_directories(mcp2210-hidapi 
                        PRIVATE
                        "${CMAKE_SOURCE_DIR}/include/mcp2210-hidapi" 
                        "${PROJECT_SOURCE_DIR}/include/hidapi" 
                        )

