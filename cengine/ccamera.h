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

#ifndef RIOENGINE_CENGINE_CCAMERA_H_
#define RIOENGINE_CENGINE_CCAMERA_H_

#include "cengine/cobject.h"
#include "misc/transform.h"

/**
  * A very simple Camera class, it should inherit from CGameObject, but it
  * doesn't because CGameObject's m_transform member is Transform type, and
  * CCamera needs CameraTransform type. The solution would be to create a
  * template class where the Type of the Transform object can be specified.
  * Then CGameObject & CCamera would inherit from it as follows:
  *     public CGameObject : public MyGenericType<Transform>
  *     public CCamera : public MyGenericType<CameraTransform>
  * This might be an easy fix but I won't do it since I'm running out of time...
  **/
class CCamera : public CObject {
public:
    CCamera(void);

    void reset();

    CameraTransform& transform();
    const CameraTransform& constTransform() const;
    void setTransform(const CameraTransform &value);

    ~CCamera(void);

protected:
    CameraTransform m_transform;
};

#endif  // RIOENGINE_CENGINE_CCAMERA_H_
