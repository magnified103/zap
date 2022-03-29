#ifndef WRAPPER_COLOR_HPP
#define WRAPPER_COLOR_HPP

#include <SDL2/SDL.h>

#include "common.hpp"

namespace sdl {

class color {
public:
    color() : internal_color{} {};

    color(uint8 r, uint8 g, uint8 b, uint8 a) : internal_color{r, g, b, a} {}

    color(uint8 r, uint8 g, uint8 b) : internal_color{r, g, b, 0xff} {}

    uint8 red() { return internal_color.r; }

    uint8 green() { return internal_color.g; }

    uint8 blue() { return internal_color.b; }

    uint8 alpha() { return internal_color.a; }

private:
    SDL_Color internal_color;
};

} // namespace sdl

#endif // WRAPPER_COLOR_HPP