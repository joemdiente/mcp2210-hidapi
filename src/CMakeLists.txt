################################################################
# MCP2210 HIDAPI Lbirary CMakeLists.txt
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

add_library(mcp2210-hidapi
        mcp2210-hidapi-gpio.c
        mcp2210-hidapi-misc.c
        mcp2210-hidapi-spi.c
        mcp2210-hidapi.c
)
# Add library 
target_link_libraries(mcp2210-hidapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/hidapi/${ARCH}/hidapi.lib)

# Include mcp2210-hidapi and hidapi headers
target_include_directories(mcp2210-hidapi 
                        PUBLIC
                        "${CMAKE_CURRENT_SOURCE_DIR}/../include/mcp2210-hidapi" 
                        "${CMAKE_CURRENT_SOURCE_DIR}/../include/hidapi/include" 
                        )

