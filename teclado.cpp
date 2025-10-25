#include "teclado.hpp"

Teclado::Teclado() {
    for(int i = 0; i < 16; i++) keys[i] = false; // não há teclas pressionadas
}

void Teclado::Atualizar() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) exit(0);

        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool down = event.type == SDL_KEYDOWN;

            switch(event.key.keysym.sym) {
                case SDLK_1: keys[0x1] = down; break; // linha 1
                case SDLK_2: keys[0x2] = down; break;
                case SDLK_3: keys[0x3] = down; break;
                case SDLK_4: keys[0xC] = down; break;
                case SDLK_q: keys[0x4] = down; break; // linha 2
                case SDLK_w: keys[0x5] = down; break;
                case SDLK_e: keys[0x6] = down; break;
                case SDLK_r: keys[0xD] = down; break;
                case SDLK_a: keys[0x7] = down; break; // linha 3
                case SDLK_s: keys[0x8] = down; break;
                case SDLK_d: keys[0x9] = down; break;
                case SDLK_f: keys[0xE] = down; break;
                case SDLK_z: keys[0xA] = down; break; // linha 4
                case SDLK_x: keys[0x0] = down; break;
                case SDLK_c: keys[0xB] = down; break;
                case SDLK_v: keys[0xF] = down; break;

                default: break;
            }
        }
    }
}

bool Teclado::Pressionada(uint8_t chip8_key) const {
    if(chip8_key > 0xF) return false;
    return keys[chip8_key];
}