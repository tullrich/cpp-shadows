#ifndef _SHADOWS_COMMON_H
#define _SHADOWS_COMMON_H

#include "GL/glew.h"

#include <string>
using std::string;
#include <iostream>

#include <iostream>
using std::endl;
using std::cout;

#include "matrix.h"
#include "vector.h"
#include "modelutil.h"
#include "camera.h"


extern int SHADER_POS;
extern int SHADER_COLOR;
extern int SHADER_NORMAL;
extern int SHADER_TC;
extern int SHADER_AMB;
extern int SHADER_DIFF;
extern int SHADER_SPEC;
extern int SHADER_SHINE;

#include <sstream>
using std::stringstream;
#include <unordered_map>
#include <utility>
using std::pair;

enum OP
{
	GREATER_THAN,
	LESS_THAN
};

typedef struct
{
	int vertIndex1;
	int vertIndex2;
	int	tri1;
	int	tri2;

	string getRecord() {
		stringstream iss;
		iss << vertIndex1 << ":" << vertIndex2;
		return iss.str();
	}

} EdgeRecord;


typedef std::unordered_map<string, EdgeRecord> EdgeMap;


#include "texture_structs.h"

#include "VirtualArrayObject.h"
#include "BufferObject.h"
#include "TextureObject.h"

#include "Renderable.h"
#include "Mesh.h"
#include "MeshRenderable.h"
#include "Shader.h"

#include "TextureManager.h"
#include "TexturedMesh.h"


#define SHADOW_LOG_ERROR(x) cout << "ERROR: " << x << endl;

#endif