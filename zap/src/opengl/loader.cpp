#include "opengl/loader.hpp"
#include "sdl2/SDL.hpp"
#include "opengl/gl.hpp"

gles_loader::gles_loader() {
    if (SDL_GL_LoadLibrary(nullptr) < 0) {
        throw sdl::sdl_exception();
    }

    // Use OpenGL 3.0 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
}
gles_loader::~gles_loader() { SDL_GL_UnloadLibrary(); }