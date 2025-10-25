#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

class Teclado {
public:
    bool keys[16];

    Teclado();

    void Atualizar();
    bool Pressionada(uint8_t chip8_key) const; // tecla foi pressionada?
};
