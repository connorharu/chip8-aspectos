all:
	g++ c8vm.cpp main.cpp tela.cpp teclado.cpp autofalante.cpp -o chip8 -lSDL2
	
clean: rm -f c8vm.o main.o chip8	