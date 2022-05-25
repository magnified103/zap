#include "opengl/loader.hpp"
#include "opengl/gl.hpp"
#include "sdl2/SDL.hpp"

gles_loader::gles_loader() {
    if (SDL_GL_LoadLibrary(nullptr) < 0) {
        throw sdl::sdl_exception();
    }

    // Use OpenGL 4.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
}
gles_loader::~gles_loader() { SDL_GL_UnloadLibrary(); }