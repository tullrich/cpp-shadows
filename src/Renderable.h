#ifndef _RENDERABLE_H
#define _RENDERABLE_H
#include "glm/glm.hpp"

class ShaderProgram;

class Renderable
{
public:
	Renderable() {};
	virtual ~Renderable() {};
	virtual void onDraw() = 0;
	virtual void onDrawSilhoutte(glm::vec4 light_pos) = 0;
	virtual void onDrawShadowVolumes(glm::vec4 light1_pos, ShaderProgram &s) = 0;
};

#endif