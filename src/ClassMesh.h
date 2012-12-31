#ifndef _CLASS_MESH_H
#define _CLASS_MESH_H
#include "shadows-common.h"

class ClassMesh : public Mesh
{
public:
	ClassMesh(Model model);
	~ClassMesh();

	int inflate();
	void render();
	void onDrawSilhouette(glm::vec4 light_pos);
	void onDrawShadowVolumes(glm::vec4 light_pos);
private:
	void calculateEdgeList();
	void calculateNormalKs();

	Model m_Model;

	BufferObject m_colorBO, m_BO, m_normalBO;
	VirtualArrayObject m_VAO, m_silhouetteVAO;
};

#endif