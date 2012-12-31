#include <cstdio>
#include "md3_reader.h"

bool MD3Reader::_checkError() {
	return file_in.fail();
}

bool MD3Reader::open(const char* filepath) {

	printf("Opening file %s...", filepath);
	file_in.open(filepath, std::fstream::in);	

	if (_checkError()) {
		printf("\nERROR: could not open file\n");
		return false;
	}

	printf("done\n");
	return true;
}

bool MD3Reader::readMesh(MD3Mesh* mesh) {

	md3_header *header;
	md3_frame *frames;
	md3_tag *tags;
	MD3Surface *surfaces;

	// Read in each component, stop and return if there is an error at any step
	// in the case of an error, the model will be empty

	// Read in the header
	header = new md3_header;
	if (!readHeader(header)) {
		return false;
	}
	
	// Read in the frames
	frames = new md3_frame[header->num_frames];
	if(!readFrames(header->num_frames, header->ofs_frames, frames)) {
		return false;
	}

	// Read in the tags
	tags = new md3_tag[header->num_tags];
	if(!readTags(header->num_tags, header->ofs_tags, tags)) {
		return false;
	}

	// Read in the surfaces
	surfaces = new MD3Surface[header->num_surfaces];
	if(!readSurfaces(header->num_surfaces, header->ofs_surfaces, surfaces)) {
		return false;
	}

	// Quick check to make sure nothing went awry
	if (header->ofs_eof != file_in.tellg() and false) {
		printf("\nERROR: end of MD3 position does not match OFS_END from file %i \n", header->ofs_eof);
		return false;
	}

	// Assign these here in case there was an error above
	mesh->header = header;
	mesh->frames = frames;
	mesh->tags = tags;
	mesh->surfaces = surfaces;

	return true;
}

bool MD3Reader::_readSurfaceHeader(int ofs_surf_head, md3_surface *surf_head) {

	file_in.seekg(ofs_surf_head);
	file_in.read((char*)surf_head, sizeof(md3_surface));

	return !_checkError();
}

bool MD3Reader::_readShaders(int pos_shaders, int num_shaders, md3_shader *shaders) {

	file_in.seekg(pos_shaders);
	file_in.read((char*)shaders, sizeof(md3_shader) * num_shaders);

	return !_checkError();
}

bool MD3Reader::_readTriangles(int pos_triangles, int num_triangles, md3_triangle *triangles) {

	file_in.seekg(pos_triangles);
	file_in.read((char*)triangles, sizeof(md3_triangle) * num_triangles);

	return !_checkError();
}

bool MD3Reader::_readTextCoords(int pos_st, int num_verts, md3_texcoord *sts) {

	file_in.seekg(pos_st);
	file_in.read((char*)sts, sizeof(md3_texcoord) * num_verts);

	return !_checkError();
}

bool MD3Reader::_readXYZNormals(int pos_xyznormal, int num_verts, md3_vertex *vertices) {

	file_in.seekg(pos_xyznormal);
	file_in.read((char*)vertices, sizeof(md3_vertex) * num_verts);

	return !_checkError();
}

bool MD3Reader::_readSurface(int ofs_surface, MD3Surface* surface) {
	int ofs_current, pos_end;
	md3_surface *surf_header;
	md3_shader *shaders;
	md3_triangle *triangles;
	md3_texcoord *sts;
	md3_vertex *xyznormals;

	// Read in the surface header
	surf_header = new md3_surface;
	if (!_readSurfaceHeader(ofs_surface, surf_header)) {
		printf("\nERROR: could not read surface header");
		return false;
	}

	// Read in all the shaders
	ofs_current = ofs_surface + surf_header->ofs_shaders;
	shaders = new md3_shader[surf_header->num_shaders];
	if (!_readShaders(ofs_current, surf_header->num_shaders, shaders)) {
		printf("\nERROR: could not read shaders\n");
		return false;
	}

	// Read in all the triangles
	ofs_current = ofs_surface + surf_header->ofs_triangles;
	triangles = new md3_triangle[surf_header->num_triangles];
	if (!_readTriangles(ofs_current, surf_header->num_triangles, triangles)) {
		printf("\nERROR: could not read triangles\n");
		return false;
	}

	// Read in all the texture coordinates
	ofs_current = ofs_surface + surf_header->ofs_st;
	sts = new md3_texcoord[surf_header->num_verts];
	if (!_readTextCoords(ofs_current, surf_header->num_verts, sts)) {
		printf("\nERROR: could not read texture coordinates\n");
		return false;
	}


	ofs_current = ofs_surface + surf_header->ofs_xyznormal;
	// Read in all the vertices
	xyznormals = new md3_vertex[surf_header->num_verts * surf_header->num_frames];
	for (int i = 0; i < surf_header->num_frames; i++) {
		if (!_readXYZNormals(ofs_current, surf_header->num_verts, xyznormals)) {
			printf("\nERROR: could not read vertices\n");
			return false;
		}
		ofs_current += sizeof(md3_vertex) * surf_header->num_verts;
	}

	// Validate the surface ofs_end
	pos_end = ofs_surface + surf_header->ofs_end;
	if (pos_end != file_in.tellg()) {
		printf("\nERROR: end of surface position does not match OFS_END from file %i + %i = %i but is \n", 
			ofs_surface, surf_header->ofs_end, pos_end);
		printSurfheader(surf_header);
		return false;
	}

	surface->surf_header = surf_header;
	surface->shaders = shaders;
	surface->triangles = triangles;
	surface->sts = sts;
	surface->xyznormals = xyznormals;

	return true;
}

bool MD3Reader::readSurface(int ofs_surface, MD3Surface* surface) {
	printf("Reading single surface...");

	if (!file_in.is_open()) {
		printf("\nERROR: File is not open\n");
		return false;
	}

	if (!_readSurface(ofs_surface, surface)) {
		return false;
	} 

	printf("done\n");
	return true;
}

bool MD3Reader::readSurfaces(int num_surfaces, int ofs_surfaces, MD3Surface* surfaces) {
	int ofs_current;

	printf("Reading %i surface%s...", num_surfaces, (num_surfaces != 1) ? "s": "");

	if (!file_in.is_open()) {
		printf("\nERROR: File is not open\n");
		return false;
	}

	ofs_current = ofs_surfaces;
	for(int i = 0; i < num_surfaces; i++) {
		if(!_readSurface(ofs_current, &surfaces[i])) {
			return false;
		}

		ofs_current += surfaces[i].surf_header->ofs_end;
	}

	printf("done\n");
	return true;
}

bool MD3Reader::readTags(int num_tags, int ofs_tags, md3_tag* tags) {
	printf("Reading %i tag%s...", num_tags, (num_tags != 1) ? "s": "");

	if (!file_in.is_open()) {
		printf("\nERROR: File is not open\n");
		return false;
	}

	file_in.seekg(ofs_tags);

	if (file_in.tellg() != ofs_tags) {
		printf("\nERROR: could not reach tag offset\n");
		return false;
	}

	file_in.read((char*)tags, sizeof(md3_tag) * num_tags);

	if (_checkError()) {
		printf("\nERROR: could not read tags\n");
		return false;
	}

	printf("done\n");
	return true;
}

bool MD3Reader::readFrames(int num_frames, int ofs_frames, md3_frame* frames) {
	printf("Reading %i frame%s...", num_frames, (num_frames != 1) ? "s": "");

	if (!file_in.is_open()) {
		printf("\nERROR: File is not open\n");
		return false;
	}

	file_in.seekg(ofs_frames);

	if (file_in.tellg() != ofs_frames) {
		printf("\nERROR: could not reach frame offset\n");
		return false;
	}

	file_in.read((char*)frames, sizeof(md3_frame) * num_frames);

	if (_checkError()) {
		printf("\nERROR: could not read frames\n");
		return false;
	}

	printf("done\n");
	return true;
}

bool MD3Reader::readHeader(md3_header* header) {
	printf("Reading header...");

	if (!file_in.is_open()) {
		printf("\nERROR: File is not open\n");
		return false;
	}

	file_in.read((char*)header, sizeof(md3_header));

	if (_checkError()) {
		printf("\nERROR: could not read header\n");
		return false;
	}

	printf("done\n");
	return true;
}

void MD3Reader::close() {
	printf("Closing file...");
	file_in.close();

	if (_checkError()) {
		printf("\nERROR: could not close file\n");
		return;
	}

	printf("done\n");
}
