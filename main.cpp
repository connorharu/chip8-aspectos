#include "c8vm.hpp"
#include "defs.hpp"
#include "teclado.hpp"
#include "tela.hpp"
#include <SDL2/SDL.h>

//Quantos quadros por segundo serão processados
#define FPS_ALVO 60

//ATRASO_FRAME é o número máximo de milisegundos que cada frame 
//deve ocupar
#define ATRASO_FRAME (1000/FPS_ALVO)

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

    // cout << "testando o teclado, CLICA DENTRO DA TELA SDL PRA CONSEGUIR TESTAR\n" << endl;

    // bool sair = false;
    // while(!sair) {
    //     teclado.Atualizar();

    //     for(int i = 0; i < 16; i++) {
    //         if(teclado.Debounce(i)) {
    //             cout << "a tecla " << hex << i << " foi pressionada\n" << endl;
    //         }
    //     }

    //     SDL_Delay(50);
    // }

    // SDL_Quit();

    // comentado por agora
    // #ifdef DEBUG
    // vm.VM_ImprimirRegistradores();
    // #endif

    while(1){

        Uint32 inicio_frame = SDL_GetTicks();

        vm.VM_ExecutarInstrucao();
        vm.teclado.Atualizar(); // pode ser encapsulado melhor

        Uint32 duracao_frame = SDL_GetTicks() - inicio_frame;
        if(duracao_frame < ATRASO_FRAME){
            SDL_Delay(ATRASO_FRAME - duracao_frame);
        }

        #ifdef DEBUG
        vm.VM_ImprimirRegistradores();
        #endif
        
    }

    return 0;
}