#ifndef _TEXTURED_MESH_H 
#define _TEXTURED_MESH_H

#include "CubeMesh.h"

/* 
	Mesh using a diffuse/ambient uvmap but hardcoded specular uvmap (for now) */

class TexturedMesh : public CubeMesh
{
public:
	TexturedMesh(string uvmap_path, string bumpmap_path, string specmap_path);
	~TexturedMesh();

	void render();
	int inflate();
protected:
	GLfloat tangents[NUM_VERTS][4];
	GLfloat binormals[NUM_VERTS][3];
private:
	void calculateTangentBasis();

	tImage *m_diffmap, *m_bumpmap, *m_specmap;
	TextureObject m_diffTO, m_bumpTO, m_specTO;

	BufferObject m_tangentBO;
};

#endif /* _TEXTURED_MESH_H */