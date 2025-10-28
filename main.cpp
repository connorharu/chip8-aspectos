#include "c8vm.hpp"
#include "defs.hpp"
#include "teclado.hpp"
#include "tela.hpp"
#include <SDL2/SDL.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc < 2) { cerr << "faltou argumentos" << endl; }

    VM vm(0x200);
    // Teclado teclado;

    vm.VM_CarregarROM(argv[1]);

    srand(10); // para instrução que precisa do rand()
    
    const int CPU_FREQ = 1000; // Frequência da CPU variável, aqui tá fixa por enquanto
    const int TIMER_FREQ = 60; // 60Hz fixos (timers e tela)
    const double CPU_INTERVAL = 1000.0 / CPU_FREQ; // em ms
    const double TIMER_INTERVAL = 1000.0 / TIMER_FREQ; // em ms

    Uint64 ultimo_tempo_cpu = SDL_GetPerformanceCounter();
    Uint64 ultimo_tempo_timer = SDL_GetPerformanceCounter();
    double cpu_acumulado = 0.0;
    double timer_acumulado = 0.0;

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

    while(1) {

        Uint64 agora = SDL_GetPerformanceCounter();
        double delta_ms = (double)(agora - ultimo_tempo_cpu) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        ultimo_tempo_cpu = agora;
        cpu_acumulado += delta_ms;
        timer_acumulado += delta_ms;

        vm.VM_AtualizarTeclado();

        while (cpu_acumulado >= CPU_INTERVAL) {
            vm.VM_ExecutarInstrucao();
            cpu_acumulado -= CPU_INTERVAL; // executa quantas instruções forem possíveis entre cada intervalo de CPU
        }

        if (timer_acumulado >= TIMER_INTERVAL) {
            timer_acumulado -= TIMER_INTERVAL; // espera para atualizar tela/timers sem ser ocupada
            vm.VM_AtualizarTimers();
            vm.VM_AtualizarTela();
            vm.VM_TocarSom();
        }

        SDL_Delay(1); // para executar mais tranquilo

        #ifdef DEBUG
        vm.VM_ImprimirRegistradores();
        #endif
    }

    SDL_Quit();

    return 0;
}