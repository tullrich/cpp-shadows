#ifndef __SHADER_UTILS_H
#define __SHADER_UTILS_H

#include <GL/glew.h>

int loadShader(const char* path, GLchar** destination, unsigned long* len);

void createShader(GLuint *shaderO, GLenum type, const char* filepath);
void linkProgram(GLuint *program, GLuint* shaders, int length);

#endif