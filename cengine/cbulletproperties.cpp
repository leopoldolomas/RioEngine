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

#include "cbulletproperties.h"
#include "bullet/btcustom.h"

//-----------------------------------------------------------------------------

CBulletProperties::CBulletProperties() : CObject() {
    m_shapeType       = CBulletProperties::ShapeTypes::Box;
    m_activationState = CBulletProperties::ActivationState::ActiveTag;
    m_linearFactor    = btCustom::zero_vector();
    m_angularFactor   = btCustom::zero_vector();
    m_size            = btCustom::zero_vector();
    m_planeNormal     = btCustom::zero_vector();
    m_physicsEnabled  = false;
    m_vertices        = "";
    m_mass            = 0;
    m_restitution     = 0;
    m_friction        = 0;
    m_height          = 0;
    m_radius          = 0;
    m_planeConstant   = 0;
    m_rigidBody       = NULL;
}

//-----------------------------------------------------------------------------

CBulletProperties::CBulletProperties(const CBulletProperties& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CBulletProperties::copyValuesFromObject(const CBulletProperties& original_obj) {
    m_physicsEnabled  = original_obj.physicsEnabled();
    m_mass            = original_obj.mass();
    m_restitution     = original_obj.restitution();
    m_friction        = original_obj.friction();
    m_shapeType       = original_obj.shapeType();
    m_activationState = original_obj.activationState();
    m_height          = original_obj.height();
    m_radius          = original_obj.radius();
    m_planeNormal     = original_obj.planeNormal();
    m_planeConstant   = original_obj.planeConstant();
    m_vertices        = original_obj.vertices();
    m_size            = original_obj.size();
    m_rigidBody       = NULL;
}

//-----------------------------------------------------------------------------

const CBulletProperties& CBulletProperties::operator=(const CBulletProperties& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

const bool CBulletProperties::physicsEnabled() const {
    return m_physicsEnabled;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setPhysicsEnabled(bool physics_enabled) {
    m_physicsEnabled = physics_enabled;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::mass() const {
    return m_mass;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setMass(float mass) {
    m_mass = mass;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::restitution() const {
    return m_restitution;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setRestitution(float restitution) {
    m_restitution = restitution;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::friction() const {
    return m_friction;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setFriction(float friction) {
    m_friction = friction;
}

//-----------------------------------------------------------------------------

const btVector3& CBulletProperties::linearFactor() const {
    return m_linearFactor;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setLinearFactor(const btVector3 &linear_factor) {
    m_linearFactor = linear_factor;
}

//-----------------------------------------------------------------------------

const btVector3& CBulletProperties::angularFactor() const {
    return m_angularFactor;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setAngularFactor(const btVector3 &angular_factor) {
    m_angularFactor = angular_factor;
}

//-----------------------------------------------------------------------------

const CBulletProperties::ShapeTypes CBulletProperties::shapeType() const {
    return m_shapeType;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setShapeType(CBulletProperties::ShapeTypes shape_type) {
    m_shapeType = shape_type;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::height() const {
    return m_height;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setHeight(float height) {
    m_height = height;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::radius() const {
    return m_radius;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setRadius(float radius) {
    m_radius = radius;
}

//-----------------------------------------------------------------------------

const btVector3& CBulletProperties::size() const {
    return m_size;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setSize(const btVector3 &size) {
    m_size = size;
}

//-----------------------------------------------------------------------------

const btVector3& CBulletProperties::planeNormal() const {
    return m_planeNormal;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setPlaneNormal(const btVector3 &plane_normal) {
    m_planeNormal = plane_normal;
}

//-----------------------------------------------------------------------------

const float CBulletProperties::planeConstant() const {
    return m_planeConstant;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setPlaneConstant(float plane_constant) {
    m_planeConstant = plane_constant;
}

//-----------------------------------------------------------------------------

const QString& CBulletProperties::vertices() const {
    return m_vertices;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setVertices(const QString& vertices) {
    m_vertices = vertices;
}

//-----------------------------------------------------------------------------

btRigidBody* CBulletProperties::rigidBody() const {
    return m_rigidBody;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setRigidBody(btRigidBody* rigid_body) {
    if (rigid_body) {
        // TODO
    }
    m_rigidBody = rigid_body;
}

//-----------------------------------------------------------------------------

CBulletProperties::ActivationState CBulletProperties::activationState() const {
    return m_activationState;
}

//-----------------------------------------------------------------------------

void CBulletProperties::setActivationState(const CBulletProperties::ActivationState &activationState) {
    m_activationState = activationState;
}

//-----------------------------------------------------------------------------

CBulletProperties::~CBulletProperties(void) {
}
