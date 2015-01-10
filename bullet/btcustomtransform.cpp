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

#include "bullet/btcustomtransform.h"

//-----------------------------------------------------------------------------

btCustomTransform::btCustomTransform() : CObject() {
    setIdentity();
}

//-----------------------------------------------------------------------------

void btCustomTransform::setIdentity() {
    m_innerTransform.setIdentity();
    m_scale.setIdentity();
}

//-----------------------------------------------------------------------------

void btCustomTransform::getOpenGLMatrix(btScalar* m) {
    btMatrix3x3 original_basis = m_innerTransform.getBasis();
    m_innerTransform.setBasis(original_basis * m_scale);
    m_innerTransform.getOpenGLMatrix(m);
    m_innerTransform.setBasis(original_basis);
}

//-----------------------------------------------------------------------------

btCustomTransform btCustomTransform::operator *(btCustomTransform transform) {
    btCustomTransform output = btCustomTransform();
    output.scale(m_scale * transform.getScale());
    output.setInnerTransform(m_innerTransform * transform.getInnerTransform());
    return output;
}

//-----------------------------------------------------------------------------

btTransform& btCustomTransform::getInnerTransform() {
    return m_innerTransform;
}

//-----------------------------------------------------------------------------

void btCustomTransform::setInnerTransform(const btTransform &innerTransform) {
    m_innerTransform = innerTransform;
}

//-----------------------------------------------------------------------------

btMatrix3x3& btCustomTransform::getScale() {
    return m_scale;
}

//-----------------------------------------------------------------------------

btVector3 btCustomTransform::getScaleAsVector3() const {
    return btVector3(m_scale[0][0],
                     m_scale[1][1],
                     m_scale[2][2]);
}

//-----------------------------------------------------------------------------

void btCustomTransform::translate(btVector3 d) {
    d.setZ(d.getZ() * -1.0f);
    m_innerTransform.setOrigin(m_innerTransform.getOrigin() + d);
}

//-----------------------------------------------------------------------------

void btCustomTransform::translate(float x, float y, float z) {
    translate(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void btCustomTransform::scale(btMatrix3x3 scale) {
    m_scale = scale;
}

//-----------------------------------------------------------------------------

void btCustomTransform::scale(btVector3 scale) {
    m_scale.setIdentity();
    m_scale[0][0] = scale.x();
    m_scale[1][1] = scale.y();
    m_scale[2][2] = scale.z();
}

//-----------------------------------------------------------------------------

void btCustomTransform::scale(float x, float y, float z) {
    scale(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void btCustomTransform::rotate(btQuaternion q) {
    m_innerTransform.setRotation(m_innerTransform.getRotation() * q);
}

//-----------------------------------------------------------------------------

void btCustomTransform::rotate(float x, float y, float z) {
    m_innerTransform.setRotation(btQuaternion(x, y, z));
}
