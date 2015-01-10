//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem, you face it like a man."
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

#include <math.h>
#include "constants.h"
#include "matrixstack.h"

//-----------------------------------------------------------------------------

namespace Math {

MatrixStack::MatrixStack() : CObject() {
    m_stack = std::stack<Matrix4>();
    identity();
}

//-----------------------------------------------------------------------------

void MatrixStack::setMatrix(Matrix4 matrix) {
    m_currentMatrix = matrix;
}

//-----------------------------------------------------------------------------

Matrix4& MatrixStack::getMatrix() {
    return m_currentMatrix;
}

//-----------------------------------------------------------------------------

void MatrixStack::identity() {
    m_currentMatrix.identity();
}

//-----------------------------------------------------------------------------

void MatrixStack::pushMatrix() {
    if (m_stack.size() > k_MatrixStack_MaxSize) {
        RE_ASSERT(false); //  Stack Overflow
    }

    m_stack.push(m_currentMatrix);
    identity();
}

//-----------------------------------------------------------------------------

void MatrixStack::popMatrix() {
    if (m_stack.size() == 0) {
        RE_ASSERT(false); //  Stack Underflow
    }

    m_currentMatrix = m_stack.top();
    m_stack.pop();
}

//-----------------------------------------------------------------------------

void MatrixStack::translate(btVector3 position) {
    translate(position.x(), position.y(), position.z());
}

//-----------------------------------------------------------------------------

void MatrixStack::translate(float x, float y, float z) {
    m_currentMatrix.translate(x, y, z);
}

//-----------------------------------------------------------------------------

void MatrixStack::scale(float x, float y, float z) {
    m_currentMatrix.scale(x, y, z);
}

//-----------------------------------------------------------------------------

void MatrixStack::scale(btVector3 _scale) {
    scale(_scale.x(), _scale.y(), _scale.z());
}

//-----------------------------------------------------------------------------

void MatrixStack::scale(float f) {
    scale(btVector3(f, f, f));
}

//-----------------------------------------------------------------------------

void MatrixStack::rotate(btQuaternion rot) {
    btVector3 *axis = &rot.getAxis();
    Matrix4 rot_matrix;
    rot_matrix.rotate(RADTODEG(rot.getAngle()), axis->x(), axis->y(), axis->z());
    m_currentMatrix = m_currentMatrix * rot_matrix;
}

//-----------------------------------------------------------------------------

void MatrixStack::rotate(btVector3 vec) {
    rotate(btQuaternion(vec.x(), vec.y(), vec.z()));
}

//-----------------------------------------------------------------------------

void MatrixStack::rotate(float x, float y, float z) {
    btVector3 rot(y, x, z);
    return rotate(rot);
}

//-----------------------------------------------------------------------------

const std::stack<Matrix4> MatrixStack::getStack() const {
    return m_stack;
}

}
