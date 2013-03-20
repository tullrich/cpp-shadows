#include "shadows-common.h" /* pch */

#include "PlaneMesh.h"

md3_vertex PlaneMesh::verts[] =  
	{
		{2500, 0, 2500}, /* bottom right */
		{-2500, 0, 2500}, /* bottom left*/
		{2500, 0, -2500}, /* top right */
		{-2500, 0, -2500}, /* top left */
	};

GLfloat PlaneMesh::colors[][3] =  
	{
		{1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0},
		{0.0, 1.0, 0.0},
		{0.0, 1.0, 0.0},

	};

GLfloat PlaneMesh::texcoords[][2] =
{
	{0.0, 0.0},
	{0.0, 1.0},
	{1.0, 0.0},
	{1.0, 1.0}
};

GLuint PlaneMesh::triangles[][3] = 
{
	{2, 1, 0},
	{1, 2, 3},

};

GLfloat PlaneMesh::normals[][3] =
{
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0}
};

PlaneMesh::PlaneMesh()
{
	
	m_BO.setData(Plane_VERTS * sizeof(md3_vertex), verts);
	m_normalBO.setData(Plane_VERTS * sizeof(GLfloat) * 3, normals);
	m_colorBO.setData(Plane_VERTS * sizeof(GLfloat) * 3, colors);
	m_texcooBO.setData(Plane_VERTS * sizeof(GLfloat) * 2, texcoords);
	m_tangentBO.setData(Plane_VERTS * sizeof(GLfloat) * 3, NULL);
	
	m_VAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);
	m_VAO.bindAttribToBuffer(SHADER_NORMAL, GL_FLOAT, sizeof(GLfloat) * 3, m_normalBO);
	m_VAO.bindAttribToBuffer(SHADER_AMB, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_DIFF, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_SPEC, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_TC, GL_FLOAT, sizeof(GLfloat) * 2, m_texcooBO);
	m_VAO.bindAttribToBuffer(SHADER_TANGENT, GL_FLOAT, sizeof(GLfloat) * 3
 		, m_tangentBO);

	m_silhouetteVAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);

}

PlaneMesh::~PlaneMesh()
{
}


int PlaneMesh::inflate()
{
	calculateTangentBasis();

	for (int i = 0; i < Plane_VERTS; i++)
	{

		cout << i << " tangent: (" << tangents[i][0] << ", " << tangents[i][1]
			<< ", " << tangents[i][2] << ", " << tangents[i][3] << ")" << endl;
	}

	m_tangentBO.setData(Plane_VERTS * sizeof(GLfloat) * 3, tangents);
	m_VAO.bindAttribToBuffer(SHADER_TANGENT, GL_FLOAT, sizeof(GLfloat) * 3
 		, m_tangentBO);
	return 1;
}


void PlaneMesh::calculateTangentBasis()
{
	glm::vec3 *tempTangent = new glm::vec3[Plane_VERTS]();
	glm::vec3 *tempBinormal = new glm::vec3[Plane_VERTS]();

 	for (int i = 0; i < Plane_TRIS; i++)
 	{
 		const GLuint *triangle = triangles[i];
 		const md3_vertex &v0 = verts[triangle[0]];
 		const md3_vertex &v1 = verts[triangle[1]];
 		const md3_vertex &v2 = verts[triangle[2]];

 		glm::vec3 q1, q2;
 		q1.x = v1.coord[0] - v0.coord[0];
 		q1.y = v1.coord[1] - v0.coord[1];
 		q1.z = v1.coord[2] - v0.coord[2];
 		q2.x = v2.coord[0] - v0.coord[0];
 		q2.y = v2.coord[1] - v0.coord[1];
 		q2.z = v2.coord[2] - v0.coord[2];

 		float s1 = texcoords[triangle[1]][0] - texcoords[triangle[0]][0];
 		float s2 = texcoords[triangle[2]][0] - texcoords[triangle[0]][0];
 		float t1 = texcoords[triangle[1]][1] - texcoords[triangle[0]][1];
 		float t2 = texcoords[triangle[2]][1] - texcoords[triangle[0]][1];


 		glm::vec3 tangent = t2 * q1 - t1 * q2;
 		tangent = glm::normalize(tangent);
 		glm::vec3 binormal = -s2 * q1 + s1 * q2;
 		binormal = glm::normalize(binormal);

 		for (int j = 0; j < 3; j++)
 		{
 			tempTangent[triangle[j]] += tangent;
 			tempBinormal[triangle[j]] += binormal;
 		}
 	}

 	for (int i = 0; i < Plane_VERTS; i++)
 	{
 		glm::vec3 t = tempTangent[i];

 		glm::vec3 normal = glm::vec3(normals[i][0], normals[i][1], normals[i][2]);
 		t -= normal * glm::dot(t, normal);
 		t = glm::normalize(t);

 		tangents[i][0] = t.x;
 		tangents[i][1] = t.y;
 		tangents[i][2] = t.z;

 		if (glm::dot(glm::cross(normal, t), tempBinormal[i]) < 1.0)
 		{
 			tangents[i][3] = -1.0f;
 		} else
 		{
 			tangents[i][3] = 1.0f;
 		}
 	}
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