#ifndef WRAPPER_SDL_HPP
#define WRAPPER_SDL_HPP

#include <SDL2/SDL.h>

#include "common.hpp"
#include "event.hpp"
#include "exception.hpp"
#include "geometry.hpp"
#include "logging.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl {

class sdl {
public:
    sdl(uint32 flags) {
        if (SDL_Init(flags) < 0) {
            throw sdl_exception();
        }
    }

    sdl() : sdl(SDL_INIT_EVERYTHING) {}

    ~sdl() noexcept { SDL_Quit(); }
};

void delay(uint32 ms) { SDL_Delay(ms); }

} // namespace sdl

#endif // WRAPPER_SDL_HPP