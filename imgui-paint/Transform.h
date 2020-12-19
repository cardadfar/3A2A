#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include <math.h>

class Vector3 {
    public:

    Vector3( float x = 0, float y = 0, float z = 0)
      : x( x ), y( y ), z( z ) { };

    float dot(Vector3 v) {
        return v.x * x +\
               v.y * y +\
               v.z * z; 
    }

    inline Vector3 operator+(Vector3 v ) {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    inline Vector3 operator-(Vector3 v ) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    inline void operator+=( Vector3 v ) {
        x += v.x; y += v.y; z += v.z;
    }

    inline void operator-=( Vector3 v ) {
        x -= v.x; y -= v.y; z -= v.z;
    }

    float x;
    float y;
    float z;

};



class Matrix3x3 {
    public:

    Matrix3x3() { 
        entries[0] = Vector3(1, 0, 0);
        entries[1] = Vector3(0, 1, 0);
        entries[2] = Vector3(0, 0, 1);
    }

    Matrix3x3(double *data) {
        for( int i = 0; i < 3; i++ ) {
            entries[i] = Vector3(data[i*3+0],data[i*3+1],data[i*3+2]);
        }
    }

    Matrix3x3(int axis, float theta) {
        switch(axis) {
            case 0: // X-axis
                entries[0] = Vector3(1, 0         , 0          );
                entries[1] = Vector3(0, cos(theta), -sin(theta));
                entries[2] = Vector3(0, sin(theta), cos(theta));
                break;
            case 1: // Y-axis
                entries[0] = Vector3( cos(theta), 0, sin(theta));
                entries[1] = Vector3(0          , 1, 0         );
                entries[2] = Vector3(-sin(theta), 0, cos(theta));
                break;
            default: // Z-axis
                entries[0] = Vector3(cos(theta), -sin(theta), 0);
                entries[1] = Vector3(sin(theta), cos(theta) , 0);
                entries[2] = Vector3(0         , 0          , 1);
        }
    }

    Vector3 dot(Vector3 v) {
        float x = v.dot(entries[0]);
        float y = v.dot(entries[1]);
        float z = v.dot(entries[2]);
        return Vector3(x, y, z);
    }

    Vector3 entries[3];

};


#endif 
