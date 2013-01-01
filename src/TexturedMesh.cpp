#include "shadows-common.h" /* pch */

 TexturedMesh::TexturedMesh(string uvmap_path)
 {
 	/* load the texture if it hasnt been */
 	TextureManager &tm = *TextureManager::getInstance();
 	m_diffmap = tm.registerTexture(uvmap_path);

 	std::cout << "ERR1 " << glGetError() << std::endl;
 	m_diffTO.uploadImage3D(*m_diffmap);
 	std::cout << "ERR2 " << glGetError() << std::endl;
 }

 TexturedMesh::~TexturedMesh()
 {

 }

 void TexturedMesh::render()
 {
 	m_diffTO.bind();
 	CubeMesh::render();
 }