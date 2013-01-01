#ifndef _TEXTURED_MESH_H 
#define _TEXTURED_MESH_H

#include "CubeMesh.h"

/* 
	Mesh using a diffuse/ambient uvmap but hardcoded specular uvmap (for now) */

class TexturedMesh : public CubeMesh
{
public:
	TexturedMesh(string uvmap_path);
	~TexturedMesh();

	void render();
private:
	tImage *m_diffmap;
	TextureObject m_diffTO;
};

#endif /* _TEXTURED_MESH_H */