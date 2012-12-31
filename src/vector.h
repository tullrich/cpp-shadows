#ifndef _VECTOR_H
#define _VECTOR_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Vector {
        public:
                float x, y, z;

				Vector();
				Vector(float x, float y, float z);

				float sum();
				void normalize();

				Vector cross (Vector &other);
				Vector operator - (Vector &other);
				Vector operator + (Vector &other);
};

#endif