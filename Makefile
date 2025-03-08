#
# User should select OS and ARCH
#
# Author: Joemel John Diente <joemdiente@gmail.com>
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