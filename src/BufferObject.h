#ifndef _BUFFER_OBJECT
#define _BUFFER_OBJECT

#include "GL/glew.h"

class BufferObject
{
public:
	BufferObject();
	~BufferObject();

	GLuint ID();
	void setData(int datalength ,const GLvoid *data);
	void bind();
private:
	GLuint name; 
};

#endif