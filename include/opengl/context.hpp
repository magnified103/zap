#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP

#include "opengl/gl.hpp"
#include "wrapper/SDL.hpp"

struct context_loader {
    SDL_GLContext gl_context;
    context_loader(SDL_Window *window);
};

#endif // OPENGL_CONTEXT_HPP