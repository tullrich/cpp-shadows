#include "shadows-common.h" /* pch */

 TexturedMesh::TexturedMesh(string uvmap_path, string bumpmap_path)
 {
 	/* load the texture if it hasnt been */
 	TextureManager &tm = *TextureManager::getInstance();
 	m_diffmap = tm.registerTexture(uvmap_path);
 	m_bumpmap = tm.registerTexture(bumpmap_path);

 	std::cout << "ERR1 " << glGetError() << std::endl;
 	m_diffTO.uploadImageCubeMap(*m_diffmap);
 	m_bumpTO.uploadImageCubeMap(*m_bumpmap);
 	std::cout << "ERR2 " << glGetError() << std::endl;
 }

 TexturedMesh::~TexturedMesh()
 {

 }

 int TexturedMesh::inflate()
 {
 	if (!m_isInflated)
 	{
 		calculateTangentBasis();
 		CubeMesh::inflate();
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
 	}

 }

 void TexturedMesh::render()
 {
 	m_diffTO.bind();
 	CubeMesh::render();
 	m_diffTO.unbind();
 }