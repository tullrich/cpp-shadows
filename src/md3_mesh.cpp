#include "md3_mesh.h"

MD3Mesh::MD3Mesh()
{
	m_bOs = NULL;
	m_vAOs = NULL;
	m_isInflated = false;
}

MD3Mesh::~MD3Mesh()
{
	if (m_bOs != NULL)
		delete[] m_bOs;

	if (m_vAOs != NULL)
		delete[] m_vAOs;
}

int MD3Mesh::getNumSurfaces() {
	return header->num_surfaces;
}

int MD3Mesh::inflate()
{
	if (m_isInflated)
		return 1;

	int numSurfs = getNumSurfaces();

	m_bOs = new BufferObject[numSurfs];
	m_colorBOs = new BufferObject[numSurfs];
	m_vAOs = new VirtualArrayObject[numSurfs];
	m_edgesList = new EdgeMap[numSurfs];
	surfNormKs = new Vec4List[numSurfs];


	for (int i = 0; i < numSurfs; i++)
	{
		MD3Surface *surf = &surfaces[i];
		GLfloat colors[surf->getNumVertices()][3];
		for (int j = 0; j < surf->getNumVertices(); j+=3)
		{
			colors[j][0] = 1.0;
			colors[j][1] = 0.0;
			colors[j][2] = 0.0;

			colors[j+1][0] = 0.0;
			colors[j+1][1] = 1.0;
			colors[j+1][2] = 0.0;

			colors[j+2][0] = 0.0;
			colors[j+2][1] = 0.0;
			colors[j+2][2] = 1.0;
		}

		m_colorBOs[i].setData(surf->getNumVertices() * sizeof(GLfloat) * 3, 
			colors);
		m_bOs[i].setData(sizeof(md3_vertex) * surf->getNumVertices(), surf->xyznormals);

		m_vAOs[i].bindAttribToBuffer(SHADER_POS, GL_SHORT, sizeof(md3_vertex), m_bOs[i]);
		m_vAOs[i].bindAttribToBuffer(SHADER_AMB, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBOs[i]);
		m_vAOs[i].bindAttribToBuffer(SHADER_DIFF, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBOs[i]);
		m_vAOs[i].bindAttribToBuffer(SHADER_SPEC, GL_FLOAT, sizeof(GLfloat) * 3, m_colorBOs[i]);
	}


	calculateEdgeList();
	calculateNormalKs();


	m_isInflated = true;

/*	m_textures = new tImage*[numSurfs];
	for (int i = 0; i < numSurfs; i++)
	{
		m_textures[i] = surfaces[i].inflateFirstTexture();
	}*/

	return 1; /* success */
}

void MD3Mesh::calculateEdgeList()
{
	for (int s = 0; s < getNumSurfaces(); s++)
	{
		edgesForSurface(&surfaces[s], surfNormKs[s], &m_edgesList[s]);
	}	

}

void MD3Mesh::calculateNormalKs()
{
	for (int s = 0; s < getNumSurfaces(); s++)
	{
		surfNormKs[s] = new glm::vec4[surfaces[s].getNumTriangles()];
		normalKsForSurface(&surfaces[s], surfNormKs[s]);
	}	
}

void MD3Mesh::edgesForSurface(MD3Surface *surf, Vec4List normList, EdgeMap *edges)
{
	EdgeRecord cEdge;
	int vertIndex1, vertIndex2, vertIndex3;

	for(int i = 0; i < surf->getNumTriangles(); i++)
	{
		vertIndex1 = surf->triangles[i].indexes[0];
		vertIndex2 = surf->triangles[i].indexes[1];
		vertIndex3 = surf->triangles[i].indexes[2];
		//cout << " num tri " <<  i << " ind1 " << vertIndex1 << " ind2 "<<  vertIndex2 << " ind3 " << vertIndex3 << endl;
		

		if (vertIndex1 < vertIndex2)
		{
			cEdge.vertIndex1 = vertIndex1;
			cEdge.vertIndex2 = vertIndex2;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			(*edges)[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex2 < vertIndex3)
		{
			cEdge.vertIndex1 = vertIndex2;
			cEdge.vertIndex2 = vertIndex3;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			(*edges)[cEdge.getRecord()] = cEdge;
		}
		if(vertIndex3 < vertIndex1)
		{
			cEdge.vertIndex1 = vertIndex3;
			cEdge.vertIndex2 = vertIndex1;
			cEdge.tri1 = i;
			cEdge.tri2 = -1;
			(*edges)[cEdge.getRecord()] = cEdge;
		}
	}

	for(int i = 0; i < surf->getNumTriangles(); i++)
	{
		vertIndex1 = surf->triangles[i].indexes[0];
		vertIndex2 = surf->triangles[i].indexes[1];
		vertIndex3 = surf->triangles[i].indexes[2];

		//cout << "vert1:" << vertIndex1 << " vert2:" << vertIndex2 << " vert3:" << vertIndex3 << endl;

		if (vertIndex1 > vertIndex2)
		{
			cEdge.vertIndex1 = vertIndex2;
			cEdge.vertIndex2 = vertIndex1;
			//cout << cEdge.getRecord() << endl;

			if (!((*edges).find(cEdge.getRecord()) == (*edges).end()))
			{
				cEdge = (*edges)[cEdge.getRecord()];

				cEdge.tri2 = i;
				(*edges)[cEdge.getRecord()] = cEdge;
			}  else {
				cout << vertIndex1 << " > " << vertIndex2 << endl;
			}
		}
		if(vertIndex2 > vertIndex3)
		{
			cEdge.vertIndex1 = vertIndex3;
			cEdge.vertIndex2 = vertIndex2;
			//cout << cEdge.getRecord() << endl;
			if (!((*edges).find(cEdge.getRecord()) == (*edges).end()))
			{
				cEdge = (*edges)[cEdge.getRecord()];
				cEdge.tri2 = i;
				(*edges)[cEdge.getRecord()] = cEdge;
			} else {
				cout << vertIndex2 << " > " << vertIndex3 << endl;
			}
		}
		if(vertIndex3 > vertIndex1)
		{
			cEdge.vertIndex1 = vertIndex1;
			cEdge.vertIndex2 = vertIndex3;
			//cout << cEdge.getRecord() << endl;
			if (!((*edges).find(cEdge.getRecord()) == (*edges).end()))
			{
				cEdge = (*edges)[cEdge.getRecord()];
				cEdge.tri2 = i;
				(*edges)[cEdge.getRecord()] = cEdge;
			} else {
				cout << vertIndex3 << " > " << vertIndex1 << endl;
			}
		}
	}

	for(auto iter = (*edges).begin(); iter != (*edges).end(); iter++)
	{

		if ( (*iter).second.tri2 == -1)
		{
			(*edges).erase(iter);
			cout << (*iter).second.tri1 << " " 
				<< (*iter).second.tri2 << endl;
		}

	}
	cout << "result" << endl;

}

void MD3Mesh::normalKsForSurface(MD3Surface *surf, Vec4List normList)
{
	int vertIndex1, vertIndex2, vertIndex3;
	glm::vec3 N;

	for(int i = 0; i < surf->getNumTriangles(); i++)
	{
		vertIndex1 = surf->triangles[i].indexes[0];
		vertIndex2 = surf->triangles[i].indexes[1];
		vertIndex3 = surf->triangles[i].indexes[2];

		glm::vec3 V1 = glm::vec3(
			surf->xyznormals[vertIndex1].coord[0],
			surf->xyznormals[vertIndex1].coord[1],
			surf->xyznormals[vertIndex1].coord[2]);
		glm::vec3 V2 = glm::vec3(
			surf->xyznormals[vertIndex2].coord[0],
			surf->xyznormals[vertIndex2].coord[1],
			surf->xyznormals[vertIndex2].coord[2]);
		glm::vec3 V3 = glm::vec3(
			surf->xyznormals[vertIndex3].coord[0],
			surf->xyznormals[vertIndex3].coord[1],
			surf->xyznormals[vertIndex3].coord[2]);
		
		N = glm::normalize( glm::cross((V2 - V1), (V3 - V1)));
/*		normals[i][0] = N.x;
		normals[i][1] = N.y;
		normals[i][2] = N.z;*/
		//cout << N.x << " " << N.y << " " << N.z << " "  << endl;


		normList[i] = glm::vec4(N, glm::dot(-N, V1));
	}
}

void MD3Mesh::render()
{
	int numSurfs = getNumSurfaces();

	for (int i = 0; i < numSurfs; i++)
	{
		m_vAOs[i].bind();
		glDrawElements(GL_TRIANGLES, 3 * surfaces[i].getNumTriangles(), 
		GL_UNSIGNED_INT, surfaces[i].triangles);
	}
}

void MD3Mesh::onDrawSilhouette(glm::vec4 light_pos)
{
	for (int s = 0; s < getNumSurfaces(); s++)
	{
		m_vAOs[s].bind();
		MD3Surface *surf = &surfaces[s];
		Vec4List normList =  surfNormKs[s];
		EdgeMap *edges = &m_edgesList[s];
	/*	for(int i = 0; i < surf->getNumTriangles(); i++)
		{

			if (glm::dot(normList[i], light_pos) > 0)
			{
				glDrawElements(GL_TRIANGLES, 3, 
					GL_UNSIGNED_INT,  &surf->triangles[i]);
			}
		}*/

		for(auto iter = (*edges).begin(); iter != (*edges).end(); iter++)
		{
			int tri1 = (*iter).second.tri1;
			bool tri1_facingLight =  glm::dot(normList[tri1], light_pos) > 0;
			int tri2 = (*iter).second.tri2;
			bool tri2_facingLight =  glm::dot(normList[tri2], light_pos) > 0;

/*			if ( tri1_facingLight && !tri2_facingLight)
			{
				glDrawElements(GL_TRIANGLES, 3, 
					GL_UNSIGNED_INT,  &surf->triangles[tri1]);
			} else if(!tri1_facingLight && tri2_facingLight)
			{
				glDrawElements(GL_TRIANGLES, 3, 
					GL_UNSIGNED_INT,  &surf->triangles[tri2]);
			}*/

			md3_vertex A = surf->xyznormals[(*iter).second.vertIndex1];
			md3_vertex B = surf->xyznormals[(*iter).second.vertIndex2];

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
}


void MD3Mesh::onDrawShadowVolumes(glm::vec4 light_pos)
{
	if (light_pos.w == 1)
	{
		for (int s = 0; s < getNumSurfaces(); s++)
		{
			m_vAOs[s].bind();
			renderPointLightQuads(light_pos, &surfaces[s], &m_edgesList[s], surfNormKs[s]);
		}
	} else 
	{
		//renderInfiniteLightTris(light_pos);
	}
}

void MD3Mesh::renderPointLightQuads(glm::vec4 light_pos, MD3Surface *surf, EdgeMap *edges, Vec4List normList)
{
	for(auto iter = (*edges).begin(); iter != (*edges).end(); iter++)
	{
		int tri1 = (*iter).second.tri1;
		bool tri1_facingLight =  glm::dot(normList[tri1], light_pos) > 0;
		int tri2 = (*iter).second.tri2;
		bool tri2_facingLight =  glm::dot(normList[tri2], light_pos) > 0;

		//cout << " edge of tri1 " << tri1 << " and tri2 " << tri2 << " " << tri1_facingLight
		//	<< " " << tri2_facingLight << endl;

		md3_vertex A = surf->xyznormals[(*iter).second.vertIndex1];
		md3_vertex B = surf->xyznormals[(*iter).second.vertIndex2];

		if ( tri1_facingLight && !tri2_facingLight)
		{
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

	for (int iter = 0; iter != surf->getNumTriangles(); iter++)
	{
		bool facing_light =  glm::dot(normList[iter], light_pos) > 0;

		if(facing_light)
		{
			md3_vertex A = surf->xyznormals[surf->triangles[iter].indexes[0]];
			md3_vertex B = surf->xyznormals[surf->triangles[iter].indexes[1]];
			md3_vertex C = surf->xyznormals[surf->triangles[iter].indexes[2]];

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

void MD3Mesh::renderInfiniteLightTris(glm::vec4 light_pos, MD3Surface *surf, EdgeMap *edges, Vec4List normList)
{

}