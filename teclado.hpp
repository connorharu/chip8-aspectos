#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

class Teclado {
public:
    bool keys[16];
    bool prev_keys[16]; // estado anterior p/ debounce

    Teclado();

    void Atualizar();
    bool Pressionada(uint8_t chip8_key) const; // tecla foi pressionada?
    bool Debounce(uint8_t chip8_key) const; // arrumando o debounce
};
