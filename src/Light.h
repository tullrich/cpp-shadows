#ifndef _LIGHT_H
#define _LIGHT_H

#include "shadows-common.h"
#include <glm/glm.hpp>


class Light : public MeshRenderable
{
public:
	Light(int x, int y, int z, bool isPoint);
	~Light();

	glm::vec4 getPosition();
	void setPosition(glm::vec4 new_pos);
	void onDrawShadowVolumes(glm::vec4 light_pos);
	void activate();
	void deactivate();
	void setType(bool isPoint);
private:
	glm::vec4 position;
};

#endif /* _LIGHT_H */