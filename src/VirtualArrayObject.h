#ifndef _VIRTUAL_ARRAY_OBJECT
#define _VIRTUAL_ARRAY_OBJECT

#include "GL/glew.h"
#include "BufferObject.h"

class VirtualArrayObject
{
public:
	VirtualArrayObject();
	~VirtualArrayObject();

	GLuint ID();
	void bind();
	void bindAttribToBuffer(GLuint attrib, GLenum type, GLsizei stride, BufferObject& bo);
	void disableAttribute(GLuint attrib);
private:
	GLuint name;
};

#endif				