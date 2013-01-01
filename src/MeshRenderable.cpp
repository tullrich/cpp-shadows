#include "shadows-common.h" /* pch */

#include "MeshRenderable.h"

MeshRenderable::MeshRenderable(Mesh *mesh, glm::vec3 position, float scale_factor) : m_mesh(mesh)
{
	this->position = position;
	m_scaleFactor = scale_factor;
	m_mesh->inflate();
	castsShadows = false;
}

MeshRenderable::~MeshRenderable()
{
}

void MeshRenderable::onDraw()
{
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glScalef(m_scaleFactor, m_scaleFactor, m_scaleFactor);
	m_mesh->render();
	glPopMatrix();
}

void MeshRenderable::onDrawSilhoutte(glm::vec4 light_pos)
{
	if (!castsShadows)
		return;
	
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glScalef(m_scaleFactor, m_scaleFactor, m_scaleFactor);
	m_mesh->onDrawSilhouette(light_pos);
	glPopMatrix();
}

void MeshRenderable::onDrawShadowVolumes(glm::vec4 light_pos, ShaderProgram &s)
{
	if (!castsShadows)
		return;

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glScalef(m_scaleFactor, m_scaleFactor, m_scaleFactor);

	glm::vec4 rel_light_pos = glm::vec4( 
		light_pos.x - position.x, 
		light_pos.y - position.y, 
		light_pos.z - position.z, 
		light_pos.w);

	s.setUniform4f("light_pos", 
    rel_light_pos.x, rel_light_pos.y, rel_light_pos.z, rel_light_pos.w);

	m_mesh->onDrawShadowVolumes(rel_light_pos);
	glPopMatrix();
}

void MeshRenderable::setPosition(glm::vec3 new_pos, float new_sf)
{
	position = new_pos;
	m_scaleFactor = new_sf;
}

void MeshRenderable::setPosition(glm::vec3 new_pos)
{
	position = new_pos;
}