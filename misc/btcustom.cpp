#include "macro.h"
#include "btcustom.h"

namespace btCustom {

//-----------------------------------------------------------------------------

btVector3 getAxisX(btMatrix3x3 &m) {
    return btVector3(m[0][0], m[0][1], m[0][2]);
}

//-----------------------------------------------------------------------------

btVector3 getAxisY(btMatrix3x3 &m) {
    return btVector3(m[1][0], m[1][1], m[1][2]);
}

//-----------------------------------------------------------------------------

btVector3 getAxisZ(btMatrix3x3 &m) {
    return btVector3(m[2][0], m[2][1], m[2][2]);
}

//-----------------------------------------------------------------------------

btVector3 zero_vector() {
    return btVector3(0,0,0);
}

//-----------------------------------------------------------------------------

btVector3 right() {
    return btVector3(1.0f, 0, 0);
}

//-----------------------------------------------------------------------------

btVector3 right(const btQuaternion &q) {
    btMatrix3x3 m = btMatrix3x3(q);
    btVector3 axis_y = btVector3(m[0][1], m[1][1], m[2][1]);
    btVector3 axis_z = btVector3(m[0][2], m[1][2], m[2][2]);
    btVector3 right = axis_y.cross(axis_z);
    right.normalize();
    return right;
}

//-----------------------------------------------------------------------------

btVector3 up() {
    return btVector3(0, 1.0f, 0);
}

//-----------------------------------------------------------------------------

btVector3 up(const btQuaternion &q) {
    btMatrix3x3 m = btMatrix3x3(q);
    btVector3 axis_x = btVector3(m[0][0], m[1][0], m[2][0]);
    btVector3 axis_z = btVector3(m[0][2], m[1][2], m[2][2]);
    btVector3 up = axis_x.cross(axis_z);
    up.normalize();
    return up;
}

//-----------------------------------------------------------------------------

btVector3 forward() {
    return btVector3(0, 0, 1.0f);
}

//-----------------------------------------------------------------------------

btVector3 forward(const btQuaternion &q)
{
    btMatrix3x3 m = btMatrix3x3(q);
    btVector3 axis_x = btVector3(m[0][0], m[1][0], m[2][0]);
    btVector3 axis_y = btVector3(m[0][1], m[1][1], m[2][1]);
    btVector3 forward = axis_x.cross(axis_y);
    forward.normalize();
    return forward;
}

//-----------------------------------------------------------------------------

void print(const btVector3& vec) {
    LOG("vec x: %.2f y: %.2f z: %.2f", vec.x(), vec.y(), vec.z());
}

//-----------------------------------------------------------------------------

void print(const btVector3* vec) {
    LOG("vec x: %.2f y: %.2f z: %.2f", vec->x(), vec->y(), vec->z());
}

//-----------------------------------------------------------------------------

void print(const btQuaternion& quat) {
    LOG("quat x: %.2f y: %.2f z: %.2f, w: %.2f", quat.x(), quat.y(), quat.z(), quat.w());
}

//-----------------------------------------------------------------------------

void print(const btMatrix3x3& matrix) {
    float data[12];
    matrix.getOpenGLSubMatrix(data);
    LOG("---------------- Matrix begins");
    LOG("%.2f %.2f %.2f %.2f", data[0], data[1], data[2], data[3]);
    LOG("%.2f %.2f %.2f %.2f", data[4], data[5], data[6], data[7]);
    LOG("%.2f %.2f %.2f %.2f", data[8], data[9], data[10], data[11]);
    LOG("---------------- Matrix ends");
}

//-----------------------------------------------------------------------------

btVector3 getAxisX(const btQuaternion &q) {
    return getAxisX(btMatrix3x3(q));
}

//-----------------------------------------------------------------------------

btVector3 getAxisY(const btQuaternion &q) {
    return getAxisY(btMatrix3x3(q));
}

//-----------------------------------------------------------------------------

btVector3 getAxisZ(const btQuaternion &q) {
    return getAxisZ(btMatrix3x3(q));
}

//-----------------------------------------------------------------------------

Matrix4 btQuaternionToMatrix4(const btQuaternion& q){
    btMatrix3x3 matrix3 = btMatrix3x3(q);
    btScalar elements_array[12];
    matrix3.getOpenGLSubMatrix(elements_array);

    Matrix4 matrix4;
    matrix4.set(
        elements_array[0],  // xx
        elements_array[1],  // xy
        elements_array[2],  // xz
            0,              // tx
        elements_array[4],  // yx
        elements_array[5],  // yy
        elements_array[6],  // yz
            0,              // ty
        elements_array[8],  // zx
        elements_array[9],  // zy
        elements_array[10],  // zz
            0,              // tz
        0,0,0,1
            );

    return matrix4;
}
}
