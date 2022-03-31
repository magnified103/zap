#ifndef WRAPPER_COLOR_HPP
#define WRAPPER_COLOR_HPP

#include <SDL2/SDL.h>

#include "common.hpp"

namespace sdl {

class color {
public:
    color() : sdl_color{} {};

    color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : sdl_color{r, g, b, a} {}

    color(Uint8 r, Uint8 g, Uint8 b) : sdl_color{r, g, b, 0xff} {}

    Uint8 red() const { return sdl_color.r; }

    Uint8 &red() { return sdl_color.r; }

    Uint8 green() const { return sdl_color.g; }

    Uint8 &green() { return sdl_color.g; }

    Uint8 blue() const { return sdl_color.b; }

    Uint8 &blue() { return sdl_color.b; }

    Uint8 alpha() const { return sdl_color.a; }

    Uint8 &alpha() { return sdl_color.a; }

private:
    SDL_Color sdl_color;
};

} // namespace sdl

#endif // WRAPPER_COLOR_HPP