#ifndef _CEMERA_H
#define _CEMERA_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shadows-common.h"
#include "matrix.h"

#define RADIAN_PER_DEGREE (3.141592654f / 180.0f)

class Camera {
        public:
 
                Vector u, v, n;
				Matrix modelView, projection;

                float znear, zfar;
				int w, h;
				float distance;
				float fieldofviewY, fieldofviewX;

				Vector eye, center, up;
                        
                Camera();
                ~Camera();

                void place();
                void place(Vector max, Vector min, int w, int h);
				void translate(Vector translation);
				void rotate(Vector rotation);
				void rotateAboutCenter(int horizontalDeg, int verticalDeg);
				void lookAt();

				void perspectiveProjection();
				void setupModelView();

				void drawSelf(Matrix topMatrix, GLuint matrixVar);
				void drawViewCenter();

				Matrix getModelViewProjectionMatrix();
	private:
				
				void drawCameraSightLine();
};

#endif