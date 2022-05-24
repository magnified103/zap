#ifndef SDL2_SDL_HPP
#define SDL2_SDL_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "sdl2/detail/common.hpp"
#include "sdl2/handlers/event.hpp"
#include "sdl2/handlers/exception.hpp"
#include "sdl2/handlers/logging.hpp"
#include "sdl2/types/geometry.hpp"
#include "sdl2/types/renderer.hpp"
#include "sdl2/types/surface.hpp"
#include "sdl2/types/texture.hpp"
#include "sdl2/types/window.hpp"

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

class ttf {
public:
    ttf() {
        if (TTF_Init() < 0) {
            throw ttf_exception();
        }
    }

    ~ttf() noexcept { TTF_Quit(); }
};

inline void delay(Uint32 ms) { SDL_Delay(ms); }

inline int set_relative_mouse_mode(bool enabled) {
    return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}

inline int gl_set_swap_interval(int interval) { return SDL_GL_SetSwapInterval(interval); }

} // namespace sdl

#endif // SDL2_SDL_HPP