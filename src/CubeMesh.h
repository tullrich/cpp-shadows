#ifndef _CUBE_MESH_H
#define _CUBE_MESH_H
#include <glm/glm.hpp>



#include "shadows-common.h"
#include "md3_mesh.h"

#define NUM_VERTS 8
#define NUM_TRIS 12



class CubeMesh : public Mesh
{

public:
	CubeMesh();
	~CubeMesh();

	int inflate();
	void render();
	void onDrawSilhouette(glm::vec4 light_pos);
	void onDrawShadowVolumes(glm::vec4 light_pos);
	
	BufferObject m_colorBO, m_BO, m_normalBO;
	VirtualArrayObject m_VAO, m_silhouetteVAO;
	GLfloat colors[NUM_VERTS][3];
private:
	void calculateEdgeList();
	void calculateNormalKs();
	void renderPointLightQuads(glm::vec4 light_pos);
	void renderInfiniteLightTris(glm::vec4 light_pos);


	static md3_vertex verts[NUM_VERTS];
	static GLuint triangles[NUM_TRIS][3];
	static GLfloat normals[NUM_TRIS][3];
	static glm::vec4 normalKs[NUM_TRIS];

	EdgeMap m_edges;
};



#endif /* _CUBE_MESH_H */