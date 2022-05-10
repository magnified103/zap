#include "../../include/opengl/gl.hpp"

// shader programs
GL_APICALL GLuint (GL_APIENTRYP glCreateProgram) (void) = nullptr;
GL_APICALL void (GL_APIENTRYP glUseProgram) (GLuint program) = nullptr;
GL_APICALL GLuint (GL_APIENTRYP glCreateShader) (GLenum type) = nullptr;
GL_APICALL void (GL_APIENTRYP glShaderSource) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) = nullptr;
GL_APICALL void (GL_APIENTRYP glCompileShader) (GLuint shader) = nullptr;
GL_APICALL void (GL_APIENTRYP glGetShaderiv) (GLuint shader, GLenum pname, GLint *params) = nullptr;
GL_APICALL void (GL_APIENTRYP glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = nullptr;
GL_APICALL void (GL_APIENTRYP glAttachShader) (GLuint program, GLuint shader) = nullptr;
GL_APICALL void (GL_APIENTRYP glLinkProgram) (GLuint program) = nullptr;
GL_APICALL void (GL_APIENTRYP glGetProgramiv) (GLuint program, GLenum pname, GLint *params) = nullptr;
GL_APICALL void (GL_APIENTRYP glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = nullptr;
GL_APICALL void (GL_APIENTRYP glDetachShader) (GLuint program, GLuint shader) = nullptr;
GL_APICALL void (GL_APIENTRYP glDeleteShader) (GLuint shader) = nullptr;
GL_APICALL void (GL_APIENTRYP glDeleteProgram) (GLuint program) = nullptr;

// utilities
GL_APICALL void (GL_APIENTRYP glEnable) (GLenum cap) = nullptr;
GL_APICALL void (GL_APIENTRYP glDepthFunc) (GLenum func) = nullptr;
GL_APICALL GLenum (GL_APIENTRYP glGetError) (void) = nullptr;
GL_APICALL void (GL_APIENTRYP glDebugMessageCallbackKHR) (GLDEBUGPROCKHR callback, const void *userParam) = nullptr;

// program input capabilities
GL_APICALL GLint (GL_APIENTRYP glGetUniformLocation) (GLuint program, const GLchar *name) = nullptr;
GL_APICALL void (GL_APIENTRYP glUniform1i) (GLint location, GLint v0) = nullptr;
GL_APICALL void (GL_APIENTRYP glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = nullptr;

// VBOs
GL_APICALL void (GL_APIENTRYP glEnableVertexAttribArray) (GLuint index) = nullptr;
GL_APICALL void (GL_APIENTRYP glDisableVertexAttribArray) (GLuint index) = nullptr;
GL_APICALL void (GL_APIENTRYP glGenBuffers) (GLsizei n, GLuint *buffers) = nullptr;
GL_APICALL void (GL_APIENTRYP glBindBuffer) (GLenum target, GLuint buffer) = nullptr;
GL_APICALL void (GL_APIENTRYP glBufferData) (GLenum target, GLsizeiptr size, const void *data, GLenum usage) = nullptr;
GL_APICALL void (GL_APIENTRYP glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) = nullptr;
GL_APICALL void (GL_APIENTRYP glDeleteBuffers) (GLsizei n, const GLuint *buffers) = nullptr;

// drawing
GL_APICALL void (GL_APIENTRYP glClear) (GLbitfield mask) = nullptr;
GL_APICALL void (GL_APIENTRYP glClearColor) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = nullptr;
GL_APICALL void (GL_APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count) = nullptr;

// load textures
GL_APICALL void (GL_APIENTRYP glActiveTexture) (GLenum texture) = nullptr;
GL_APICALL void (GL_APIENTRYP glGenTextures) (GLsizei n, GLuint *textures) = nullptr;
GL_APICALL void (GL_APIENTRYP glBindTexture) (GLenum target, GLuint texture) = nullptr;
GL_APICALL void (GL_APIENTRYP glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = nullptr;
GL_APICALL void (GL_APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param) = nullptr;
GL_APICALL void (GL_APIENTRYP glGenerateMipmap) (GLenum target) = nullptr;