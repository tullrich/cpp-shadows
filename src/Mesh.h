#ifndef _MESH_H
#define _MESH_H
#include "glm/glm.hpp"

class Mesh
{
public:
	Mesh() : m_isInflated(false) {};
	virtual ~Mesh() {};

	virtual int inflate() = 0;
	virtual void render() = 0;
	virtual void onDrawSilhouette(glm::vec4 light_pos) {};
	virtual void onDrawShadowVolumes(glm::vec4 light_pos) = 0;

protected:
	bool m_isInflated;
};

#endif /* _MESH_H */