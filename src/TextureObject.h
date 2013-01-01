#ifndef _TEXTURE_OBJECT_H 
#define _TEXTURE_OBJECT_H

#include "shadows-common.h"

class TextureObject 
{
public:
	TextureObject();
	~TextureObject();

	void uploadImage3D(tImage &img);
	void bind();
private:
	GLuint name, sname;
};

#endif /* _TEXTURE_OBJECT_H */