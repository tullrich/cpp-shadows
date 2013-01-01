#ifndef _SHADER_H
#define _SHADER_H

#include "shadows-common.h"


class ShaderProgram
{
public:
	ShaderProgram();

	int attachFileAs(const GLenum type, const string shaderpath);
	void start();
	int link();
	int activate();
	int getAttributeIndex(const char *name);
	void setUniform4f(const char* name, float v1, float v2, float v3, float v4);
	void setUniform1i(const char* name, int v1);
	GLuint program;
private:
	/* GL handles*/
	GLuint shaders[2];

};


#endif