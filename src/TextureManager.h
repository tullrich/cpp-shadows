#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include <map>
#include <string>
#include <regex>
#include "shadows-common.h"

#define IMAGE_STR_REGEX ".*"
#define DEFAULT_IMG_EXT ".jpg"

class TextureManager {

	typedef std::map<string, tImage*> tTextureMap;

protected:
	tImage* _allocateTexture(string path);
	void _deallocateShader();

public:
	static TextureManager* getInstance();

	TextureManager();
	tImage* registerTexture(string path);
	tImage* getTextureByName(string name);
	void dumpTextures();

private:
	tTextureMap textures;
	std::regex image_file_regex;

};

#endif