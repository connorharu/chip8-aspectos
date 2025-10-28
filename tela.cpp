#include "tela.hpp"
#include <iostream>

Tela::Tela(int escalaFator) {
    window = nullptr;
    renderer = nullptr;
    largura = 64;
    altura = 32;
    escala = escalaFator;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(
        "Chip8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        largura * escala,
        altura * escala,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Inicializa a tela preta
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

Tela::~Tela() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Tela::verificarFechar() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return true;
    }
    return false;
}

void Tela::exibirImagem(const uint8_t DISPLAY[64 * 32]) {
    // Limpa a tela antes de redesenhar
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Percorre todos os 2048 pixels (64 x 32)
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            uint8_t pixel = DISPLAY[y * 64 + x];
            if (pixel) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }

            SDL_Rect rect = { x * escala, y * escala, escala, escala };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Mostra o resultado na tela
    SDL_RenderPresent(renderer);
}