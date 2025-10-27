#include "c8vm.hpp"
#include "defs.hpp"
#include "teclado.hpp"
#include "tela.hpp"
#include <SDL2/SDL.h>

int main(int argc, char** argv){
    if(argc < 2) { cerr << "faltou argumentos" << endl; }

    VM vm(0x200);
    Teclado teclado;

    vm.VM_CarregarROM(argv[1]);

    vm.VM_ImprimirRegistradores();

    cout << "primeiros 16 bytes da RAM a partir do PC:" << endl;
    for (int i = 0; i < 16; i++) {
        printf("%02X ", vm.RAM[vm.PC + i]);
    }
    cout << endl;

    cout << "testando o teclado, CLICA DENTRO DA TELA SDL PRA CONSEGUIR TESTAR\n" << endl;

    bool sair = false;
    while(!sair) {
        teclado.Atualizar();

        for(int i = 0; i < 16; i++) {
            if(teclado.Debounce(i)) {
                cout << "a tecla " << hex << i << " foi pressionada\n" << endl;
            }
        }

        SDL_Delay(50);
    }

    SDL_Quit();

    // comentado por agora
    // #ifdef DEBUG
    // vm.VM_ImprimirRegistradores();
    // #endif

    // while(1){
    //     vm.VM_ExecutarInstrucao();
    //     #ifdef DEBUG
    //     vm.VM_ImprimirRegistradores();
    //     #endif
    // }

    return 0;
}