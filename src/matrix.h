#ifndef _MATRIX_H
#define _MATRIX_H

#include "vector.h"

#define RADIAN_PER_DEGREE (3.141592654f / 180.0f)

class Matrix {

        public:

                float vals[16];

                Matrix();

				Matrix operator *(Matrix m);
				Vector operator *(Vector m);

				void transpose();
				void invert();
				void invert2();

				
				static bool invertMatrix(const float m[16], float invOut[16]);

				void loadRotation(Vector rotation);

                void rotate(Vector rotation);

                void apply(Vector *vector);

				void setComponents(Vector left, Vector up, Vector forward);

				void loadFrustum(const float r, const float t, const float znear, const float zfar);

				void loadTranslation(Vector translation);
				void translate(Vector translation);

				char* toString();

};

#endif