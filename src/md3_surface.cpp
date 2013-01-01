#include "shadows-common.h" /* pch */

#include "md3_surface.h"
#include <GL/gl.h>

int MD3Surface::getNumTriangles() {
	return surf_header->num_triangles;
}

int MD3Surface::getNumVertices() {
	return surf_header->num_verts;
}

int MD3Surface::getNumShaders() {
	return surf_header->num_shaders;
}

tImage* MD3Surface::inflateFirstTexture()
{
	tImage *img;
	TextureManager *tm = TextureManager::getInstance();
	img = tm->registerTexture(shaders[0].name);

	return img;
}