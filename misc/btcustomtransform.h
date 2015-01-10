//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem?, you face it like a man."
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
// software, either in source code form or as a compiled binary, for any purpose,
// commercial or non-commercial, and by any means.
//
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be
// an overt act of relinquishment in perpetuity of all present and future
// rights to this software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//--------------------------------------------------------------- @License ends

#ifndef RIOENGINE_BTCUSTOMTRANSFORM_H_
#define RIOENGINE_BTCUSTOMTRANSFORM_H_

#include "bullet/btBulletCollisionCommon.h"
#include "cengine/cobject.h"

/**
 * @brief btTransform + Scale Matrix
 */
class btCustomTransform : public CObject
{
public:
    btCustomTransform();
    btCustomTransform operator *(btCustomTransform transform);

    void setIdentity();
    void getOpenGLMatrix(btScalar *m);

    btTransform& getInnerTransform();
    void setInnerTransform(const btTransform &inner_transform);

    btMatrix3x3& getScale();
    btVector3 getScaleAsVector3() const;

    void translate(btVector3 d);
    void translate(float x, float y, float z);
    void scale(btMatrix3x3 m);
    void scale(btVector3 s);
    void scale(float x, float y, float z);
    void rotate(btQuaternion q);
    void rotate(float x, float y, float z);

private:
    btTransform m_innerTransform;
    btMatrix3x3 m_scale;
};

#endif // RIOENGINE_BTCUSTOMTRANSFORM_H_
