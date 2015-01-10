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
#include "misc/genericshelper.hpp"
#include "misc/stringhelper.hpp"
#include "cengine/uidprovider.h"
#include "cengine/cgameobjectfactory.h"
#include "cengine/cactionmanager.h"
#include "clengine/classetsrepository.h"
#include "cengine/cgamescene.h"
#include "cengine/cdirector.h"
#include "cengine/cgameobject.h"

//-----------------------------------------------------------------------------

CGameObject::CGameObject() : CLNode() {
    __init();
}

//-----------------------------------------------------------------------------

void CGameObject::__init() {
    m_actionManager = CDIRECTOR->actionManager();
    m_uid = UIDProvider::nextUID();
    m_CObjectType = CGameObjectFactory::types().indexOf("CGameObject");
    transform().setParent(this);
    m_tag = -1;
}

//-----------------------------------------------------------------------------

CGameObject::CGameObject(const CGameObject& src_game_object) : CLNode() {
    copyValuesFromObject(src_game_object);
}

//-----------------------------------------------------------------------------

const CGameObject& CGameObject::operator=(const CGameObject& original_obj) {
    if (this == &original_obj)
        return *this;

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CGameObject::copyValuesFromObject(const CGameObject& original_obj) {
    __init();

    copyNodeValuesFromNode((CLNode*)&original_obj);
    setProperty("objectName", original_obj.property("objectName"));
    m_transform = Transform(original_obj.constTransform());
    m_CObjectType = original_obj.CObjectType();
    bulletProperties = original_obj.bulletProperties;
    m_uid = UIDProvider::nextUID();
    m_tag = original_obj.tag();

    setDynamicProperties(original_obj.dynamicProperties());
}

//-----------------------------------------------------------------------------

void CGameObject::setActionManager(CActionManager* action_manager) {
    if ( m_actionManager != action_manager ) {
        stopAllActions();
        SAFE_RELEASE(m_actionManager);

        m_actionManager = (CActionManager*)action_manager->retain();
    }
}

//-----------------------------------------------------------------------------

CActionManager* CGameObject::actionManager() const {
    return m_actionManager;
}

//-----------------------------------------------------------------------------

void CGameObject::setName(const std::string& name) {
    setProperty("objectName", QString::fromStdString(name));
}

//-----------------------------------------------------------------------------

std::string CGameObject::name() const {
    return property("objectName").toString().toStdString();
}

//-----------------------------------------------------------------------------

const btVector3 CGameObject::position(Transform::UnitType unit_type) {
    return transform().localPosition(unit_type);
}

//-----------------------------------------------------------------------------

void CGameObject::setPosition(const btVector3 &value) {
    transform().setLocalPosition(value);
}

//-----------------------------------------------------------------------------

void CGameObject::setPosition(float x, float y, float z) {
    setPosition(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

const btVector3 CGameObject::scale() {
    return transform().localScale();
}

//-----------------------------------------------------------------------------

void CGameObject::setScale(btVector3 &scale) {
    transform().setLocalScale(scale);
}

//-----------------------------------------------------------------------------

void CGameObject::setScale(float x, float y, float z) {
    setScale(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void CGameObject::setScale(float _scale) {
    setScale(btVector3(scale().x() * _scale,
                       scale().y() * _scale,
                       scale().z() * _scale));
}

//-----------------------------------------------------------------------------

btVector3 CGameObject::rotation() {
    btScalar yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
    transform().local(true).getInnerTransform().getBasis().getEulerZYX(yaw, pitch, roll); // TODO this call is faster but does not produce the expected output
    return btVector3(RADTODEG(roll), RADTODEG(pitch), RADTODEG(yaw));
}

//-----------------------------------------------------------------------------

btQuaternion CGameObject::rotationQuat() {
    return transform().rotation();
}

//-----------------------------------------------------------------------------

void CGameObject::setRotation(const btVector3 &vec_rotation) {
    btQuaternion rotation = btQuaternion(
                DEGTORAD(vec_rotation.y()),
                DEGTORAD(vec_rotation.x()),
                DEGTORAD(vec_rotation.z()));
    rotation.normalize();
    transform().setLocalRotation(rotation);
}

//-----------------------------------------------------------------------------

void CGameObject::setRotation(const btQuaternion& rotation) {
    transform().setLocalRotation(rotation);
}

//-----------------------------------------------------------------------------

void CGameObject::setRotation(float x, float y, float z) {
    setRotation(btVector3(x, y, z));
}

//-----------------------------------------------------------------------------

void CGameObject::setRotation(float w, float x, float y, float z) {
    setRotation(btQuaternion(w, x, y, z));
}

//-----------------------------------------------------------------------------

const QList<QProperty> CGameObject::dynamicProperties() const {
    QList<QProperty> dynamic_properties = QList<QProperty>();
    QList<QByteArray> prop_names = dynamicPropertyNames();
    QListIterator<QByteArray> it(prop_names);
    while (it.hasNext()) {
        QByteArray name = it.next();
        dynamic_properties.append(QProperty(QString(name), property(qPrintable(name))));
    }
    return dynamic_properties;
}

//-----------------------------------------------------------------------------

void CGameObject::setDynamicProperties(QList<QProperty> dynamic_properties) {
    QListIterator<QProperty> it(dynamic_properties);
    while (it.hasNext()) {
        QProperty property = it.next();
        setProperty(qPrintable(property.objectName()), property.value());
    }
}

//-----------------------------------------------------------------------------

int CGameObject::CObjectType() const {
    return m_CObjectType;
}

//-----------------------------------------------------------------------------

void CGameObject::setCObjectType(int val) {
    m_CObjectType = val;
}

//-----------------------------------------------------------------------------

const QString& CGameObject::CObjectTypeName() const {
    return CGameObjectFactory::getTypeNameHavingId(m_CObjectType);
}

//-----------------------------------------------------------------------------

void CGameObject::setCObjectTypeName(const QString& type_name) {
    m_CObjectType = CGameObjectFactory::getObjectTypeId(qPrintable(type_name));
}

//-----------------------------------------------------------------------------

unsigned int CGameObject::uid() const {
    return m_uid;
}

//-----------------------------------------------------------------------------

const bool CGameObject::bPhysicsEnabled() const {
    return bulletProperties.physicsEnabled();
}

//-----------------------------------------------------------------------------

void CGameObject::setbPhysicsEnabled(bool physics_enabled) {
    bulletProperties.setPhysicsEnabled(physics_enabled);
}

//-----------------------------------------------------------------------------

const float CGameObject::bMass() const {
    return bulletProperties.mass();
}

//-----------------------------------------------------------------------------

void CGameObject::setbMass(float mass) {
    bulletProperties.setMass(mass);
}

//-----------------------------------------------------------------------------

const float CGameObject::bRestitution() const {
    return bulletProperties.restitution();
}

//-----------------------------------------------------------------------------

void CGameObject::setbRestitution(float restitution) {
    bulletProperties.setRestitution(restitution);
}

//-----------------------------------------------------------------------------

const float CGameObject::bFriction() const {
    return bulletProperties.friction();
}

void CGameObject::setbFriction(float friction) {
    bulletProperties.setFriction(friction);
}

const btVector3& CGameObject::bLinearFactor() const {
    return bulletProperties.linearFactor();
}

//-----------------------------------------------------------------------------

void CGameObject::setbLinearFactor(const btVector3 &linear_factor) {
    bulletProperties.setLinearFactor(linear_factor);
}

//-----------------------------------------------------------------------------

const btVector3& CGameObject::bAngularFactor() const {
    return bulletProperties.angularFactor();
}

//-----------------------------------------------------------------------------

void CGameObject::setbAngularFactor(const btVector3 &angular_factor) {
    bulletProperties.setAngularFactor(angular_factor);
}

//-----------------------------------------------------------------------------

const CBulletProperties::ShapeTypes CGameObject::shapeType() const {
    return bulletProperties.shapeType();
}

//-----------------------------------------------------------------------------

void CGameObject::setShapeType(CBulletProperties::ShapeTypes shape_type) {
    bulletProperties.setShapeType(shape_type);
}

//-----------------------------------------------------------------------------

const float CGameObject::bShapeTypeRadius() const {
    return bulletProperties.radius();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypeRadius(float radius) {
    bulletProperties.setRadius(radius);
}

//-----------------------------------------------------------------------------

const float CGameObject::bShapeTypeHeight() const {
    return bulletProperties.height();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypeHeight(float height) {
    bulletProperties.setHeight(height);
}

//-----------------------------------------------------------------------------

const btVector3 CGameObject::bShapeTypeSize() const {
    return bulletProperties.size();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypeSize(btVector3 &size) {
    bulletProperties.setSize(size);
}

//-----------------------------------------------------------------------------

const QString CGameObject::bShapeTypeVertices() const {
    return bulletProperties.vertices();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypeVertices(QString vertices) {
    bulletProperties.setVertices(vertices);
}

//-----------------------------------------------------------------------------

const btVector3 CGameObject::bShapeTypePlaneNormal() const {
    return bulletProperties.planeNormal();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypePlaneNormal(btVector3 &plane_normal) {
    bulletProperties.setPlaneNormal(plane_normal);
}

//-----------------------------------------------------------------------------

const float CGameObject::bShapeTypePlaneConstant() const {
    return bulletProperties.planeConstant();
}

//-----------------------------------------------------------------------------

void CGameObject::setbShapeTypePlaneConstant(float plane_constant) {
    bulletProperties.setPlaneConstant(plane_constant);
}

//-----------------------------------------------------------------------------

const CBulletProperties::ActivationState CGameObject::activationState() const {
    return bulletProperties.activationState();
}

//-----------------------------------------------------------------------------

void CGameObject::setActivationState(CBulletProperties::ActivationState activation_state) {
    bulletProperties.setActivationState(activation_state);
}

//-----------------------------------------------------------------------------

btCollisionShape* CGameObject::collisionShape() {
    QVariant value = property("ShapeType_Size");
    btVector3 size = *static_cast<btVector3*>(value.data());
    btVector3 bt_size = btVector3(size.x(), size.y(), size.z());

    value = property("ShapeType_PlaneNormal");
    size = *static_cast<btVector3*>(value.data());
    btVector3 plane_normal = btVector3(size.x(), size.y(), size.z());

    float radius         = property("ShapeType_Radius").toFloat();
    float height         = property("ShapeType_Height").toFloat();
    float plane_constant = property("ShapeType_PlaneConstant").toFloat();

    QString vertices_str = property("ShapeType_Vertices").toString();
    std::vector<std::string> vertices_vec = StringHelper::split(vertices_str.toStdString(), " ");
    std::vector<float> fixed_vertices_vec;
    for (std::vector<std::string>::iterator it = vertices_vec.begin();
        it != vertices_vec.end();
        ++it) {
        char* p;
        float converted = strtol((*it).c_str(), &p, 10);
        if (*p) {
            fixed_vertices_vec.push_back(0);
        } else {
            fixed_vertices_vec.push_back(converted);
        }
    }
    int vertices_count = fixed_vertices_vec.size();
    btScalar* vertices = new btScalar[vertices_count];
    for (unsigned int i = 0; i < fixed_vertices_vec.size(); i++) {
        vertices[i] = fixed_vertices_vec[i];
    }

    btCollisionShape* collision_shape = NULL;
    switch (bulletProperties.shapeType()) {
    case CBulletProperties::Box:
        collision_shape = new btBoxShape(bt_size);
        break;
    case CBulletProperties::Sphere:
        collision_shape = new btSphereShape(radius);
        break;
    case CBulletProperties::StaticPlane:
        collision_shape = new btStaticPlaneShape(plane_normal, plane_constant);
        break;
    case CBulletProperties::Cone:
        collision_shape = new btConeShape(radius, height);
        break;
    case CBulletProperties::Capsule:
        collision_shape = new btCapsuleShape(radius, height);
        break;
    case CBulletProperties::Cylinder:
        collision_shape = new btCylinderShape(bt_size);
        break;
    case CBulletProperties::Convex:
        collision_shape  = new btConvexHullShape(vertices, vertices_count);
        break;
    case CBulletProperties::ConvexTriangle:
        // TODO
        break;
    }
    SAFE_RELEASE(vertices);

    return collision_shape;
}

//-----------------------------------------------------------------------------

CGameScene* CGameObject::parentScene() const {
    QObject* _parent = parent();
    RE_ASSERT(_parent);
    while (_parent) {
        bool is_game_scene = (qobject_cast<CGameScene*>(_parent) != NULL);
        if (is_game_scene) {
            break;
        }
        _parent = _parent->parent();
    }
    RE_ASSERT(_parent);

    return reinterpret_cast<CGameScene*>(_parent);
}

//-----------------------------------------------------------------------------

void CGameObject::start() {
}

//-----------------------------------------------------------------------------

void CGameObject::preUpdate() {
}

//-----------------------------------------------------------------------------

void CGameObject::update() {
}

//-----------------------------------------------------------------------------

void CGameObject::postWorldUpdate() {
}

//-----------------------------------------------------------------------------

void CGameObject::postUpdate() {
}

//-----------------------------------------------------------------------------

void CGameObject::runAction(CAction* action) {
    RE_ASSERT(action);
    m_actionManager->addAction(action, this, false);
}

//-----------------------------------------------------------------------------

void CGameObject::stopAllActions() {
    m_actionManager->removeAllActionsFromTarget(this);
}

//-----------------------------------------------------------------------------

void CGameObject::stopAction(CAction* action) {
    m_actionManager->removeAction(action);
}

//-----------------------------------------------------------------------------

void CGameObject::stopActionByTag(unsigned int a_tag) {
    //    RE_ASSERT( aTag != kCCActionTagInvalid, @"Invalid tag");
    m_actionManager->removeActionByTag(a_tag, this);
}

//-----------------------------------------------------------------------------

CAction* CGameObject::getActionByTag(unsigned int a_tag) {
    //    RE_ASSERT( aTag != kCCActionTagInvalid, @"Invalid tag");
    return m_actionManager->getActionByTag(a_tag, this);
}

//-----------------------------------------------------------------------------

unsigned int CGameObject::numberOfRunningActions() {
    return m_actionManager->numberOfRunningActionsInTarget(this);
}

//-----------------------------------------------------------------------------

void CGameObject::deployIntoDynamicsWorld() {
    if (bulletProperties.rigidBody()) {
        return;
    }
    RE_ASSERT(parent());

    CGameScene* parent_scene = parentScene();
    btCollisionShape* collision_shape = collisionShape();
    RE_ASSERT(collision_shape);

    btCustomTransform _transform = transform().global(false);
    _transform.getInnerTransform().setOrigin(_transform.getInnerTransform().getOrigin() * k_QGLGameEditorViewport_SizeFactor);

    btDefaultMotionState* motion_state = new btDefaultMotionState(_transform.getInnerTransform());
    btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(bulletProperties.mass(),
                                                           motion_state,
                                                           collision_shape);
    rigid_body_ci.m_restitution = property("Restitution").toFloat();
    rigid_body_ci.m_friction    = property("Friction").toFloat();
    btRigidBody* rigid_body = new btRigidBody(rigid_body_ci);
    rigid_body->setUserPointer(this);
    rigid_body->setActivationState(bulletProperties.activationState());
    parent_scene->dynamicsWorld()->addRigidBody(rigid_body);
    rigid_body->setWorldTransform(_transform.getInnerTransform());
    bulletProperties.setRigidBody(rigid_body);
    bulletProperties.setPhysicsEnabled(true);
}

//-----------------------------------------------------------------------------

void CGameObject::removeFromDynamicsWorld() {
    btRigidBody* body = bulletProperties.rigidBody();
    if (!body) {
        return;
    }
    RE_ASSERT(parent());

    CGameScene* parent_scene = parentScene();

    parent_scene->dynamicsWorld()->removeRigidBody(body);
    SAFE_RELEASE(body->getMotionState());
    SAFE_RELEASE(body->getCollisionShape());
    SAFE_RELEASE(body);
    bulletProperties.setRigidBody(NULL);
}

//-----------------------------------------------------------------------------

void CGameObject::updateBulletProperties() {
    btVector3 fixed_size = m_size * k_QGLGameEditorViewport_SizeFactor * 0.5f; // TODO too hacky
    float largest_axis = std::max(std::max(fixed_size.x(), fixed_size.y()), fixed_size.z());

    setbShapeTypeRadius(largest_axis);
    setbShapeTypeSize(fixed_size);
    setbShapeTypeHeight(fixed_size.y());
}

//-----------------------------------------------------------------------------

int CGameObject::tag() const {
    return m_tag;
}

//-----------------------------------------------------------------------------

void CGameObject::setTag(int tag) {
    m_tag = tag;
}

//-----------------------------------------------------------------------------

const btVector3& CGameObject::calcSize() {
    CLNode::calcSize();
    updateBulletProperties();
    return m_size;
}

//-----------------------------------------------------------------------------

void CGameObject::addChild(CGameObject* child) {
    RE_ASSERT(child);
    SAFE_RETAIN(child);
    child->setParent(this);
}

//-----------------------------------------------------------------------------

void CGameObject::onCollisionWithObject(CGameObject* object) {
}

//-----------------------------------------------------------------------------

void CGameObject::aboutToBeRemoved() {
}

//-----------------------------------------------------------------------------

void CGameObject::remove() {
    stopAllActions();
    parentScene()->removeGameObjectWhenAble(this);
}

//-----------------------------------------------------------------------------

CGameObject::~CGameObject(void) {
    removeFromDynamicsWorld();  // remove body from dynamics world (bullet physics)
    // remove children
    std::vector<CGameObject*>* gameobjs_to_remove = new std::vector<CGameObject*>();
    for (QList<QObject*>::const_iterator it = children().begin();
        it != children().end();
        ++it) {
        if (qobject_cast<CGameObject*>(*it) != NULL) {
            CGameObject* game_obj = static_cast<CGameObject*>(*it);
            game_obj->stopAllActions();
            //SAFE_RELEASE(game_obj); //  we can't release the object yet, otherwise it will crash
            gameobjs_to_remove->push_back(game_obj);
        }
    }
    GenericsHelper::deleteVector(gameobjs_to_remove);
}
