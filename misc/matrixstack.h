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

#ifndef RIOENGINE_MISC_MATRIXSTACK_H_
#define RIOENGINE_MISC_MATRIXSTACK_H_

#include <stack>
#include "transform_common.h"
#include "cengine/csingleton.h"
#include "bullet/matrix/matrices.h"

#define MATRIXSTACK CSingleton<Math::MatrixStack>::getSharedInstance()

namespace Math {

class MatrixStack : public CObject {
public:
   MatrixStack();

   void setMatrix(Matrix4);
   Matrix4& getMatrix();

   void identity();

   void pushMatrix();
   void popMatrix();

   void translate(btVector3 position);
   void translate(float x, float y, float z);
   void scale(float x, float y, float z);
   void scale(btVector3 scale);
   void scale(float scale);
   void rotate(btQuaternion rot);
   void rotate(btVector3 vec);
   void rotate(float x, float y, float z);

private:
   const std::stack<Matrix4> getStack() const;

    Matrix4             m_currentMatrix;
    std::stack<Matrix4> m_stack;
};

}

#endif  // MATRIXSTACK_H
