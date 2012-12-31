#ifndef __MODELUTIL_H
#define __MODELUTIL_H

#include <GL/gl.h>
#include "vector.h"

class Triangle {
	public: 
		Vector v0; 
		Vector v1; 
		Vector v2; 

		Vector normal[3];
		Vector face_normal;
		
		int material_indices[3];
		char Color[3];
};

class Material {
	public: 
		Vector ambient;
		Vector diffuse;
		Vector specular;

		GLfloat shine;
};

class Model {
	public: 
		char*    pathname; 

		char*    name;

		int numMaterials;
		Material* materials;

		int numTriangles;
		Triangle* triangles;

		Vector max;
		Vector min;
};


/*  
 */
Model readModel(const char* fileName);
void drawModel(Model *model, bool colorOverride, float color[3]);
#endif