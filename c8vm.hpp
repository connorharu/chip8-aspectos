#pragma once 

#include <iostream>
#include <fstream>
#include <cstdint>
#include "tela.hpp"
#include "teclado.hpp"
// #include <cstring>
using namespace std;

class VM {
public: // acho que nao compensa fazer getter e setter pras variaveis

    Tela tela;
    Teclado teclado;
    
    uint16_t PC;            // Program Counter
    uint8_t SP;             // Stack Pointer
    uint16_t I;             // Registrador de índice
    uint8_t DT;             // Temporizador
    uint8_t ST;             // Temporizador de som
    uint8_t V[16];          // Registradores de propósito geral
    
    uint16_t stack[16];     // Pilha
    uint8_t RAM[4096];      // Memória 4KB
    uint8_t DISPLAY[64*32]; // Tela

    VM(uint16_t pc_inicial);

    void VM_CarregarROM(char* arq_rom);

    void VM_ExecutarInstrucao();

    void VM_ImprimirRegistradores();
};


