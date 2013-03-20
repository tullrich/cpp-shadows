#ifndef _TEXTURE_OBJECT_H 
#define _TEXTURE_OBJECT_H

#include "shadows-common.h"

class TextureObject 
{
public:
	TextureObject();
	~TextureObject();

	void uploadImage2D(GLuint index, tImage &img);
	void uploadImageCubeMap(tImage &img);
	void bind();
	void unbind();
private:
	GLuint name, sname;
	GLuint m_index;
};

#endif /* _TEXTURE_OBJECT_H */