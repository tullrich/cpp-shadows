#include "shadows-common.h" /* pch */

#include "stdio.h"
#include <iostream>
#include <sys/types.h>
#include "TextureManager.h"
#include "SOIL.h" 


TextureManager::TextureManager() : image_file_regex(IMAGE_STR_REGEX, std::regex_constants::ECMAScript | std::regex_constants::icase) {
	cout << "texture manager created" << endl;
}

TextureManager* TextureManager::getInstance()
{
	static TextureManager manager;

	return &manager;
}


tImage* TextureManager::_allocateTexture(string path) {
	tImage *img = new tImage;

	std::cout << "Allocating Image..." << std::endl;

	int width, height, channels;
	img->pixels = SOIL_load_image
		(
			path.c_str(),
			&width, &height, &channels,
			SOIL_LOAD_RGBA
		);

	img->height = height;
	img->width = width;
	img->channels = channels;

	if (img->pixels == NULL) {
		std::cout << "\tBad path: " << path << std::endl;
		// likely file not found, bail
		return NULL;
	}

	std::cout << "\tGood img read! Allocating Shader: " << path << std::endl;

	//SOIL_free_image_data( img->pixels );

	return img;
}

void TextureManager::_deallocateShader() 
{

}

tImage* TextureManager::getTextureByName(string name) 
{
	tTextureMap::iterator it;

	if (name.size() <= 0)
		return NULL;

	it = textures.find(name);

	if (it == textures.end()) {
		return NULL;
	}

	return (*it).second;
}

tImage* TextureManager::registerTexture(string path) 
{
	string trimmed_path, forced_ext;
	tImage *img;

	//std::count << path.length() << "test" << std::endl;
	printf("registeringShader:\n");

	if (path.length() <= 0) {
		return NULL;
	}

	trimmed_path = path.substr(0, path.find_last_of('.'));

	/* see if we've already loaded this shader */
	img = getTextureByName(trimmed_path);
	if (img != NULL) {
		std::cout << "\texisting texture found" << std::endl;
		return img;
	}

	/* check to see if this shader is just a static texture */
	if (regex_match(path.begin(), path.end(), image_file_regex)) {
		std::cout << "\tstatic texture detected: " << path << std::endl;
		img = _allocateTexture(path);

		// default path does not work, try .jpg instead
		if (img == NULL) {
			forced_ext = trimmed_path + DEFAULT_IMG_EXT;
			img = _allocateTexture(forced_ext);
		}
	} else {
		std::cout << "\tunsupported texture type " << path << std::endl;
	}

	if (img == NULL)
	{
		SHADOW_LOG_ERROR("UNABLE TO LOAD IMAGE");
		return NULL;
	}

	/* add this texture to the map */
	textures[trimmed_path] = img;

	return img;
}

void TextureManager::dumpTextures() 
{

	printf("dumping textures\n");
	for(auto textureIter = textures.begin(); textureIter != textures.end(); textureIter++) {
    	printf("\tKey: '%s' Location: %#x\n", (*textureIter).first.c_str(), (*textureIter).second);
	}
}
