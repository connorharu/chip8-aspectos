#include "c8vm.hpp"
#include "defs.hpp"
#include "teclado.hpp"
#include "tela.hpp"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "autofalante.hpp"

int main(int argc, char** argv){
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " arquivo_rom [-e escala da tela] [-f freq da cpu] [-c pc inicial]\n";
        return 1;
    }

    int TELA_ESCALA = 10;
    uint16_t PC_INICIAL = 0x200;
    int CPU_FREQ = 1000;

    // lê parâmetros opcionais
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'e': // escala
                    if (i + 1 < argc) TELA_ESCALA = atoi(argv[++i]);
                    else cerr << "Faltou valor para -e\n";
                    break;

                case 'f': // frequência
                    if (i + 1 < argc) CPU_FREQ = atoi(argv[++i]);
                    else cerr << "Faltou valor para -f\n";
                    break;

                case 'c': // PC inicial
                    if (i + 1 < argc) PC_INICIAL = strtol(argv[++i], nullptr, 0); // aceita 0x...
                    else cerr << "Faltou valor para -c\n";
                    break;

                default:
                    cerr << "Opção desconhecida: " << argv[i] << endl;
                    break;
            }
        }
    }

    AutoFalante autofalante;
    Tela tela(TELA_ESCALA);
    VM vm(PC_INICIAL, tela, autofalante);


    if(vm.VM_CarregarROM(argv[1])){
        return 1;
    }

    srand(10); // para instrução que precisa do rand()
    
    
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