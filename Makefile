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
#  * Author: Joemel John Diente <JoemelJohn.Diente@microchip.com>
#

# User Variables
OS=WINDOWS_NT
ARCH=AMD64

# Do not modify
MY_FILE:=hidapi.dll
INCLUDE:=./include/mcp2210-hidapi/
INCLUDE_HIDAPI:=./include/hidapi/

# Check OS and ARCH then automatically set vars
ifeq ($(OS), WINDOWS_NT) #Windows
	OUTPUT=mcp2210-hidapi-win32
	ifeq ($(ARCH), AMD64)
		OS_ARCH=x64
	else
		OS_ARCH=x86
	endif
else #Linux (Not Yet Tested)
	OUTPUT:=mcp2210-hidapi-linux
endif

all: clean | build 

# Build: checks for OS dependencies before building. (For now all is built with -g)
build: update_dependency | clean
	gcc -g -c ./src/mcp2210-hidapi.c  -I${INCLUDE} -I${INCLUDE_HIDAPI}
	gcc -g -c ./src/mcp2210-hidapi-gpio.c  -I${INCLUDE} -I${INCLUDE_HIDAPI}
	gcc -g -c ./src/mcp2210-hidapi-spi.c -I${INCLUDE} -I${INCLUDE_HIDAPI}
	gcc -g -c ./src/mcp2210-hidapi-misc.c  -I${INCLUDE} -I${INCLUDE_HIDAPI}
	gcc -g -c ./main.c -I./include/hidapi/ -I${INCLUDE} -I${INCLUDE_HIDAPI}
	gcc -g  ./*.o -L./${OS_ARCH}/ -lhidapi -o ${OUTPUT}.exe 
# Clean: (-) makes sures that even with error next recipe will be executed.
clean:
	-rm *.exe *.o

run: build
	.\${OUTPUT}.exe 

debug: build 
	gdb .\${OUTPUT}.exe

update_dependency:
	-rm *.dll
	cp ./${OS_ARCH}/hidapi.dll ./

# Debugging Notes:
#
# Windows
# 	-If showing "skipping incompatible" check x86 or x64, this is known working on x64 (ARCH=AMD64)
#
# Linux
#