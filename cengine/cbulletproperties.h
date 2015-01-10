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

#ifndef RIOENGINE_CENGINE_CBULLETPROPERTIES_H_
#define RIOENGINE_CENGINE_CBULLETPROPERTIES_H_

#include "cengine/cobject.h"
#include "bullet/btbulletdynamicscommon.h"

/**
 * Class that represents the most common properties of a btRigidBody
 * (see bullet physics engine and Rigid Bodies)
 */
class CBulletProperties : public CObject {
    Q_OBJECT
    Q_ENUMS(ShapeTypes)
    Q_ENUMS(ActivationState)

public:
    enum ShapeTypes { Sphere = 0, Box = 1, Cylinder = 2, Capsule = 3, Cone = 4,
                      Convex = 5, ConvexTriangle = 6, StaticPlane = 7 };

    enum ActivationState { ActiveTag = 1, IslandSleeping = 2, WantsDeactivation = 3, DisableDeactivation = 4, DisableSimulation = 5 };

    CBulletProperties();
    CBulletProperties(const CBulletProperties& original_obj);
    const CBulletProperties& operator=(const CBulletProperties& original_obj);
    virtual void copyValuesFromObject(const CBulletProperties& original_obj);

    const bool physicsEnabled() const;
    void setPhysicsEnabled(bool physics_enabled);

    const float mass() const;
    void setMass(float mass);

    const float restitution() const;
    void setRestitution(float restitution);

    const float friction() const;
    void setFriction(float friction);

    const btVector3& linearFactor() const;
    void setLinearFactor(const btVector3& linear_factor);

    const btVector3& angularFactor() const;
    void setAngularFactor(const btVector3& angular_factor);

    const ShapeTypes shapeType() const;
    void setShapeType(ShapeTypes shape_type);

    const float height() const;
    void setHeight(float height);

    const float radius() const;
    void setRadius(float radius);

    const btVector3& size() const;
    void setSize(const btVector3 &size);

    const btVector3& planeNormal() const;
    void setPlaneNormal(const btVector3& plane_normal);

    const float planeConstant() const;
    void setPlaneConstant(float plane_constant);

    const QString& vertices() const;
    void setVertices(const QString& vertices);

    btRigidBody* rigidBody() const;
    void setRigidBody(btRigidBody* rigid_body);

    CBulletProperties::ActivationState activationState() const;
    void setActivationState(const CBulletProperties::ActivationState &activationState);

    ~CBulletProperties();

private:
    btRigidBody* m_rigidBody;

    bool  m_physicsEnabled;
    float m_mass;
    float m_restitution;
    float m_friction;
    float m_height;
    float m_radius;
    float m_planeConstant;

    btVector3 m_linearFactor;
    btVector3 m_angularFactor;
    btVector3 m_size;
    btVector3 m_planeNormal;

    CBulletProperties::ShapeTypes      m_shapeType;
    CBulletProperties::ActivationState m_activationState;

    QString m_vertices;
};

#endif  // RIOENGINE_CENGINE_CBULLETPROPERTIES_H_
