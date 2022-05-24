#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP

#include "sdl2/SDL.hpp"
#include "opengl/gl.hpp"

struct context_loader {
    SDL_GLContext gl_context;
    context_loader(SDL_Window *window);
};

#endif // OPENGL_CONTEXT_HPP