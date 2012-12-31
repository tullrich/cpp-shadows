#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

Matrix::Matrix() { 
    vals[0]=1.0; vals[4]=0.0; vals[8] =0.0; vals[12]=0.0;
    vals[1]=0.0; vals[5]=1.0; vals[9] =0.0; vals[13]=0.0;
    vals[2]=0.0; vals[6]=0.0; vals[10]=1.0; vals[14]=0.0;
    vals[3]=0.0; vals[7]=0.0; vals[11]=0.0; vals[15]=1.0;
}

void Matrix::apply(Vector *vector) {
    Vector &v = *vector;

    v.x = v.x * vals[0] + v.y * vals[4] + v.z * vals[8];
    v.y = v.x * vals[1] + v.y * vals[5] + v.z * vals[9];
    v.z = v.x * vals[2] + v.y * vals[6] + v.z * vals[10];
}

Matrix Matrix::operator *(Matrix m) {   
	Matrix temp = Matrix();

	/* rows */
	for (int i = 0; i < 4; i++) {

		/* columns */
		for (int j = 0; j < 4; j++) {

			temp.vals[i + j * 4] = (vals[i] * m.vals[j * 4]) 
					+ (vals[i + 4] * m.vals[j * 4 + 1])  
					+ (vals[i + 8] * m.vals[j * 4 + 2])  
					+ (vals[i + 12] * m.vals[j * 4 + 3]);

		}
	}

	return temp;
}

Vector Matrix::operator *(Vector v) {   
	Vector temp = Vector();
	
	temp.x = vals[0] * v.x + vals[4] * v.y + vals[8] * v.z + vals[12];
	temp.y = vals[1] * v.x + vals[5] * v.y + vals[9] * v.z + vals[13];
	temp.z = vals[2] * v.x + vals[6] * v.y + vals[10] * v.z + vals[14];

	return temp;
}

void Matrix::loadRotation(Vector rotation) {
	float sinA = sin(rotation.x * RADIAN_PER_DEGREE);
    float cosA = cos(rotation.x * RADIAN_PER_DEGREE);

	float sinB = sin(rotation.y * RADIAN_PER_DEGREE);
    float cosB = cos(rotation.y * RADIAN_PER_DEGREE);

	float sinC = sin(rotation.z * RADIAN_PER_DEGREE);
    float cosC = cos(rotation.z * RADIAN_PER_DEGREE);

	vals[0]  = cosC * cosB;
    vals[1]  = cosA * sinC * cosB + sinA * sinB;
    vals[2]  = sinA * sinC * cosB - cosA * sinB;
    vals[3]  = 0;

    vals[4]  = -sinC;
    vals[5]  = cosA * cosC;
    vals[6]  = sinA * cosC;
    vals[7]  = 0;

    vals[8]  = cosC * sinB;
    vals[9]  = cosA * sinC * sinB - sinA * cosB;
    vals[10] = sinA * sinC * sinB + cosA * cosB;
    vals[11] = 0;

    vals[12] = 0;
    vals[13] = 0;
    vals[14] = 0;
    vals[15] = 1;
}

void Matrix::loadFrustum(const float r, const float t, const float znear, const float zfar) {

	float zdist = zfar - znear;

	vals[0]  = znear/r;
    vals[1]  = 0;
    vals[2]  = 0;
    vals[3]  = 0;

    vals[4]  = 0;
    vals[5]  = znear/t;
    vals[6]  = 0;
    vals[7]  = 0;

    vals[8]  = 0;
    vals[9]  = 0;
    vals[10] = -(zfar + znear)/zdist;
    vals[11] = -1;

    vals[12] = 0;
    vals[13] = 0;
    vals[14] = -2*(zfar * znear)/zdist;
    vals[15] = 0;
}

void Matrix::setComponents(Vector left, Vector up, Vector forward) {
	vals[0]  = left.x;
    vals[1]  = left.y;
    vals[2]  = left.z;
    vals[3]  = 0;

    vals[4]  = up.x;
    vals[5]  = up.y;
    vals[6]  = up.z;
    vals[7]  = 0;

    vals[8]  = forward.x;
    vals[9]  = forward.y;
    vals[10] = forward.z;
    vals[11] = 0;

    vals[12] = 0;
    vals[13] = 0;
    vals[14] = 0;
    vals[15] = 1;
}

void Matrix::invert() {
	Matrix R;
	R.vals[0] = vals[0];
	R.vals[1] = vals[1];
	R.vals[1] = vals[2];

	R.vals[4] = vals[4];
	R.vals[5] = vals[5];
	R.vals[6] = vals[6];

	R.vals[8] = vals[8];
	R.vals[9] = vals[9];
	R.vals[10] = vals[10];

	R.transpose();

	Matrix T;

	T.vals[12] = -vals[12];
	T.vals[13] = -vals[13];
	T.vals[14] = -vals[14];
	
	memcpy(vals, (R * T).vals, sizeof(float) * 16);
}

void Matrix::invert2() {
	float temp[16];

	Matrix::invertMatrix(vals, temp);

	memcpy(vals, temp, sizeof(float) * 16);
}

bool Matrix::invertMatrix(const float m[16], float invOut[16]) {
	float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

void Matrix::loadTranslation(Vector translation) {
	vals[0]  = 1;
    vals[1]  = 0;
    vals[2]  = 0;
    vals[3]  = 0;

    vals[4]  = 0;
    vals[5]  = 1;
    vals[6]  = 0;
    vals[7]  = 0;

    vals[8]  = 0;
    vals[9]  = 0;
    vals[10] = 1;
    vals[11] = 0;

    vals[12] = translation.x;
    vals[13] = translation.y;
    vals[14] = translation.z;
    vals[15] = 1;
}

void Matrix::transpose() {
	Matrix temp;

	temp.vals[0] = vals[0];
	temp.vals[1] = vals[4];
	temp.vals[2] = vals[8];
	temp.vals[3] = vals[12];

	temp.vals[4] = vals[1];
	temp.vals[5] = vals[5];
	temp.vals[6] = vals[9];
	temp.vals[7] = vals[13];

	temp.vals[8] = vals[2];
	temp.vals[9] = vals[6];
	temp.vals[10] = vals[10];
	temp.vals[11] = vals[14];

	temp.vals[12] = vals[3];
	temp.vals[13] = vals[7];
	temp.vals[14] = vals[11];
	temp.vals[15] = vals[15];

	memcpy(vals, temp.vals, sizeof(float) * 16);
}

void Matrix::translate(Vector translation) {
	vals[12] += translation.x;
	vals[13] += translation.y;
	vals[14] += translation.z;
	//vals[12] += translation.x * vals[0] + translation.y * vals[4] + translation.z * vals[8];
	//vals[13] += translation.x * vals[1] + translation.y * vals[5] + translation.z * vals[9];
	//vals[14] += translation.x * vals[2] + translation.y * vals[6] + translation.z * vals[10];
}

char* Matrix::toString() {
	char out[500];
	sprintf(out, "[%3f, %3f, %3f, %3f\n%3f, %3f, %3f, %3f\n%3f, %3f, %3f, %3f\n%3f, %3f, %3f, %3f]\n", 
		vals[0], vals[4],vals[8], vals[12], 
		vals[1], vals[5], vals[9], vals[13],
		vals[2], vals[6], vals[10], vals[14],
		vals[3], vals[7], vals[11], vals[15]);

	return out;
}