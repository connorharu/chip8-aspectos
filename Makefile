all:
	g++ c8vm.cpp main.cpp -o chip8
	
clean: rm -f c8vm.o main.o chip8	