#ifndef WRAPPER_SDL_HPP
#define WRAPPER_SDL_HPP

#include <SDL2/SDL.h>

#include "exception.hpp"
#include "geometry.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl {

class sdl {
public:
    sdl() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw sdl_exception();
        }
    }

    sdl(Uint32 flags) {
        if (SDL_Init(flags) < 0) {
            throw sdl_exception();
        }
    }
    ~sdl() noexcept { SDL_Quit(); }
};

void delay(Uint32 ms) { SDL_Delay(ms); }

}

#endif // WRAPPER_SDL_HPP