#include <fstream>
#include <cstdlib>
#include <iostream>

#include "shaderutils.h"

using namespace std;

int loadShader(const char* path, GLchar** destShader, unsigned long* len) {
	std::ifstream file;

	file.open(path, ios::in);
	if (!file) {
		return -1;
	}

    file.seekg(0,ios::end);
    *len = file.tellg();
    file.seekg(ios::beg);

	*destShader = new char[*len+1];
	if (destShader == 0) {
		return -1;
	}

	(*destShader)[*len] = 0;

	int i = 0;
	while (file.good()) {
       (*destShader)[i] = file.get();
       if (!file.eof())
       i++;
   }
    
   (*destShader)[i] = 0;
    
   file.close();
      
   return 0;
}

void createShader(GLuint *shaderO, GLenum type,  const char* filepath) {
	unsigned long len = 0;
	GLchar* shader = 0;

	if(loadShader(filepath, &shader, &len)) {
		printf("Could not load shader\n");
		return exit(-1);
	}

	int length = (int) len;
	
	*shaderO = glCreateShader(type);
	glShaderSource(*shaderO, 1, (const GLchar**) &shader, &length);
	glCompileShader(*shaderO);

	int isSafe = 0;
	glGetShaderiv(*shaderO, GL_COMPILE_STATUS, &isSafe);
	if (isSafe == false)
	{
	   	printf("Error compiling shader\n");

		int maxLength;
		glGetShaderiv(*shaderO, GL_INFO_LOG_LENGTH, &maxLength);
	 
		/* The maxLength includes the NULL character */
		char* shaderProgramInfoLog = new char[maxLength];
	 
		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetShaderInfoLog(*shaderO, maxLength, &maxLength, shaderProgramInfoLog);

		cout << shaderProgramInfoLog << endl;
	 
		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;

		return exit(-1);
	} 
	printf("Shader created\n");
}

void linkProgram(GLuint *program, GLuint* shaders, int length) {
	
	for (int i = 0; i < 2; i++) {
		if (shaders[i] != -1)
			glAttachShader(*program, shaders[i]);
	}
	glLinkProgram(*program); 

	GLint linked;
	glGetProgramiv(*program, GL_LINK_STATUS, &linked);
	if (linked == false)
	{
	   	printf("Error compiling program\n");

		int maxLength;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &maxLength);
	 
		/* The maxLength includes the NULL character */
		char* shaderProgramInfoLog = new char[maxLength];
	 
		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetProgramInfoLog(*program, maxLength, &maxLength, shaderProgramInfoLog);

		cout << shaderProgramInfoLog << endl;
	 
		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;
		return exit(-1);
	} 

	printf("program created\n");
}