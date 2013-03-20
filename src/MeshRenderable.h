#ifndef _MESH_RENDERABLE_H
#define _MESH_RENDERABLE_H
#include "shadows-common.h"


class MeshRenderable : public Renderable
{
public:
	MeshRenderable(Mesh *mesh, glm::vec3 position = glm::vec3(0,0,0), float scale_factor = 1.0);
	~MeshRenderable();

	virtual void onDraw(glm::vec3 view_pos, glm::vec4 light_pos, ShaderProgram &s);
	virtual void onDrawSilhoutte(glm::vec4 light_pos);
	virtual void onDrawShadowVolumes(glm::vec4 light_pos,  ShaderProgram &s);
	virtual void setPosition(glm::vec3 new_pos, float new_sf);
	virtual void setPosition(glm::vec3 new_pos);
	bool castsShadows;
protected:
	Mesh *m_mesh;
private:
	glm::vec3 position;

	float m_scaleFactor;
};

#endif