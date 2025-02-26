OUTPUT=mcp2210-hidapi-win32

all:
	gcc -g main.c -L./x64 -lhidapi -I./include -o ${OUTPUT}.exe 
clean:
	rm *.exe 
run:
	.\${OUTPUT}.exe 
debug:
	gdb .\${OUTPUT}.exe