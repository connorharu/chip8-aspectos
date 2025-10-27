#include "c8vm.hpp"
#include "defs.hpp"
#include <stdio.h>
#include <stdlib.h>

VM::VM(uint16_t pc_inicial) : tela(10), teclado() {
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
    uint16_t NNN   = inst & 0x0FFF;

    #ifdef DEBUG
    cout << "Instrução executada: 0x" << (int)inst << endl;
    #endif

    switch(grupo){

    case 0: {
        // CLS
        if(inst == 0x00E0){
            for(int i = 0; i < 64 * 32; i++){
                this->DISPLAY[i] = 0;
            }
            break;
        }
        if(inst == 0x00EE){
            this->PC = stack[SP];
            this->SP--;
            break;
        }
        break;
    }

    case 1:
        this->PC = NNN;
        break;
    
    case 2:
        this->stack[SP] = this->PC;
        this->SP++;
        this->PC = NNN;
        break;

    case 3:
        if (this->V[X] == NN) this->PC += 2;
        break;

    case 4:
        if (this->V[X] != NN) this->PC += 2;
        break;

    case 5: 
        if (this->V[X] == this->V[Y]) this->PC += 2;
        break;

    case 6:
        this->V[X] = NN;
        break;

    case 7:
        this->V[X] += NN;
        break;

    case 8: {
        switch (N) {
        case 0:
            this->V[X] = this->V[Y];
            break;
        
        case 1:
            this->V[X] = this->V[X] | this->V[Y];
            break;

        case 2:
            this->V[X] = this->V[X] & this->V[Y];
            break;

        case 3:
            this->V[X] = this->V[X] ^ this->V[Y];
            break;

        case 4: {
            this->V[0xF] = 0;
            if (this->V[X] + this->V[Y] > 255) {
                this->V[0xF] = 1; // overflow
            }
            this->V[X] = this->V[X] + this->V[Y];
            break;
        }

        case 5: {
            this->V[0xF] = 1;
            if (this->V[X] < this->V[Y]) {
                this->V[0xF] = 0; // underflow
            }
            this->V[X] = this->V[X] - this->V[Y];
            break;
        }

        case 6: {
            // this->V[X] = this->V[Y]; // de acordo com a documentação isso pode ser configurável
            this->V[0xF] = this->V[X] & 0x01;
            this->V[X] = this->V[X] >> 1;
            break;
        }

        case 7: {
            this->V[0xF] = 1;
            if (this->V[Y] < this->V[X]) {
                this->V[0xF] = 0; // underflow
            }
            this->V[X] = this->V[Y] - this->V[X];
            break;
        }

        case 0xE: {
            // this->V[X] = this->V[Y]; // de acordo com a documentação isso pode ser configurável
            this->V[0xF] = (this->V[X] & 0x80) ? 1 : 0;
            this->V[X] = this->V[X] << 1;
            break;
        }

        default:
            cout << "Instrução não encontrada: 0x" << (int)inst << endl;
            exit(1); //stdlib
        }
    }

    case 9: 
        if (this->V[X] != this->V[Y]) this->PC += 2;
        break;

    case 0xA:
        this->I = NNN;
        break;
    
    case 0xB:
        this->PC = this->V[0] + NNN;
        break;

    case 0xC:
        this->V[X] = (rand() % 256) & NN;
        break;

    case 0xD: {
        uint8_t x = V[X] % 64;
        uint8_t y = V[Y] % 32;
        this->V[0xF] = 0;
        for (int linha_y = 0; linha_y < N; linha_y++) {
            uint8_t pixel = this->RAM[I + linha_y];

            for (int linha_x = 0; linha_x < 8; linha_x++) {
                if ((pixel & (0x80 >> linha_x)) != 0) { // testa cada bit
                    int px = (x + linha_x) % 64;
                    int py = (y + linha_y) % 32;
                    int pos = py * 64 + px;

                    if (this->DISPLAY[pos] == 1)
                        V[0xF] = 1; // houve colisão (não vi as consquências disso ainda)

                    this->DISPLAY[pos] ^= 1; // XOR
                }
            }
        }
        break;
    }

    case 0xE: {
        if(NN == 0x9E) {
            if (this->teclado.Pressionada(this->V[X])) this->PC += 2;
            break;
        }
        if(NN == 0xA1) {
            if (!this->teclado.Pressionada(this->V[X])) this->PC += 2;
            break;
        }
    }

    case 0xF: {

        switch(NN) {

            case 0x07: 
                this->V[X] = this->DT;
                break;

            case 0x15:
                this->DT = this->V[X];
                break;

            case 0x18:
                this->ST = this->V[X];
                break;

            case 0x1E: {
                this->V[0xF] = 0;
                this->I += V[X];
                if (this->I > 0x0FFF) {
                    this->V[0xF] = 1;
                }
                break;
            }

            case 0x0A: {
                bool alguma_pressionada = false;
                for (int i = 0; i < 16; i++) {
                    if (this->teclado.Pressionada(i)) {
                        this->V[X] = i;
                        alguma_pressionada = true;
                        break;
                    }
                }
                if (!alguma_pressionada) {
                    this->PC -= 2;
                }
                break;
            }

            case 0x29:
                this->I = this->V[X] * 5; // cada caractere tem 5 bytes
                break;

            case 0x33: {
                uint8_t val = this->V[X];
                this->RAM[I] = val / 100;
                this->RAM[I+1] = (val / 10) % 10;
                this->RAM[I+2] = val % 10;
                break;
            }

            case 0x55: {
                for (int i = 0; i < X + 1; i++) {
                    this->RAM[I+i] = this->V[0+i];
                }
                break;
            }

            case 0x65: {
                for (int i = 0; i < X + 1; i++) {
                    this->V[0+i] = this->RAM[I+i];
                }
                break;
            }

            default:
                cout << "Grupo não implementado! Instrução: 0x" << (int)inst << endl;
                exit(1); //stdlib
        }
    }

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
