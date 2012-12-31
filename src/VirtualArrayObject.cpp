#include "VirtualArrayObject.h"

VirtualArrayObject::VirtualArrayObject()
{
	glGenVertexArrays(1, &name);
}

VirtualArrayObject::~VirtualArrayObject()
{
	glDeleteVertexArrays(1, &name);
}

void VirtualArrayObject::bindAttribToBuffer(GLuint attrib, GLenum type, GLsizei stride, BufferObject& bo)
{
	bind();
 	glEnableVertexAttribArray(attrib); 

 	bo.bind();
 	glVertexAttribPointer( attrib, 3, type, GL_FALSE, stride, 0);
}

void VirtualArrayObject::disableAttribute(GLuint attrib)
{
	bind();
	glDisableVertexAttribArray(attrib);
}

void VirtualArrayObject::bind()
{
	glBindVertexArray(name);
}

GLuint VirtualArrayObject::ID()
{
	return name;
}