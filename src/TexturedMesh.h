#ifndef _TEXTURED_MESH_H 
#define _TEXTURED_MESH_H

#include "CubeMesh.h"

/* 
	Mesh using a diffuse/ambient uvmap but hardcoded specular uvmap (for now) */

class TexturedMesh : public CubeMesh
{
public:
	TexturedMesh(string uvmap_path, string bumpmap_path);
	~TexturedMesh();

	void render();
	int inflate();
private:
	void calculateTangentBasis();

	tImage *m_diffmap, *m_bumpmap;
	TextureObject m_diffTO, m_bumpTO;

	BufferObject m_tangentBO;
};

#endif /* _TEXTURED_MESH_H */