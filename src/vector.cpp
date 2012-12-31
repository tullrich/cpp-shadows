#include "vector.h"

Vector::Vector() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vector::Vector(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vector::sum() {

    return( (float)sqrt( x * x + y * y + z * z ) );

}

void Vector::normalize() {
    float total = sum();

    x = x / total;
    y = y / total;
    z = z / total;
}

Vector Vector::cross( Vector &other ) {
    Vector result;

    result.x =  y * other.z - z * other.y;
    result.y = -x * other.z + z * other.x;
    result.z =  x * other.y - y * other.x;

    return result;
}

Vector Vector::operator - ( Vector &other ) {
    Vector result(0.0, 0.0, 0.0);

    result.x = x - other.x;
    result.y = y - other.y;
    result.z = z - other.z;

    return result;
}

Vector Vector::operator + ( Vector &other ) {
    Vector result(0.0, 0.0, 0.0);

    result.x = x + other.x;
    result.y = y + other.y;
    result.z = z + other.z;

    return result;
}