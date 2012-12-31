#include <cstdio>
#include "md3_structs.h"
#include "md3_surface.h"
#include "md3_mesh.h"

void printHeader(md3_header* h) {
	printf("md3_header {\n");
	printf("	ident: %i\n", h->ident);
	printf("	version: %i\n", h->version);
	printf("	name: %s\n", h->name);
	printf("	flags: %i\n", h->flags);
	printf("	num_frames: %i\n", h->num_frames);
	printf("	num_tags: %i\n", h->num_tags);
	printf("	num_surfaces: %i\n", h->num_surfaces);
	printf("	num_skins: %i\n", h->num_skins);
	printf("	ofs_frames: %i\n", h->ofs_frames);
	printf("	ofs_tags: %i\n", h->ofs_tags);
	printf("	ofs_surfaces: %i\n", h->ofs_surfaces);
	printf("	ofs_eof: %i\n", h->ofs_eof);
	printf("}\n");
}

void printFrame(md3_frame* f) {
	printf("md3_frame {\n");
	printf("	min_bounds: <%f,%f,%f>\n", f->min_bounds.x, f->min_bounds.y, f->min_bounds.z);
	printf("	max_bounds: <%f,%f,%f>\n", f->max_bounds.x, f->max_bounds.y, f->max_bounds.z);
	printf("	local_origin: <%f,%f,%f>\n", f->local_origin.x, f->local_origin.y, f->local_origin.z);
	printf("	radius: %f\n", f->radius);
	printf("	name: %s\n", f->name);
	printf("}\n");
}

void printTag(md3_tag* t) {
	printf("md3_tag {\n");
	printf("	name:%s\n", t->name);
	printf("	origin: <%f,%f,%f>\n", t->origin.x, t->origin.y, t->origin.z);
	printf("	axis:	[%9f, %9f, %9f\n", t->axis[0].x, t->axis[0].y, t->axis[0].z);
	printf("	 	 %9f, %9f, %9f\n", t->axis[1].x, t->axis[1].y, t->axis[1].z);
	printf("	 	 %9f, %9f, %9f]\n", t->axis[2].x, t->axis[2].y, t->axis[2].z);
	printf("}\n");
}

void printSurfheader(md3_surface* sh) {
	printf("md3_surface {\n");
	printf("	ident: %i\n", sh->ident);
	printf("	name: %s\n", sh->name);
	printf("	flags: %i\n", sh->flags);
	printf("	num_frames: %i\n", sh->num_frames);
	printf("	num_shaders: %i\n", sh->num_shaders);
	printf("	num_verts: %i\n", sh->num_verts);
	printf("	num_triangles: %i\n", sh->num_triangles);
	printf("	ofs_triangles: %i\n", sh->ofs_triangles);
	printf("	ofs_shaders: %i\n", sh->ofs_shaders);
	printf("	ofs_st: %i\n", sh->ofs_st);
	printf("	ofs_xyznormal: %i\n", sh->ofs_xyznormal);
	printf("	ofs_end: %i\n", sh->ofs_end);
	printf("}\n");
}

void printShader(md3_shader* sh) {
	printf("md3_shader {\n");
	printf("	name: %s\n", sh->name);
	printf("	shader_index: %i\n", sh->shader_index);
	printf("}\n");
}

void printVertex(md3_vertex* v) {
	printf("md3_vertex {\n");
	printf("	coord: <%i, %i, %i>\n", v->coord[0], v->coord[1], v->coord[2]);
	printf("	normal: <%u, %u>", v->normal[0], v->normal[1]);
	printf("}\n");
}

void printTriangle(md3_triangle* t) {
	printf("md3_triangle {\n");
	printf("	indexes: [%i, %i, %i]\n", t->indexes[0], t->indexes[1], t->indexes[2]);
	printf("}\n");
}

void printTexCoord(md3_texcoord* st) {
	printf("md3_texcoord {\n");
	printf("	indexes: {%f, %f}\n", st->st[0], st->st[1]);
	printf("}\n");
}

void printFrames(int num_frames, md3_frame* frames) {
	for (int i = 0; i < num_frames; i++) {		
		printFrame(&frames[i]);
	}
}

void printTags(int num_tags, md3_tag* tags) {
	for (int i = 0; i < num_tags; i++) {		
		printTag(&tags[i]);
	}
}

void printShaders(int num_shaders, md3_shader* shaders) {
	for (int i = 0; i < num_shaders; i++) {		
		printShader(&shaders[i]);
	}
}

void printVertices(int num_verts, md3_vertex* vertices) {
	for (int i = 0; i < num_verts; i++) {		
		printVertex(&vertices[i]);
	}
}

void printTriangles(int num_triangles, md3_triangle* triangles) {
	for (int i = 0; i < num_triangles; i++) {		
		printTriangle(&triangles[i]);
	}
}

void printTexCoords(int num_verts, md3_texcoord* sts) {
	for (int i = 0; i < num_verts; i++) {		
		printTexCoord(&sts[i]);
	}
}

void printSurface(MD3Surface* s) {
	md3_surface *header = s->surf_header;
	printf("MD3Surface {\n");
		printSurfheader(header);
		printShaders(header->num_shaders, s->shaders);
	printf("}\n");
}

void printSurfacePrimatives(MD3Surface* s) {
	md3_surface *header = s->surf_header;
	printf("MD3Surface {\n");
		printSurfheader(header);
		printShaders(header->num_shaders, s->shaders);
		//printTriangles(header->num_triangles, s->triangles);
		//printVertices(header->num_verts, s->xyznormals);
		//printTexCoords(header->num_verts, s->sts);
	printf("}\n");
}

void printSurfaces(int num_surfaces, MD3Surface* surfaces) {
	for (int i = 0; i < num_surfaces; i++) {		
		printSurfacePrimatives(&surfaces[i]);
	}
}

void printMesh(MD3Mesh *mesh) {
	printHeader(mesh->header);
	printFrames(mesh->header->num_frames, mesh->frames);
	printTags(mesh->header->num_tags, mesh->tags);
	printSurfaces(mesh->header->num_surfaces, mesh->surfaces);
}