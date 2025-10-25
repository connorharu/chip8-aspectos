#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

class Tela {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int largura, altura, escala;

public:
    Tela(int escalaFator);
    ~Tela();

    void exibirImagem(const uint8_t DISPLAY[64 * 32]);
    bool verificarFechar();
};
