#include "shadows-common.h" /* pch */

 TexturedMesh::TexturedMesh(string uvmap_path, string bumpmap_path, string specmap_path)
 {
 	/* load the texture if it hasnt been */
 	TextureManager &tm = *TextureManager::getInstance();
 	m_diffmap = tm.registerTexture(uvmap_path);
 	m_bumpmap = tm.registerTexture(bumpmap_path);
 	m_specmap = tm.registerTexture(specmap_path);

 	std::cout << "ERR1 " << glGetError() << std::endl;
 	m_diffTO.uploadImage2D(0, *m_diffmap);
 	m_bumpTO.uploadImage2D(1, *m_bumpmap);
 	m_specTO.uploadImage2D(2, *m_specmap);
 	std::cout << "ERR2 " << glGetError() << std::endl;

 	m_VAO.bindAttribToBuffer(SHADER_TANGENT, GL_FLOAT, sizeof(GLfloat) * 3
 		, m_tangentBO);
 }

 TexturedMesh::~TexturedMesh()
 {

 }

 int TexturedMesh::inflate()
 {
 	if (!m_isInflated)
 	{
 		CubeMesh::inflate();
 		calculateTangentBasis();

 		for (int i = 0; i < NUM_VERTS; i++)
 		{
 			cout << "tangent: (" << tangents[i][0] << ", " << tangents[i][1]
 				<< ", " << tangents[i][2] << ", " << tangents[i][3] << ")" << endl;
 		}

 		m_tangentBO.setData(NUM_VERTS * sizeof(GLfloat) * 3, tangents);
 	}

 	return 1;
 }

 void TexturedMesh::calculateTangentBasis()
 {
	glm::vec3 *tempTangent = new glm::vec3[NUM_VERTS]();
	glm::vec3 *tempBinormal = new glm::vec3[NUM_VERTS]();

 	for (int i = 0; i < NUM_TRIS; i++)
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

 	for (int i = 0; i < NUM_VERTS; i++)
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

 void TexturedMesh::render()
 {
 	m_diffTO.bind();
 	m_bumpTO.bind();
 	m_specTO.bind();
 	CubeMesh::render();
 	//m_diffTO.unbind();
 }