#ifndef _MD_STRUCTS_H
#define _MD_STRUCTS_H


/* POD structure matching the first input from an md3 file */
typedef struct {
	int ident;
	int version;
	unsigned char name[64];
	int flags;
	int num_frames;
	int num_tags;
	int num_surfaces;
	int num_skins;
	int ofs_frames;
	int ofs_tags;
	int ofs_surfaces;
	int ofs_eof;

} md3_header;

typedef struct {
	float x;
	float y;
	float z;
} vec3;

typedef struct {
	vec3 min_bounds;
	vec3 max_bounds;
	vec3 local_origin;
	float radius;
	char name[16];
} md3_frame;

typedef struct {
	char name[64];
	vec3 origin;
	vec3 axis[3];
} md3_tag;

typedef struct {
	int ident;
	char name[64];
	int flags;
	int num_frames;
	int num_shaders;
	int num_verts;
	int num_triangles;
	
	int ofs_triangles;
	int ofs_shaders;
	int ofs_st;
	int ofs_xyznormal;
	int ofs_end;
} md3_surface;

typedef struct {
	char name[64];
	int shader_index;
} md3_shader;

typedef struct {
	int indexes[3];
} md3_triangle;

typedef struct {
	float st[2];
} md3_texcoord;

typedef struct {
	short coord[3];
	char normal[2];
} md3_vertex;

void printHeader(md3_header* h);
void printFrame(md3_frame* f);
void printTag(md3_tag* t);
void printSurfheader(md3_surface* t);
void printShader(md3_shader* shader);
void printVertex(md3_vertex* v);
void printTriangle(md3_triangle* t);
void printTexCoord(md3_texcoord* st);
void printFrames(int num_frames, md3_frame* f);
void printTags(int num_tags, md3_tag* t);
void printShaders(int num_shaders, md3_shader* shaders);
void printVertices(int num_verts, md3_vertex* vertices);
void printTriangles(int num_triangles, md3_triangle* triangles);
void printTexCoords(int num_verts, md3_texcoord* sts);
#endif
