#include "opengl/context.hpp"
#include "opengl/gl.hpp"

context_loader::context_loader(SDL_Window *window)
    : gl_context{SDL_GL_CreateContext(window)} {
    if (!gl_context) {
        throw sdl::sdl_exception();
    }
#define SDL_GL_InitializeFunction(name)                                        \
    if (!(name = reinterpret_cast<decltype(name)>(                             \
              SDL_GL_GetProcAddress(#name)))) {                                \
        throw sdl::sdl_exception();                                            \
    }
    // shader programs
    SDL_GL_InitializeFunction(glCreateProgram);
    SDL_GL_InitializeFunction(glUseProgram);
    SDL_GL_InitializeFunction(glCreateShader);
    SDL_GL_InitializeFunction(glShaderSource);
    SDL_GL_InitializeFunction(glCompileShader);
    SDL_GL_InitializeFunction(glGetShaderiv);
    SDL_GL_InitializeFunction(glGetShaderInfoLog);
    SDL_GL_InitializeFunction(glAttachShader);
    SDL_GL_InitializeFunction(glLinkProgram);
    SDL_GL_InitializeFunction(glGetProgramiv);
    SDL_GL_InitializeFunction(glGetProgramInfoLog);
    SDL_GL_InitializeFunction(glDetachShader);
    SDL_GL_InitializeFunction(glDeleteShader);
    SDL_GL_InitializeFunction(glDeleteProgram);

    // utilities
    SDL_GL_InitializeFunction(glEnable);
    SDL_GL_InitializeFunction(glDepthFunc);
    SDL_GL_InitializeFunction(glGetError);
    SDL_GL_InitializeFunction(glDebugMessageCallbackKHR);
    SDL_GL_InitializeFunction(glBlendFunc);

    // program input capabilities
    SDL_GL_InitializeFunction(glGetUniformLocation);
    SDL_GL_InitializeFunction(glUniform1i);
    SDL_GL_InitializeFunction(glUniformMatrix4fv);

    // VBOs
    SDL_GL_InitializeFunction(glEnableVertexAttribArray);
    SDL_GL_InitializeFunction(glDisableVertexAttribArray);
    SDL_GL_InitializeFunction(glGenBuffers);
    SDL_GL_InitializeFunction(glBindBuffer);
    SDL_GL_InitializeFunction(glBufferData);
    SDL_GL_InitializeFunction(glVertexAttribPointer);
    SDL_GL_InitializeFunction(glDeleteBuffers);

    // drawing
    SDL_GL_InitializeFunction(glClear);
    SDL_GL_InitializeFunction(glClearColor);
    SDL_GL_InitializeFunction(glDrawArrays);

    // load textures
    SDL_GL_InitializeFunction(glActiveTexture);
    SDL_GL_InitializeFunction(glGenTextures);
    SDL_GL_InitializeFunction(glBindTexture);
    SDL_GL_InitializeFunction(glTexImage2D);
    SDL_GL_InitializeFunction(glTexParameteri);
    SDL_GL_InitializeFunction(glGenerateMipmap);
#undef SDL_GL_InitializeFunction
}