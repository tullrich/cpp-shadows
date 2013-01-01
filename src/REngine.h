#ifndef _R_ENGINE_H
#define _R_ENGINE_H

#include "shadows-common.h"

#include <set>

#include "Light.h"
#include "GlCanvas.h"
#include "md3_reader.h"
#include "CubeMesh.h"

class REngine : public GlCanvas
{
	typedef std::set<Renderable*> Renderables;

public:
	REngine(int X, int Y, int W, int H, const char *L);
	~REngine();

	void onDraw();
	void init();

	void addMd3FromFile(const char* filepath);
	void addRenderEntity(Renderable& r);
	void mouseDown(int x, int y);
	void mouseDrag(int x, int y);

	/* controls */
	void setLightPosition(int x, int y, int z);
	void setRenderScene(bool doRender);
	void setRenderShadowVolumes(bool doRender);
	void setRenderSilhouttes(bool doRender);
	void setVolumesCullFront(bool doCull);
	void setVolumesCullBack(bool doCull);
	void setLightType(bool isPoint);
	void setBlockPosition(int x, int y, int z, float scale);
	void spawnBlockAt(int x, int y, int z, float scale);
	void setFoV(double fov, double znear, double zfar);
private:
	void drawPass();
	void silhouttePass();
	void shadowPass();
	void checkError();
	void depthPass();
	ShaderProgram renderProgram, extrusionProgram;

	Camera c;
	Renderables entities;

	Light *light1;
	CubeMesh *cube;
	MeshRenderable *block1;

	int lastx;
	int lasty;

	double m_fov, m_znear, m_zfar;

	bool m_RenderScene, m_RenderShadowVolumes, m_RenderSilhouttes;
	bool m_VolumesCullFront, m_VolumesCullBack;
};

#endif		