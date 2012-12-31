#ifndef _MD3_SURFACE_H
#define _MD3_SURFACE_H
#include "md3_structs.h"
#include <list>

#include "shadows-common.h"

class MD3Surface {
public:
	md3_surface *surf_header;
	md3_shader *shaders;
	md3_triangle *triangles;
	md3_texcoord *sts;
	md3_vertex *xyznormals;

	int getNumTriangles();
	int getNumVertices();
	int getNumShaders();

	tImage* inflateFirstTexture();
};

void printSurface(MD3Surface* t);
void printSurfaces(int num_surfaces, MD3Surface* t);
void printSurfacePrimatives(MD3Surface* s);

#endif
