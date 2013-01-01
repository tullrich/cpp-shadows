#include "shadows-common.h"

TextureObject::TextureObject()
{
	glGenTextures(1, &name);
	glGenSamplers(1, &sname);
}

TextureObject::~TextureObject()
{
	glDeleteVertexArrays(1, &name);
	glDeleteSamplers(1, &sname);
}

void TextureObject::uploadImage3D(tImage &img)
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, name);
	glBindSampler(0, sname);

 	cout << img.height << " x  " << img.width 
 		<< " depth " << img.channels << endl; 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);

	unsigned char *temp = new unsigned char[img.height * img.width * 4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

void TextureObject::bind()
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, name);
	glBindSampler(0, sname);
}