all:
	gcc -c c8vm.cpp
	gcc -c main.cpp
	gcc -o chip8 c8vm.o main.o
	./chip8 1-chip8-logo.ch8
	
clean: rm -f c8vm.o main.o chip8	