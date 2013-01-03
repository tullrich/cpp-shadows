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

void TextureObject::uploadImage2D(tImage &img)
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, name);
	glBindSampler(0, sname);

 	cout << img.height << " x  " << img.width 
 		<< " depth " << img.channels << endl; 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

void TextureObject::uploadImageCubeMap(tImage &img)
{
	glActiveTexture(GL_TEXTURE0 + 0);
	
	bind();

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img.height, img.width, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	unbind();
}

void TextureObject::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, name);
	glBindSampler(0, sname);
}

void TextureObject::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindSampler(0, 0);
}