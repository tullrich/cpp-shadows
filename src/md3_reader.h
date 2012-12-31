#ifndef _MD3READER_H
#define _MD3READER_H
#include <fstream>
#include "md3_structs.h"
#include "md3_mesh.h"

class MD3Reader {
	public:

		bool open(const char* filepath);
		bool readMesh(MD3Mesh* mesh);
		bool readHeader(md3_header* header);
		bool readFrames(int num_frames, int ofs_frames, md3_frame* frames);
		bool readTags(int num_tags, int ofs_tags, md3_tag* tags);
		bool readSurfaces(int num_surfaces, int ofs_surfaces, MD3Surface* surfaces);
		bool readSurface(int ofs_surface, MD3Surface* surface);
		void close();

	private:
		std::fstream file_in;

		bool _checkError();

		bool _readSurface(int ofs_surface, MD3Surface* surface);
		
		bool _readSurfaceHeader(int ofs_surf_head, md3_surface *surf_head);
		bool _readShaders(int pos_shaders, int num_shaders, md3_shader *shaders);
		bool _readTriangles(int pos_triangles, int num_triangles, md3_triangle *triangles);
		bool _readTextCoords(int pos_sts, int num_verts, md3_texcoord *sts);
		bool _readXYZNormals(int pos_xyznormal, int num_verts, md3_vertex *vertices);
};

#endif
