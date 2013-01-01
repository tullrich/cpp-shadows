#include "shadows-common.h" /* pch */

#include "PlaneMesh.h"

md3_vertex PlaneMesh::verts[] =  
	{
		{5000, 0, 5000},
		{-5000, 0, 5000},
		{5000, 0, -5000},
		{-5000, 0, -5000},
	};

GLfloat PlaneMesh::colors[][3] =  
	{
		{1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0},
		{0.0, 1.0, 0.0},
		{0.0, 1.0, 0.0},

	};

GLuint PlaneMesh::triangles[][3] = 
{
	{2, 1, 0},
	{1, 2, 3},

};

GLfloat PlaneMesh::normals[][3];

PlaneMesh::PlaneMesh()
{
	
	m_BO.setData(Plane_VERTS * sizeof(md3_vertex), verts);
	m_normalBO.setData(Plane_TRIS * sizeof(GLfloat) * 3, NULL);
	m_colorBO.setData(Plane_VERTS * sizeof(GLfloat) * 3, colors);


	
	m_VAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);
	m_VAO.bindAttribToBuffer(SHADER_NORMAL, GL_FLOAT, sizeof(GLfloat) * 3, m_normalBO);
	m_VAO.bindAttribToBuffer(SHADER_AMB, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_DIFF, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_SPEC, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	
	m_silhouetteVAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);

}

PlaneMesh::~PlaneMesh()
{
}


int PlaneMesh::inflate()
{

	return 1;
}

void PlaneMesh::render()
{
	m_VAO.bind();
	//glEnableVertexAttribArray(SHADER_AMB);
	//glVertexAttrib4f(SHADER_AMB, 0.3, 0.3, 0.3, 1.0);
	//glVertexAttrib4f(SHADER_DIFF, 1.0, 1.0, 0.0, 1.0);
	//glVertexAttrib4f(SHADER_SPEC, 1.0, 1.0, 1.0, 1.0);
	glVertexAttrib1f(SHADER_SHINE, 1.0);

	glDrawElements(GL_TRIANGLES, Plane_TRIS * 3, 
		GL_UNSIGNED_INT, triangles);
}

void PlaneMesh::onDrawShadowVolumes(glm::vec4 light_pos)
{

}