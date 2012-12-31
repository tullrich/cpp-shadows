#include "Light.h"
#include "CubeMesh.h"
#include "GL/glu.h"

Light::Light(int x, int y, int z, bool isPoint) : MeshRenderable(new CubeMesh, glm::vec3(x, y, z), .5)
{
	position = glm::vec4(x, y , z, (isPoint) ? 1:0);

	CubeMesh *m_cube = (CubeMesh*)m_mesh;
	m_cube->colors[0][0] = 1.0;
	m_cube->colors[0][1] = 1.0;
	m_cube->colors[0][2] = 1.0;
	m_cube->colors[1][0] = 1.0;
	m_cube->colors[1][1] = 1.0;
	m_cube->colors[1][2] = 1.0;
	m_cube->colors[2][0] = 1.0;
	m_cube->colors[2][1] = 1.0;
	m_cube->colors[2][2] = 1.0;
	m_cube->colors[3][0] = 1.0;
	m_cube->colors[3][1] = 1.0;
	m_cube->colors[3][2] = 1.0;
	m_cube->colors[4][0] = 1.0;
	m_cube->colors[4][1] = 1.0;
	m_cube->colors[4][2] = 1.0;
	m_cube->colors[5][0] = 1.0;
	m_cube->colors[5][1] = 1.0;
	m_cube->colors[5][2] = 1.0;
	m_cube->colors[6][0] = 1.0;
	m_cube->colors[6][1] = 1.0;
	m_cube->colors[6][2] = 1.0;
	m_cube->colors[7][0] = 1.0;
	m_cube->colors[7][1] = 1.0;
	m_cube->colors[7][2] = 1.0;

	m_cube->m_colorBO.setData(8 * sizeof(GLfloat) * 3, m_cube->colors);
	m_cube->m_VAO.bindAttribToBuffer(SHADER_AMB, GL_FLOAT, sizeof(GLfloat) * 3, m_cube->m_colorBO);
	m_cube->m_VAO.bindAttribToBuffer(SHADER_DIFF, GL_FLOAT, sizeof(GLfloat) * 3, m_cube->m_colorBO);
	m_cube->m_VAO.bindAttribToBuffer(SHADER_SPEC, GL_FLOAT, sizeof(GLfloat) * 3, m_cube->m_colorBO);
	
}

Light::~Light()
{

}

glm::vec4 Light::getPosition()
{
	return position;
}

void Light::setPosition(glm::vec4 new_pos)
{
	MeshRenderable::setPosition(glm::vec3(new_pos));
	position = new_pos;
}

void Light::onDrawShadowVolumes(glm::vec4 light_pos)
{
	/* stop subclass shadow rendering */
}

void Light::activate()
{
	GLfloat specular[] = {0.5f, 0.5f, 0.5f, position.w};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, position.w };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	GLfloat ambient[] = { 0, 0, 0, position.w};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat light_p[4];
	light_p[0] = position.x;
	light_p[1] = position.y;
	light_p[2] = position.z;
	light_p[3] = position.w;
	glLightfv(GL_LIGHT0, GL_POSITION, light_p);

	glEnable(GL_LIGHT0);
}

void Light::deactivate()
{
	GLfloat specular[] = {0.0f, 0.0f, 0.0f, position.w};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat diffuse[] = {0.0f, 0.0f, 0.0f, position.w};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	GLfloat ambient[] = {0.0f, 0.0f, 0.0f, position.w};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glDisable(GL_LIGHT0);
}

void Light::setType(bool isPoint)
{
	position.w = (isPoint) ? 1.0 : 0.0;
}