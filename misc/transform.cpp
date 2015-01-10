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

#include "constants.h"
#include "misc/btcustom.h"
#include "cengine/cgameobject.h"
#include "transform.h"

//-----------------------------------------------------------------------------

Transform::Transform()  : CObject() {
    m_localTransform = btCustomTransform();
}

//-----------------------------------------------------------------------------

btRigidBody* Transform::parentsBody() const {
    if (!parent()) {
        return NULL;
    }

    CGameObject* parent_as_game_object = qobject_cast<CGameObject*>(parent());
    RE_ASSERT(parent_as_game_object);

    return parent_as_game_object->bulletProperties.rigidBody();
}

//-----------------------------------------------------------------------------

CGameScene* Transform::parentsScene() const {
    if (!parent()) {
        return NULL;
    }

    CGameObject* parent_as_game_object = qobject_cast<CGameObject*>(parent());
    RE_ASSERT(parent_as_game_object);

    return parent_as_game_object->parentScene();
}

//-----------------------------------------------------------------------------

btVector3 Transform::position() const {
    if(parent() && parent()->parent()) { // TODO Wat? Improve this
        CGameObject* parent_object = qobject_cast<CGameObject*>(parent()->parent());
        if(parent_object) {
            return parent_object->transform().position() + localPosition();
        }
    }
    return localPosition();
}

//-----------------------------------------------------------------------------

btVector3 Transform::scale() const {
    if(parent() && parent()->parent()) { //  TODO improve this
        CGameObject* parent_object = qobject_cast<CGameObject*>(parent()->parent());
        if(parent_object) {
            return parent_object->transform().scale() * localScale();
        }
    }
    return localScale();
}

//-----------------------------------------------------------------------------

btQuaternion Transform::rotation() const {
    if(parent() && parent()->parent()) { //  TODO improve this
        CGameObject* parent_object = qobject_cast<CGameObject*>(parent()->parent());
        if(parent_object) {
            return parent_object->transform().rotation() * localRotation();
        }
    }
    return localRotation();
}

//-----------------------------------------------------------------------------

const btVector3& Transform::localPosition(UnitType unit_type) const {
    if (!parentsBody()) {
        const btVector3& origin = m_localTransform.getInnerTransform().getOrigin();
        return unit_type == WORLD ? origin : origin * k_QGLGameEditorViewport_SizeFactor;
    }

    btTransform t;
    parentsBody()->getMotionState()->getWorldTransform(t);

    btTransform& transform = m_localTransform.getInnerTransform();
    transform.setOrigin(
                unit_type == WORLD ? t.getOrigin() / k_QGLGameEditorViewport_SizeFactor : t.getOrigin()
                );
    return transform.getOrigin();
}

//-----------------------------------------------------------------------------

btVector3 Transform::localScale() const {
    btMatrix3x3 scale = m_localTransform.getScale();
    return btVector3(scale[0][0],
                     scale[1][1],
                     scale[2][2]);
}

//-----------------------------------------------------------------------------

btQuaternion Transform::localRotation() const {
    if (!parentsBody()) {
        return  m_localTransform.getInnerTransform().getRotation();
    }

    btTransform t;
    parentsBody()->getMotionState()->getWorldTransform(t);

    btTransform transform = m_localTransform.getInnerTransform();
    transform.setRotation(t.getRotation());
    return transform.getRotation();
}

//-----------------------------------------------------------------------------

void Transform::setLocalPosition(const btVector3& p) {
    if (!parentsBody()) {
        m_localTransform.getInnerTransform().setOrigin(p);
        return;
    }

    btTransform t = parentsBody()->getWorldTransform();
    t.setOrigin(p * k_QGLGameEditorViewport_SizeFactor);

    parentsBody()->setWorldTransform(t);

    // TODO what is the difference between the two following calls?
    parentsBody()->setCenterOfMassTransform(t);
    parentsBody()->getMotionState()->setWorldTransform(t);
}

//-----------------------------------------------------------------------------

void Transform::setLocalPosition(float x, float y, float z) {
    setLocalPosition(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void Transform::setLocalScale(const btVector3& s) {
    btMatrix3x3* scale = &m_localTransform.getScale();
    scale->setValue(s.x(), 0, 0,
                    0, s.y(), 0,
                    0, 0, s.z());
}

//-----------------------------------------------------------------------------

void Transform::setLocalScale(float x, float y, float z) {
    setLocalScale(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void Transform::setLocalScale(float scale) {
    setLocalScale(scale, scale, scale);
}

//-----------------------------------------------------------------------------

void Transform::setLocalRotation(const btQuaternion& r) {
    if (!parentsBody()) {
        m_localTransform.getInnerTransform().setRotation(r);
        return;
    }

    btTransform t;
    parentsBody()->getMotionState()->getWorldTransform(t);
    t.setRotation(r);

    // TODO what is the difference between the two following calls?
    parentsBody()->setWorldTransform(t);
    parentsBody()->getMotionState()->setWorldTransform(t);
}

//-----------------------------------------------------------------------------

void Transform::setLocalRotation(float w, float x, float y, float z) {
    setLocalRotation(btQuaternion(w, x, y, z));
}

//-----------------------------------------------------------------------------

btVector3 Transform::getRightVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::right();
    }
    return btCustom::right(localRotation());
}

//-----------------------------------------------------------------------------

btVector3 Transform::getUpVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::up();
    }
    return btCustom::up(localRotation());
}

//-----------------------------------------------------------------------------

btVector3 Transform::getForwardVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::forward();
    }
    return btCustom::forward(localRotation());
}

//-----------------------------------------------------------------------------

void Transform::moveX(float dx, TransformMode transform_mode) {
    btVector3 right_vec = getRightVector(transform_mode);
    btVector3 new_position = localPosition() + (right_vec * dx);
    setLocalPosition(new_position);
}

//-----------------------------------------------------------------------------

void Transform::moveY(float dy, TransformMode transform_mode) {
    btVector3 up_vec = getUpVector(transform_mode);
    btVector3 new_position = localPosition() + (up_vec * dy);
    setLocalPosition(new_position);
}

//-----------------------------------------------------------------------------

void Transform::moveZ(float dz, TransformMode transform_mode) {
    dz *= -1.0f;
    btVector3 forward_vec = getForwardVector(transform_mode);
    btVector3 new_position = localPosition() + (forward_vec * dz);
    setLocalPosition(new_position);
}

//-----------------------------------------------------------------------------

void Transform::rotate(btQuaternion &q, TransformMode transform_mode) {
    if (transform_mode == LOCAL) {
        setLocalRotation(localRotation() * q);
    } else {
        setLocalRotation(q * localRotation());
    }
}

//-----------------------------------------------------------------------------

void Transform::rotateX(float angle, TransformMode transform_mode) {
    btQuaternion x_rot(btVector3(1.0f, 0, 0), DEGTORAD(angle));
    rotate(x_rot, transform_mode);
}

//-----------------------------------------------------------------------------

void Transform::rotateY(float angle, TransformMode transform_mode) {
    btQuaternion y_rot(btVector3(0, 1.0f, 0), DEGTORAD(angle));
    if (transform_mode == LOCAL) {
        setLocalRotation(y_rot * localRotation());
    } else {
        setLocalRotation(localRotation() * y_rot);
    }
}

//-----------------------------------------------------------------------------

void Transform::rotateZ(float angle, TransformMode transform_mode) {
    btQuaternion z_rot(btVector3(0, 0, 1.0f), angle);
    rotate(z_rot, transform_mode);
}

//-----------------------------------------------------------------------------

btCustomTransform& Transform::local(bool scale) const {
    btMatrix3x3 scale_transform;
    scale_transform.setIdentity();

    if(scale) {
        scale_transform = m_localTransform.getScale();
    }

    if (!parentsBody()) {
        return m_localTransform;
    }

    m_localTransform.setInnerTransform(parentsBody()->getWorldTransform());
    m_localTransform.scale(scale_transform);
    m_localTransform.getInnerTransform().setOrigin(m_localTransform.getInnerTransform().getOrigin() / k_QGLGameEditorViewport_SizeFactor);
    return m_localTransform;
}

//-----------------------------------------------------------------------------

btCustomTransform Transform::global(bool scale) const {
    if(parent() && parent()->parent()) { // TODO improve this
        CGameObject* parent_object = qobject_cast<CGameObject*>(parent()->parent());
        if(parent_object) {
            return parent_object->transform().local(scale) * local(scale);
        }
    }
    return local(scale);
}

//------------------------------------------- CameraTransform overridden methods

btVector3 CameraTransform::getRightVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::right();
    }
    return btCustom::getAxisX(localRotation());
}

//-----------------------------------------------------------------------------

btVector3 CameraTransform::getUpVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::up();
    }
    return btCustom::getAxisY(localRotation());
}

//-----------------------------------------------------------------------------

btVector3 CameraTransform::getForwardVector(Transform::TransformMode transform_mode) {
    if(transform_mode == Transform::GLOBAL) {
        return btCustom::forward();
    }
    return btCustom::getAxisZ(localRotation());
}
