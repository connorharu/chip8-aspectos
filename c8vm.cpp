#include "c8vm.hpp"
#include <stdio.h>
#include <stdlib.h>

VM::VM(uint16_t pc_inicial) : tela(10) {
    this->PC = pc_inicial; // requisito valor passado por parametro
    for (int i = 0; i < 4096; i++){ this->RAM[i] = 0; }
    for (int i = 0; i < 16; i++){ this->V[i] = 0; }
    for (int i = 0; i < 16; i++){ this->stack[i] = 0; }
    for (int i = 0; i < 64 * 32; i++){ this->DISPLAY[i] = 0; }
    this->SP = 0;
    this->I = 0;

    uint8_t hexadecimais[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++) {
        RAM[i] = hexadecimais[i];
    }
    
    while(!tela.verificarFechar()){}
};

void VM::VM_CarregarROM(char* arq_rom) {
    ifstream rom(arq_rom, ios::binary); // abre como binario em modo leitura
    if(!rom.is_open()) { cerr << "não foi possivel abrir o arquivo" << arq_rom << endl; }

    rom.seekg(0, ios::end);
    size_t tamanho = rom.tellg(); // tamanho da ROM
    rom.seekg(0, ios::beg);

    if(tamanho > (4096 - this->PC)) {
        cerr << "rom muito grande pra caber na memória" << endl;
        rom.close();
        return;
    }

    char* destino = reinterpret_cast<char*>(this->RAM + this->PC); // colocar bytes na RAM a partir desse endereço
    rom.read(destino, tamanho); // bytes de tamanho 'tamanho'
    if(!rom) {
        cerr << "ocorreu um erro ao ler a ROM" << endl;
        rom.close();
        return;
    }

    rom.close();
    cout << "ROM carregada em 0x" << hex << this->PC << endl; // imprime hexadecimal

};

void VM::VM_ExecutarInstrucao(){
    uint8_t inst0 = this->RAM[this->PC];
    uint8_t inst1 = this->RAM[this->PC + 1];
    uint16_t inst = (inst0 << 8) | inst1;

    this->PC += 2;

    uint8_t grupo = inst >> 12;
    uint8_t X     = (inst & 0x0F00) >> 8;
    uint8_t Y     = (inst & 0x00F0) >> 4;
    uint8_t N     = inst & 0x000F;
    uint8_t NN    = inst & 0x00FF;
    uint8_t NNN   = inst & 0x0FFF;

    switch(grupo){

    case 0:
        // CLS
        if(inst == 0x00E0){
            for(int i = 0; i < 64 * 32; i++){
                this->DISPLAY[i] = 0;
            }
            break;
        }

    case 6:
        this->V[X] = NN;
        break;

    default:
        cout << "Grupo não implementado! Instrução: 0x" << (int)inst << endl;
        exit(1); //stdlib
    }
}

void VM::VM_ImprimirRegistradores(){
    cout << "PC: 0x" << std::hex << this->PC << " " << endl;
    cout << "I: 0x" << this->I << " " << endl;
    cout << "SP: 0x" << (int)this->SP << "\n" << endl;
    for(int i = 0; i < 16; i++)
        cout << "V[" << i << "]: 0x" << (int)this->V[i] << " " << endl;
}