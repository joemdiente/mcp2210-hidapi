OUTPUT=mcp2210-hidapi-win32

all: clean | build

build:
	gcc -g main.c -L./x64/ -lhidapi -I./include/ -o ${OUTPUT}.exe 
clean:
	rm *.exe 
run: build
	.\${OUTPUT}.exe 
debug:
	gdb .\${OUTPUT}.exe