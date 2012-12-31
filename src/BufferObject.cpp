#include "BufferObject.h"

BufferObject::BufferObject()
{
	glGenBuffers(1, &name);
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &name);
}

void BufferObject::setData(int datalength ,const GLvoid *data)
{
	bind();
	
 	glBufferData(GL_ARRAY_BUFFER, datalength, data, GL_STATIC_DRAW);
}

void BufferObject::bind()
{
 	glBindBuffer(GL_ARRAY_BUFFER, name);
}

GLuint BufferObject::ID()
{
	return name;
}