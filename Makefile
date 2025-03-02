OUTPUT=mcp2210-hidapi-win32

all: clean | build 

build:
	gcc -g -c ./src/mcp2210-hidapi-misc.c  -I./include/mcp2210-hidapi/
	gcc -g -c ./src/mcp2210-hidapi.c  -I./include/mcp2210-hidapi/ -I./include/hidapi/
	gcc -g -c ./main.c -I./include/hidapi/ -I./include/mcp2210-hidapi/
	gcc -g  ./*.o -L./x64/ -lhidapi -o ${OUTPUT}.exe 
clean:
	rm *.exe *.o
run: build
	.\${OUTPUT}.exe 
debug:
	gdb .\${OUTPUT}.exe