#include "CubeMesh.h"


md3_vertex CubeMesh::verts[] =  
	{
		{200, 200, 200},
		{-200, 200, 200},
		{200, 200, -200},
		{-200, 200, -200},

		{200, -200, 200},
		{-200, -200, 200},
		{200, -200, -200},
		{-200, -200, -200}
	};



GLuint CubeMesh::triangles[][3] = 
{
	{2, 1, 0},
	{1, 2, 3},

	{0, 6, 2},
	{6, 0, 4},

	{5, 0, 1},
	{0, 5, 4},

	{7, 1, 3},
	{1, 7, 5},

	{2, 7, 3},
	{7, 2, 6},

	{6, 5, 7},
	{5, 6, 4}
};

GLfloat CubeMesh::normals[NUM_TRIS][3];
glm::vec4 CubeMesh::normalKs[NUM_TRIS];

CubeMesh::CubeMesh()
{
	colors[0][0] = 1.0;
	colors[0][1] = 0.0;
	colors[0][2] = 0.0;
	colors[1][0] = 0.0;
	colors[1][1] = 1.0;
	colors[1][2] = 0.0;
	colors[2][0] = 0.0;
	colors[2][1] = 0.0;
	colors[2][2] = 1.0;
	colors[3][0] = 1.0;
	colors[3][1] = 1.0;
	colors[3][2] = 1.0;
	colors[4][0] = 1.0;
	colors[4][1] = 1.0;
	colors[4][2] = 0.0;
	colors[5][0] = 1.0;
	colors[5][1] = 1.0;
	colors[5][2] = 0.0;
	colors[6][0] = 1.0;
	colors[6][1] = 1.0;
	colors[6][2] = 0.0;
	colors[7][0] = 1.0;
	colors[7][1] = 1.0;
	colors[7][2] = 0.0;
	
	
	m_BO.setData(NUM_VERTS * sizeof(md3_vertex), verts);
	m_normalBO.setData(NUM_TRIS * sizeof(GLfloat) * 3, NULL);
	m_colorBO.setData(NUM_VERTS * sizeof(GLfloat) * 3, colors);


	
	m_VAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);
	m_VAO.bindAttribToBuffer(SHADER_NORMAL, GL_FLOAT, sizeof(GLfloat) * 3, m_normalBO);
	m_VAO.bindAttribToBuffer(SHADER_AMB, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_DIFF, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	m_VAO.bindAttribToBuffer(SHADER_SPEC, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBO);
	
	m_silhouetteVAO.bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_BO);

}

int CubeMesh::inflate()
{
	if(!m_isInflated)
	{
		calculateEdgeList();
		calculateNormalKs();


		m_normalBO.setData(NUM_TRIS * sizeof(GLfloat) * 3, normals);

		m_isInflated = true;
	}

	return 1;
}

void CubeMesh::calculateEdgeList()
{
	EdgeRecord cEdge;
	int vertIndex1, vertIndex2, vertIndex3;

	for(int i = 0; i < NUM_TRIS; i++)
	{
		vertIndex1 = triangles[i][0];
		vertIndex2 = triangles[i][1];
		vertIndex3 = triangles[i][2];

		//cout << "vert1:" << vertIndex1 << " vert2:" << vertIndex2 << " vert3:" << vertIndex3 << endl;

		if (vertIndex1 < vertIndex2)
		{
			cEdge.vertIndex1 = vertIndex1;
			cEdge.vertIndex2 = vertIndex2;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			//cout << cEdge.getRecord() << endl;
			m_edges[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex2 < vertIndex3)
		{
			cEdge.vertIndex1 = vertIndex2;
			cEdge.vertIndex2 = vertIndex3;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			//cout << cEdge.getRecord() << endl;
			m_edges[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex3 < vertIndex1)
		{
			cEdge.vertIndex1 = vertIndex3;
			cEdge.vertIndex2 = vertIndex1;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			//cout << cEdge.getRecord() << endl;
			m_edges[cEdge.getRecord()] = cEdge;
		}
	}


	for(int i = 0; i < NUM_TRIS; i++)
	{
		vertIndex1 = triangles[i][0];
		vertIndex2 = triangles[i][1];
		vertIndex3 = triangles[i][2];

		//cout << "vert1:" << vertIndex1 << " vert2:" << vertIndex2 << " vert3:" << vertIndex3 << endl;

		if (vertIndex1 > vertIndex2)
		{
			cEdge.vertIndex1 = vertIndex2;
			cEdge.vertIndex2 = vertIndex1;
			//cout << cEdge.getRecord() << endl;
			cEdge = m_edges[cEdge.getRecord()];
			cEdge.tri2 = i;
			m_edges[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex2 > vertIndex3)
		{
			cEdge.vertIndex1 = vertIndex3;
			cEdge.vertIndex2 = vertIndex2;
			//cout << cEdge.getRecord() << endl;
			cEdge = m_edges[cEdge.getRecord()];
			cEdge.tri2 = i;
			m_edges[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex3 > vertIndex1)
		{
			cEdge.vertIndex1 = vertIndex1;
			cEdge.vertIndex2 = vertIndex3;
			//cout << cEdge.getRecord() << endl;
			cEdge = m_edges[cEdge.getRecord()];
			cEdge.tri2 = i;
			m_edges[cEdge.getRecord()] = cEdge;
		}
	}
/*
	cout << "result" << endl;
	for(auto iter = m_edges.begin(); iter != m_edges.end(); iter++)
	{
		cout << (*iter).first << " :: " 
			<< (*iter).second.tri1 << " " 
			<< (*iter).second.tri2 << endl;
	}*/
}

void CubeMesh::calculateNormalKs()
{
	int vertIndex1, vertIndex2, vertIndex3;
	glm::vec3 N;

	for(int i = 0; i < NUM_TRIS; i++)
	{
		vertIndex1 = triangles[i][0];
		vertIndex2 = triangles[i][1];
		vertIndex3 = triangles[i][2];

		glm::vec3 V1 = glm::vec3(
			verts[vertIndex1].coord[0],
			verts[vertIndex1].coord[1],
			verts[vertIndex1].coord[2]);
		glm::vec3 V2 = glm::vec3(
			verts[vertIndex2].coord[0],
			verts[vertIndex2].coord[1],
			verts[vertIndex2].coord[2]);
		glm::vec3 V3 = glm::vec3(
			verts[vertIndex3].coord[0],
			verts[vertIndex3].coord[1],
			verts[vertIndex3].coord[2]);
		
		N = glm::normalize( glm::cross((V2 - V1), (V3 - V1)));
		normals[i][0] = N.x;
		normals[i][1] = N.y;
		normals[i][2] = N.z;
		//cout << N.x << " " << N.y << " " << N.z << " "  << endl;


		normalKs[i] = glm::vec4(N, glm::dot(-N, V1));
	}
}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::render()
{
	m_VAO.bind();
	//glEnableVertexAttribArray(SHADER_AMB);
	//glVertexAttrib4f(SHADER_AMB, 0.3, 0.3, 0.3, 1.0);
	//glVertexAttrib4f(SHADER_DIFF, 1.0, 1.0, 0.0, 1.0);
	//glVertexAttrib4f(SHADER_SPEC, 1.0, 1.0, 1.0, 1.0);
	glVertexAttrib1f(SHADER_SHINE, 1.0);

	glDrawElements(GL_TRIANGLES, NUM_TRIS * 3, 
		GL_UNSIGNED_INT, triangles);
}

void CubeMesh::onDrawSilhouette(glm::vec4 light_pos)
{
	m_silhouetteVAO.bind();
	glVertexAttrib4f(SHADER_DIFF, 0.0, 1.0, 0.0, 1.0);

	for(auto iter = m_edges.begin(); iter != m_edges.end(); iter++)
	{

		int tri1 = (*iter).second.tri1;
		bool tri1_facingLight =  glm::dot(normalKs[tri1], light_pos) > 0;
		int tri2 = (*iter).second.tri2;
		bool tri2_facingLight =  glm::dot(normalKs[tri2], light_pos) > 0;

		md3_vertex A = verts[(*iter).second.vertIndex1];
		md3_vertex B = verts[(*iter).second.vertIndex2];

		if ( tri1_facingLight && !tri2_facingLight)
		{
			glBegin(GL_LINES);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glEnd();
		} else if(!tri1_facingLight && tri2_facingLight)
		{
			glBegin(GL_LINES);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glEnd();
		}
	}
}

void CubeMesh::onDrawShadowVolumes(glm::vec4 light_pos)
{
	m_silhouetteVAO.bind();
	//glVertexAttrib3f(SHADER_COLOR, 0.0, 1.0, 0.0);

	/*for(int i = 0; i < NUM_TRIS; i++)
	{
		if (glm::dot(normalKs[i], light_pos) > 0)
		{
			glDrawElements(GL_TRIANGLES, 3, 
				GL_UNSIGNED_INT, &triangles[i]);
		}
	}*/

	/* */

	if (light_pos.w == 1)
	{
		renderPointLightQuads(light_pos);
	} else 
	{
		renderInfiniteLightTris(light_pos);
	}

}

void CubeMesh::renderPointLightQuads(glm::vec4 light_pos)
{
	for(auto iter = m_edges.begin(); iter != m_edges.end(); iter++)
	{
		int tri1 = (*iter).second.tri1;
		bool tri1_facingLight =  glm::dot(normalKs[tri1], light_pos) > 0;
		int tri2 = (*iter).second.tri2;
		bool tri2_facingLight =  glm::dot(normalKs[tri2], light_pos) > 0;

		//cout << " edge of tri1 " << tri1 << " and tri2 " << tri2 << " " << tri1_facingLight
		//	<< " " << tri2_facingLight << endl;

		md3_vertex A = verts[(*iter).second.vertIndex1];
		md3_vertex B = verts[(*iter).second.vertIndex2];

		if ( tri1_facingLight && !tri2_facingLight)
		{
			/*glm::vec4 V1 = glm::vec4(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glm::vec4 V2 = glm::vec4(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glm::vec4 V3 = glm::vec4(A.coord[0], A.coord[1], A.coord[2], 0.0);
			glm::vec4 V4 = glm::vec4(B.coord[0], B.coord[1], B.coord[2], 0.0);*/

			glBegin(GL_QUADS);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 0.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 0.0);
			glEnd();
		} else if(!tri1_facingLight && tri2_facingLight)
		{
			glBegin(GL_QUADS);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 0.0);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 0.0);
			glEnd();
		}

	}

	for (int iter = 0; iter != NUM_TRIS; iter++)
	{
		bool facing_light =  glm::dot(normalKs[iter], light_pos) > 0;

		if(facing_light)
		{
			md3_vertex A = verts[triangles[iter][0]];
			md3_vertex B = verts[triangles[iter][1]];
			md3_vertex C = verts[triangles[iter][2]];

			glBegin(GL_TRIANGLES);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 0.0);
			glVertex4f(C.coord[0], C.coord[1], C.coord[2], 0.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 0.0);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(C.coord[0], C.coord[1], C.coord[2], 1.0);
			glEnd();
		}
	}
}

void CubeMesh::renderInfiniteLightTris(glm::vec4 light_pos)
{
	for(auto iter = m_edges.begin(); iter != m_edges.end(); iter++)
	{
		int tri1 = (*iter).second.tri1;
		bool tri1_facingLight =  glm::dot(normalKs[tri1], light_pos) > 0;
		int tri2 = (*iter).second.tri2;
		bool tri2_facingLight =  glm::dot(normalKs[tri2], light_pos) > 0;

		//cout << "infinite edge of tri1 " << tri1 << " and tri2 " << tri2 << " " << tri1_facingLight
		//	<< " " << tri2_facingLight << endl;

		md3_vertex A = verts[(*iter).second.vertIndex1];
		md3_vertex B = verts[(*iter).second.vertIndex2];

		if ( tri1_facingLight && !tri2_facingLight)
		{
			/*glm::vec4 V1 = glm::vec4(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glm::vec4 V2 = glm::vec4(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glm::vec4 V3 = glm::vec4(A.coord[0], A.coord[1], A.coord[2], 0.0);
			glm::vec4 V4 = glm::vec4(B.coord[0], B.coord[1], B.coord[2], 0.0);*/

			glBegin(GL_TRIANGLES);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(0.0, 0.0, 0.0, 0.0);
			glEnd();
		} else if(!tri1_facingLight && tri2_facingLight)
		{
			glBegin(GL_TRIANGLES);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(0.0, 0.0, 0.0, 0.0);
			glEnd();
		}
	}

	for (int iter = 0; iter != NUM_TRIS; iter++)
	{
		bool facing_light =  glm::dot(normalKs[iter], light_pos) > 0;

		if(facing_light)
		{
			md3_vertex A = verts[triangles[iter][0]];
			md3_vertex B = verts[triangles[iter][1]];
			md3_vertex C = verts[triangles[iter][2]];

			glBegin(GL_TRIANGLES);
			glVertex4f(A.coord[0], A.coord[1], A.coord[2], 1.0);
			glVertex4f(B.coord[0], B.coord[1], B.coord[2], 1.0);
			glVertex4f(C.coord[0], C.coord[1], C.coord[2], 1.0);
			glEnd();
		}
	}
}