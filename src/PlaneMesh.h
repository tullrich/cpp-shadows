#ifndef _PLANE_MESH_H
#define _PLANE_MESH_H
#include <glm/glm.hpp>

#include <sstream>
using std::stringstream;
#include <unordered_map>
#include <utility>
using std::pair;

#include "shadows-common.h"
#include "md3_structs.h"

#define Plane_VERTS 4
#define Plane_TRIS 2

class PlaneMesh : public Mesh
{


public:
	PlaneMesh();
	~PlaneMesh();

	int inflate();
	void render();
	void onDrawShadowVolumes(glm::vec4 light_pos);
private:
	BufferObject m_colorBO, m_BO, m_normalBO;
	VirtualArrayObject m_VAO, m_silhouetteVAO;

	static md3_vertex verts[Plane_VERTS];
	static GLfloat colors[Plane_VERTS][3];
	static GLuint triangles[Plane_TRIS][3];
	static GLfloat normals[Plane_TRIS][3];
};



#endif /* _PLANE_MESH_H */