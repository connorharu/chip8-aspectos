#include "autofalante.hpp"
#include <SDL2/SDL.h>
#include <iostream>

#define SAMPLE_RATE 44100
#define FREQUENCIA_BEEP 440
#define AMPLITUDE 28000 

bool AutoFalante::ativo = false;
double AutoFalante::fase = 0.0;
bool AutoFalante::dispositivoInicializado = false;

void AutoFalante::audio_callback(void* userdata, Uint8* stream, int len) {
    int16_t* buffer = reinterpret_cast<int16_t*>(stream);
    int samples = len / sizeof(int16_t);
    double periodo = (double)SAMPLE_RATE / FREQUENCIA_BEEP;

    for (int i = 0; i < samples; i++) {
        if (AutoFalante::ativo) {
            // Onda quadrada simples
            buffer[i] = ((int)(AutoFalante::fase++) % (int)periodo < periodo / 2)
                        ? AMPLITUDE : -AMPLITUDE;
            if (AutoFalante::fase > periodo) AutoFalante::fase -= periodo;
        } else {
            // silêncio
            buffer[i] = 0; 
        }
    }
}

AutoFalante::AutoFalante(){
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erro ao iniciar áudio SDL: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 512;
    spec.callback = AutoFalante::audio_callback;
    spec.userdata = this;

    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        std::cerr << "Erro ao abrir dispositivo de áudio: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_PauseAudio(0);
    dispositivoInicializado = true;
}

AutoFalante::~AutoFalante() {
    if (dispositivoInicializado) {
        SDL_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

void AutoFalante::beep(bool tocar) {
    ativo = tocar;
}
