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

#ifndef RIOENGINE_MISC_TRANSFORM_H_
#define RIOENGINE_MISC_TRANSFORM_H_

#include "cengine/cobject.h"
#include "misc/btcustomtransform.h"

class CGameScene;

class Transform : public CObject {
public:
    enum TransformMode { LOCAL, GLOBAL };
    enum UnitType { WORLD, PHYSICSWORLD };

    Transform();

    btCustomTransform& local (bool scale = true) const;
    btCustomTransform  global(bool scale = true) const;

    btVector3    position() const;
    btVector3    scale   () const;
    btQuaternion rotation() const;

    const btVector3& localPosition(UnitType unit_type = WORLD) const;
    btVector3    localScale()    const;
    btQuaternion localRotation() const;

    void setLocalPosition(float x, float y, float z);
    void setLocalPosition(const btVector3& vec);

    void setLocalScale(float x, float y, float z);
    void setLocalScale(float localScale);
    void setLocalScale(const btVector3& scale);

    void setLocalRotation(float w, float x, float y, float z);
    void setLocalRotation(const btQuaternion& q);

    virtual btVector3 getRightVector  (TransformMode transform_mode = LOCAL);
    virtual btVector3 getUpVector     (TransformMode transform_mode = LOCAL);
    virtual btVector3 getForwardVector(TransformMode transform_mode = LOCAL);

    void moveX(float dx, TransformMode transform_mode = GLOBAL);
    void moveY(float dy, TransformMode transform_mode = GLOBAL);
    void moveZ(float dz, TransformMode transform_mode = GLOBAL);

    void rotate(btQuaternion &q, TransformMode transform_mode = GLOBAL);
    void rotateX(float angle,    TransformMode transform_mode = GLOBAL);
    void rotateY(float angle,    TransformMode transform_mode = GLOBAL);
    void rotateZ(float angle,    TransformMode transform_mode = GLOBAL);

private:
    btRigidBody* parentsBody() const;
    CGameScene* parentsScene() const;

    mutable btCustomTransform m_localTransform;
};

class CameraTransform : public Transform {
    virtual btVector3 getRightVector  (TransformMode transform_mode = LOCAL);
    virtual btVector3 getUpVector     (TransformMode transform_mode = LOCAL);
    virtual btVector3 getForwardVector(TransformMode transform_mode = LOCAL);
};

#endif // RIOENGINE_MISC_TRANSFORM_H_
