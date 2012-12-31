#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modelutil.h"

Model readModel(const char *fileName)
{
	Model model;
	model.max.x = model.max.y = model.max.z = 0;
	model.min.x = model.min.y = model.min.z = 0;

	int i;
	
	FILE* fp = fopen(fileName,"r");
	if (fp==NULL) { 
		printf("ERROR: unable to open TriObj [%s]!\n",fileName); 
		exit(1); 
	}

	char ch;
	fscanf(fp, "%c", &ch);
	while(ch!= '\n') // skip the first line – object’s name
		fscanf(fp, "%c", &ch);

	//
	fscanf(fp,"# triangles = %d\n", &model.numTriangles); // read # of triangles
	fscanf(fp,"Material count = %d\n", &model.numMaterials); // read material count
	//

	model.materials = new Material[model.numMaterials];
	model.triangles = new Triangle[model.numTriangles];

	for (i=0; i<model.numMaterials; i++) {
		Material* cur = &(model.materials[i]);

		fscanf(fp, "ambient color %f %f %f\n", &((*cur).ambient.x), &((*cur).ambient.y), &((*cur).ambient.z));
		fscanf(fp, "diffuse color %f %f %f\n", &((*cur).diffuse.x), &((*cur).diffuse.y), &((*cur).diffuse.z));
		fscanf(fp, "specular color %f %f %f\n", &((*cur).specular.x), &((*cur).specular.y), &((*cur).specular.z));
		fscanf(fp, "material shine %f\n", &((*cur).shine));
	}
	//
	fscanf(fp, "%c", &ch);
	while(ch!= '\n') // skip documentation line
		fscanf(fp, "%c", &ch);
	//
	// allocate triangles for tri model
	//
	printf ("Reading in %s (%d triangles). . . ", fileName, model.numTriangles);
	//
	for (i=0; i<model.numTriangles; i++) // read triangles
	{
		fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
		&(model.triangles[i].v0.x), &(model.triangles[i].v0.y), &(model.triangles[i].v0.z),
		&(model.triangles[i].normal[0].x), &(model.triangles[i]. normal [0].y), &(model.triangles[i]. normal [0].z),
		&(model.triangles[i].material_indices[0]));

		
		if (model.triangles[i].v0.x > model.max.x) {
			model.max.x = model.triangles[i].v0.x;
		} 
		
		if (model.triangles[i].v0.x < model.min.x) {
			model.min.x = model.triangles[i].v0.x;
		}

		if (model.triangles[i].v0.y > model.max.y) {
			model.max.y = model.triangles[i].v0.y;
		} 
		
		if (model.triangles[i].v0.y < model.min.y) {
			model.min.y = model.triangles[i].v0.y;
		}

		if (model.triangles[i].v0.z > model.max.z) {
			model.max.z = model.triangles[i].v0.z;
		} 
		
		if (model.triangles[i].v0.z < model.min.z) {
			model.min.z = model.triangles[i].v0.z;
		}

		fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
		&(model.triangles[i].v1.x), &(model.triangles[i].v1.y), &(model.triangles[i].v1.z),
		&(model.triangles[i].normal[1].x), &(model.triangles[i].normal[1].y), &(model.triangles[i].normal[1].z),
		&(model.triangles[i].material_indices[1]));

		if (model.triangles[i].v1.x > model.max.x) {
			model.max.x = model.triangles[i].v1.x;
		}
		
		if (model.triangles[i].v1.x < model.min.x) {
			model.min.x = model.triangles[i].v1.x;
		}

		if (model.triangles[i].v1.y > model.max.y) {
			model.max.y = model.triangles[i].v1.y;
		} 
		
		if (model.triangles[i].v1.y < model.min.y) {
			model.min.y = model.triangles[i].v1.y;
		}

		if (model.triangles[i].v1.z > model.max.z) {
			model.max.z = model.triangles[i].v1.z;
		} 
		
		if (model.triangles[i].v1.z < model.min.z) {
			model.min.z = model.triangles[i].v1.z;
		}

		fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
		&(model.triangles[i].v2.x), &(model.triangles[i].v2.y), &(model.triangles[i].v2.z),
		&(model.triangles[i].normal[2].x), &(model.triangles[i].normal[2].y), &(model.triangles[i].normal[2].z),
		&(model.triangles[i].material_indices[2]));
		fscanf(fp, "face normal %f %f %f\n", &(model.triangles[i].face_normal.x), &(model.triangles[i].face_normal.y),
		&(model.triangles[i].face_normal.z));

		if (model.triangles[i].v2.x > model.max.x) {
			model.max.x = model.triangles[i].v2.x;
		} 
		
		if (model.triangles[i].v2.x < model.min.x) {
			model.min.x = model.triangles[i].v2.x;
		}

		if (model.triangles[i].v2.y > model.max.y) {
			model.max.y = model.triangles[i].v2.y;
		} 
		
		if (model.triangles[i].v2.y < model.min.y) {
			model.min.y = model.triangles[i].v2.y;
		}

		if (model.triangles[i].v2.z > model.max.z) {
			model.max.z = model.triangles[i].v2.z;
		} 
		
		if (model.triangles[i].v2.z < model.min.z) {
			model.min.z = model.triangles[i].v2.z;
		}

		//
		model.triangles[i].Color[0] = (unsigned char)(int)(255*(model.materials[model.triangles[i].material_indices[0]].diffuse.x));
		model.triangles[i].Color[1] = (unsigned char)(int)(255*(model.materials[model.triangles[i].material_indices[0]].diffuse.y));
		model.triangles[i].Color[2] = (unsigned char)(int)(255*(model.materials[model.triangles[i].material_indices[0]].diffuse.z));
	}
	fclose(fp);

	printf ("Finished\n");

	return model;
}

void drawModel(Model *model, bool colorOverride, float color[3]) {
	glColor3f (1.0, 1.0, 1.0);  
	int i;
	Triangle *t;
	glBegin(GL_TRIANGLES);
	for (i = 0; i < model->numTriangles; i++) {
		t = &(model->triangles[i]);
		
		/* assign color */
		if (!colorOverride) {
			//glColor3ub((*t).Color[0], (*t).Color[1], (*t).Color[2]);
		} else {
			//glColor3f(color[0], color[1], color[2]);
		}
		
		/* assign material */
		Material *material = &model->materials[t->material_indices[0]];

		GLfloat mat_ambient[] = { material->ambient.x, material->ambient.y, material->ambient.z, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

		GLfloat mat_diffuse[] = { material->diffuse.x, material->diffuse.y, material->diffuse.z, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

		GLfloat mat_specular[] = { material->specular.x, material->specular.y, material->specular.z, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

		/* shininess */
		GLfloat shine = material->shine ;
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		/* assign normal */
		glNormal3f(t->face_normal.x, t->face_normal.y, t->face_normal.z);

		/* input vertices */
		glVertex3f((*t).v0.x, (*t).v0.y, (*t).v0.z); 
		glVertex3f((*t).v1.x, (*t).v1.y, (*t).v1.z); 
		glVertex3f((*t).v2.x, (*t).v2.y, (*t).v2.z); 
	}
	glEnd();
}