#ifndef _MD3_MESH_H
#define _MD3_MESH_H
#include "md3_structs.h"
#include "shadows-common.h"
#include "md3_surface.h"

class MD3Mesh : public Mesh
{
private:
	typedef glm::vec4* Vec4List;

public:
	md3_header *header;
	md3_frame *frames;
	md3_tag *tags;
	MD3Surface *surfaces;

	/* ctor */
	MD3Mesh();
	~MD3Mesh();

	int getNumSurfaces();
	int inflate();
	void render();
	void onDrawSilhouette(glm::vec4 light_pos);
	void onDrawShadowVolumes(glm::vec4 light_pos);
private:
	void calculateEdgeList();
	void calculateNormalKs();
	void edgesForSurface(MD3Surface *surf, Vec4List normList, EdgeMap *edges);
	void normalKsForSurface(MD3Surface *surf, Vec4List normList);

	void renderPointLightQuads(glm::vec4 light_pos, MD3Surface *surf, EdgeMap *edges, Vec4List normList);
	void renderInfiniteLightTris(glm::vec4 light_pos, MD3Surface *surf, EdgeMap *edges, Vec4List normList);


	bool m_isInflated;
	BufferObject *m_bOs, *m_colorBOs;
	VirtualArrayObject *m_vAOs, *m_silhouetteVAO;
	tImage** m_textures;


	EdgeMap *m_edgesList;
	Vec4List* surfNormKs;
};

void printMesh(MD3Mesh *mesh);

#endif
