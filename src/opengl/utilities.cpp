#include <cstring>
#include <string>
#include <vector>

#include "../../include/opengl/gl.hpp"
#include "../../include/opengl/utilities.hpp"
#include "../../include/wrapper/SDL.hpp"

GLuint load_shader_program(const std::string &vertex_shader_src,
                           const std::string &fragment_shader_src) {
    GLint succeeded = GL_TRUE;

    // vertex shader
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_shader_src_ptr = vertex_shader_src.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_shader_src_ptr, nullptr);
    sdl::log_info("Compiling vertex shader, shader id = %u", vertex_shader_id);
    glCompileShader(vertex_shader_id);
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
    if (!succeeded) {
        GLint info_log_length;
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> info_log(info_log_length);
        glGetShaderInfoLog(vertex_shader_id, info_log_length, nullptr, &info_log[0]);
        throw std::runtime_error("GL:" + std::string(info_log.begin(), info_log.end()));
    }

    // fragment shader
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_shader_src_ptr = fragment_shader_src.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_shader_src_ptr, nullptr);
    sdl::log_info("Compiling fragment shader, shader id = %u", fragment_shader_id);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
    if (!succeeded) {
        GLint info_log_length;
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> info_log(info_log_length);
        glGetShaderInfoLog(fragment_shader_id, info_log_length, nullptr, &info_log[0]);
        throw std::runtime_error("GL:" + std::string(info_log.begin(), info_log.end()));
    }

    // link them together
    GLuint shader_program_id = glCreateProgram();
    sdl::log_info("Attaching vertex shader to shader program, program id = %u", shader_program_id);
    glAttachShader(shader_program_id, vertex_shader_id);
    sdl::log_info("Attaching fragment shader to shader program");
    glAttachShader(shader_program_id, fragment_shader_id);
    sdl::log_info("Linking shader program");
    glLinkProgram(shader_program_id);
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &succeeded);
    if (!succeeded) {
        GLint info_log_length;
        glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> info_log(info_log_length);
        glGetProgramInfoLog(shader_program_id, info_log_length, nullptr, &info_log[0]);
        throw std::runtime_error("GL:" + std::string(info_log.begin(), info_log.end()));
    }

    // cleanup everything
    glDetachShader(shader_program_id, vertex_shader_id);
    glDeleteShader(vertex_shader_id);
    glDetachShader(shader_program_id, fragment_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader_program_id;
}

GLuint load_texture(const std::string &absolute_path) {
    sdl::surface old_surface(absolute_path);
    sdl::surface new_surface(SDL_CreateRGBSurface(0, old_surface.get()->w, old_surface.get()->h, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                                  0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
                                                  0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
                                                  ));
    // copy to new surface
    SDL_BlitSurface(old_surface.get(), nullptr, new_surface.get(), nullptr);

    SDL_Surface *surface = new_surface.get();

    // If the surface must be locked
    if (SDL_MUSTLOCK(surface)) {
        // Lock the surface
        SDL_LockSurface(surface);
    }

    char *pixels = reinterpret_cast<char *>(surface->pixels);

    // flip the surface
    for (int i = 0; i * 2 < surface->h - 1; i++) {
        int pitch = surface->pitch;
        std::vector<char> temp(pitch);
        std::memcpy(&temp[0], pixels + (i * pitch), pitch);
        std::memcpy(pixels + (i * pitch), pixels + ((surface->h - 1 - i) * pitch), pitch);
        std::memcpy(pixels + ((surface->h - 1 - i) * pitch), &temp[0], pitch);
    }

    // gl stuffs
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    // bind to context
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // copy from SDL_Surface to opengl
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_surface.get()->w, new_surface.get()->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, new_surface.get()->pixels);

    // Unlock surface
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    // When MAGnifying the image (no bigger mipmap available), use LINEAR
    // filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // When MINifying the image, use a LINEAR blend of two mipmaps, each
    // filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture_id;
}