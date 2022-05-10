#ifndef OPENGL_UTILITIES_HPP
#define OPENGL_UTILITIES_HPP

#include <string>

#include "gl.hpp"

[[nodiscard]] GLuint load_shader_program(const std::string &vertex_shader_src,
                           const std::string &fragment_shader_src);

[[nodiscard]] GLuint load_texture(const std::string &absolute_path);

#endif // OPENGL_UTILITIES_HPP