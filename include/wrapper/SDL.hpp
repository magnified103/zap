#ifndef WRAPPER_SDL_HPP
#define WRAPPER_SDL_HPP

#include <SDL2/SDL.h>

#include "common.hpp"
#include "event.hpp"
#include "exception.hpp"
#include "geometry.hpp"
#include "logging.hpp"
#include "renderer.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl {

class sdl {
public:
    sdl(Uint32 flags) {
        if (SDL_Init(flags) < 0) {
            throw sdl_exception();
        }
    }

    sdl() : sdl(SDL_INIT_EVERYTHING) {}

    ~sdl() noexcept { SDL_Quit(); }
};

class img {
public:
    img(int flags) {
        if (IMG_Init(flags) != flags) {
            throw img_exception();
        }
    }

    img() : img(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) {}

    ~img() noexcept { IMG_Quit(); }
};

inline void delay(Uint32 ms) { SDL_Delay(ms); }

inline int set_relative_mouse_mode(bool enabled) {
    return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}

inline int gl_set_swap_interval(int interval) {
    return SDL_GL_SetSwapInterval(interval);
}

} // namespace sdl

#endif // WRAPPER_SDL_HPP