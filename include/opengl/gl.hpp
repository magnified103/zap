#ifndef OPENGL_GL_HPP
#define OPENGL_GL_HPP

#define GL_GLES_PROTOTYPES 0

#include <SDL2/SDL_opengles2.h>

// shader programs
extern GL_APICALL GLuint (GL_APIENTRYP glCreateProgram) (void);
extern GL_APICALL void (GL_APIENTRYP glUseProgram) (GLuint program);
extern GL_APICALL GLuint (GL_APIENTRYP glCreateShader) (GLenum type);
extern GL_APICALL void (GL_APIENTRYP glShaderSource) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern GL_APICALL void (GL_APIENTRYP glCompileShader) (GLuint shader);
extern GL_APICALL void (GL_APIENTRYP glGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
extern GL_APICALL void (GL_APIENTRYP glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern GL_APICALL void (GL_APIENTRYP glAttachShader) (GLuint program, GLuint shader);
extern GL_APICALL void (GL_APIENTRYP glLinkProgram) (GLuint program);
extern GL_APICALL void (GL_APIENTRYP glGetProgramiv) (GLuint program, GLenum pname, GLint *params);
extern GL_APICALL void (GL_APIENTRYP glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern GL_APICALL void (GL_APIENTRYP glDetachShader) (GLuint program, GLuint shader);
extern GL_APICALL void (GL_APIENTRYP glDeleteShader) (GLuint shader);
extern GL_APICALL void (GL_APIENTRYP glDeleteProgram) (GLuint program);

// utilities
extern GL_APICALL void (GL_APIENTRYP glEnable) (GLenum cap);
extern GL_APICALL void (GL_APIENTRYP glDepthFunc) (GLenum func);
extern GL_APICALL GLenum (GL_APIENTRYP glGetError) (void);
extern GL_APICALL void (GL_APIENTRYP glDebugMessageCallbackKHR) (GLDEBUGPROCKHR callback, const void *userParam);

// program input capabilities
extern GL_APICALL GLint (GL_APIENTRYP glGetUniformLocation) (GLuint program, const GLchar *name);
extern GL_APICALL void (GL_APIENTRYP glUniform1i) (GLint location, GLint v0);
extern GL_APICALL void (GL_APIENTRYP glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// VBOs
extern GL_APICALL void (GL_APIENTRYP glEnableVertexAttribArray) (GLuint index);
extern GL_APICALL void (GL_APIENTRYP glDisableVertexAttribArray) (GLuint index);
extern GL_APICALL void (GL_APIENTRYP glGenBuffers) (GLsizei n, GLuint *buffers);
extern GL_APICALL void (GL_APIENTRYP glBindBuffer) (GLenum target, GLuint buffer);
extern GL_APICALL void (GL_APIENTRYP glBufferData) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern GL_APICALL void (GL_APIENTRYP glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern GL_APICALL void (GL_APIENTRYP glDeleteBuffers) (GLsizei n, const GLuint *buffers);

// drawing
extern GL_APICALL void (GL_APIENTRYP glClear) (GLbitfield mask);
extern GL_APICALL void (GL_APIENTRYP glClearColor) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern GL_APICALL void (GL_APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count);

// load textures
extern GL_APICALL void (GL_APIENTRYP glActiveTexture) (GLenum texture);
extern GL_APICALL void (GL_APIENTRYP glGenTextures) (GLsizei n, GLuint *textures);
extern GL_APICALL void (GL_APIENTRYP glBindTexture) (GLenum target, GLuint texture);
extern GL_APICALL void (GL_APIENTRYP glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern GL_APICALL void (GL_APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param);
extern GL_APICALL void (GL_APIENTRYP glGenerateMipmap) (GLenum target);

#endif // OPENGL_GL_HPP