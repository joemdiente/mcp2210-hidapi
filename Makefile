OUTPUT=mcp2210-hidapi-win32

all: clean | build 

build:
	gcc -g -c ./mcp2210-hidapi-misc.c  -I./include/ -I./
	gcc -g -c ./mcp2210-hidapi.c  -I./include/ -I./
	gcc -g -c ./main.c -I./include/
	gcc -g  ./*.o -L./x64/ -lhidapi -o ${OUTPUT}.exe 
clean:
	rm *.exe *.o
run: build
	.\${OUTPUT}.exe 
debug:
	gdb .\${OUTPUT}.exe