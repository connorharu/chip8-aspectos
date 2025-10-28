#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class AutoFalante {
public:
    AutoFalante();
    ~AutoFalante();
    void beep(bool tocar);
private:
    static bool ativo;
    static double fase;
    static bool dispositivoInicializado;
    static void audio_callback(void* userdata, Uint8* stream, int len);
};
