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

#include <regex>
#include "constants.h"
#include "misc/transform.h"
#include "misc/genericshelper.hpp"
#include "cengine/cgameobject.h"
#include "clengine/classetsrepository.h"
#include "cengine/cgameobjectfactory.h"
#include "cengine/cdirector.h"
#include "cengine/ccamera.h"
#include "cengine/cgamescene.h"

//-----------------------------------------------------------------------------

CGameScene::CGameScene() : CObject() {
    m_gameObjects                   = new std::vector<CGameObject*>();
    m_gameObjectsPendingToBeAdded   = new std::vector<CGameObject*>();
    m_gameObjectsPendingToBeRemoved = new std::vector<CGameObject*>();

    m_collisionShapes               = btAlignedObjectArray<btCollisionShape*>();
    m_collisionConfiguration        = new btDefaultCollisionConfiguration();
    m_dispatcher                    = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache          = new btDbvtBroadphase();
    m_solver                        = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld                 = new btDiscreteDynamicsWorld(m_dispatcher,
                                                                  m_overlappingPairCache,
                                                                  m_solver,
                                                                  m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
    m_isRunningUpdateLoops = false;
}

//-----------------------------------------------------------------------------

CGameScene::CGameScene(const CGameScene& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

CGameScene& CGameScene::operator=(const CGameScene& original_obj) {
    if (this == &original_obj)
        return *this;

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CGameScene::copyValuesFromObject(const CGameScene& original_obj) {
    // TODO
}

//-----------------------------------------------------------------------------

btDiscreteDynamicsWorld* CGameScene::dynamicsWorld() const {
    return m_dynamicsWorld;
}

//-----------------------------------------------------------------------------

void CGameScene::callStart_R(CGameObject* child) {
    child->start();
    for(QObjectList::const_iterator it = child->children().begin();
        it != child->children().end();
        ++it) {
        CGameObject* child_as_gameobject = qobject_cast<CGameObject*>(*it);
        if(child_as_gameobject != NULL) {
            callStart_R(child_as_gameobject);
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::callPreUpdate_R(CGameObject* child) {
    child->preUpdate();
    for(QObjectList::const_iterator it = child->children().begin();
        it != child->children().end();
        ++it) {
        CGameObject* child_as_gameobject = qobject_cast<CGameObject*>(*it);
        if(child_as_gameobject != NULL) {
            callPreUpdate_R(child_as_gameobject);
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::callUpdate_R(CGameObject* child) {
    child->update();
    for(QObjectList::const_iterator it = child->children().begin();
        it != child->children().end();
        ++it) {
        CGameObject* child_as_gameobject = qobject_cast<CGameObject*>(*it);
        if(child_as_gameobject != NULL) {
            callUpdate_R(child_as_gameobject);
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::callPostUpdate_R(CGameObject* child) {
    child->postUpdate();
    for(QObjectList::const_iterator it = child->children().begin();
        it != child->children().end();
        ++it) {
        CGameObject* child_as_gameobject = qobject_cast<CGameObject*>(*it);
        if(child_as_gameobject != NULL) {
            callPostUpdate_R(child_as_gameobject);
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::start() {    
    m_isRunningUpdateLoops = true;
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        callStart_R(*it);
    }
}

//-----------------------------------------------------------------------------

void CGameScene::preUpdate() {
    m_isRunningUpdateLoops = true;
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        callPreUpdate_R(*it);
    }
}

//-----------------------------------------------------------------------------

void CGameScene::update() {    
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        callUpdate_R(*it);
    }

    updateWorld();
    postUpdateWorld();
    performCollisionCallbacks();
}

//-----------------------------------------------------------------------------

void CGameScene::postUpdate() {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        callPostUpdate_R(*it);
    }
    m_isRunningUpdateLoops = false;

    //  move objects from m_queuedGameObject to m_gameObjects
    for (std::vector<CGameObject*>::iterator it = m_gameObjectsPendingToBeAdded->begin();
         it != m_gameObjectsPendingToBeAdded->end();
         ++it) {
        SAFE_RETAIN(*it);
        m_gameObjects->push_back(*it);
    }
    GenericsHelper::deleteVector(m_gameObjectsPendingToBeAdded);
    m_gameObjectsPendingToBeAdded = new std::vector<CGameObject*>();

    // delete the objects that are pending to be removed
    for (std::vector<CGameObject*>::iterator it = m_gameObjectsPendingToBeRemoved->begin();
         it != m_gameObjectsPendingToBeRemoved->end();
         ++it) {
        removeGameObject((*it)->uid());
    }
    GenericsHelper::deleteVector(m_gameObjectsPendingToBeRemoved);
    m_gameObjectsPendingToBeRemoved = new std::vector<CGameObject*>();
}

//-----------------------------------------------------------------------------

CGameObject* CGameScene::createGameObjectWithNode(const std::string& node_id, bool add_to_scene, const char* object_type) {
    if (CLASSETSREPOSITORY->getNodeHavingId(node_id)) {
        CLNode* original_node = CLASSETSREPOSITORY->getNodeHavingId(node_id);
        CLNode* node_copy = new CLNode(*original_node);

        CGameObject* game_object = NULL;
        if(object_type) {
            game_object = CGameObjectFactory::createInstance(object_type);
            SAFE_RETAIN(game_object);
        } else {
            game_object = new CGameObject();
        }
        RE_ASSERT(game_object);

        game_object->copyNodeValuesFromNode(node_copy);
        SAFE_RELEASE(node_copy);
        std::string fixed_node_id = node_id;
        game_object->setProperty("objectName", QString::fromStdString(createUniqueGameObjectName(fixed_node_id)));

        if (add_to_scene) {
            addGameObject(game_object);
        }
        game_object->autorelease();
        return game_object;
    }
    return NULL;
}

//-----------------------------------------------------------------------------

void CGameScene::addGameObject(CGameObject* game_object) {
    SAFE_RETAIN(game_object);
    game_object->setParent(this);
    if(m_isRunningUpdateLoops) {
        m_gameObjectsPendingToBeAdded->push_back(game_object);
    } else {
        m_gameObjects->push_back(game_object);
    }
    if(game_object->bulletProperties.physicsEnabled()) {
        game_object->deployIntoDynamicsWorld();
    }
    callStart_R(game_object);
}

//-----------------------------------------------------------------------------

const CGameObject* CGameScene::getGameObject(const std::string& name) const {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* game_obj = *it;
        if (game_obj->objectName().toStdString() == name) {
            return game_obj;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------

const CGameObject* CGameScene::getGameObject(uint uid) const {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* game_obj = *it;
        if (game_obj->uid() == uid) {
            return game_obj;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------

std::vector<CGameObject*>* CGameScene::getGameObjects() const {
    return m_gameObjects;
}

//-----------------------------------------------------------------------------

bool CGameScene::existsGameObjectWithName(const std::string& name) {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        if ((*it)->name() == name) {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------

std::string& CGameScene::fixName(std::string& name) {
    std::string regex_ext_str = "_([0-9]*)";
    std::tr1::regex regex_ext(regex_ext_str);
    std::tr1::cmatch match;
    if (regex_search(name.c_str(), match, regex_ext)) {
        std::string match_str = match.str().substr(1, match.str().size() - 1);  // remove the '_' char
        unsigned int index = atoi(match_str.c_str());

        name = name.substr(0, name.size() - match_str.length() - 1);

        char suffix[4];  // up to '_' char and a maximum of 3 digits
        sprintf_s(suffix, "_%i", ++index);

        name.append(suffix);
    } else  // then we just need to add a suffix, for example: my_game_object => my_game_object_1
        name.append("_1");

    return name;
}

//-----------------------------------------------------------------------------

std::string& CGameScene::createUniqueGameObjectName(std::string& name) {
    while (existsGameObjectWithName(name)) {
        name = fixName(name);
    }
    return name;
}

//-----------------------------------------------------------------------------

CGameObject* CGameScene::duplicateGameObject(const CGameObject * game_object) {
    CGameObject* clone = new CGameObject(*game_object);
    clone->setName(createUniqueGameObjectName(game_object->name()));
    addGameObject(clone);
    SAFE_RELEASE(clone);

    return clone;
}

//-----------------------------------------------------------------------------

bool CGameScene::removeGameObject(uint uid) {
    // retain the game object, it will be deleted until the undo/redo action goes out of scope
    CGameObject* game_object = NULL;
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        if ((*it)->uid() == uid) {
            game_object = (*it);
            break;
        }
    }
    if (game_object == NULL) {
        return false;
    }

    game_object->aboutToBeRemoved();
    game_object->removeFromDynamicsWorld();
    game_object->retain();
    game_object->setParent(NULL);
    return GenericsHelper::deleteObjectFromVector(m_gameObjects, game_object);
}

//-----------------------------------------------------------------------------

void CGameScene::removeGameObjectWhenAble(CGameObject *object) {
    RE_ASSERT(object);

    if(!GenericsHelper::vectorContainsObject(m_gameObjectsPendingToBeRemoved, object)) {
        m_gameObjectsPendingToBeRemoved->push_back(object);
    }
}

//-----------------------------------------------------------------------------

void CGameScene::clear() {
    stopAllActions();
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* obj = *it;
        obj->aboutToBeRemoved();
    }
    GenericsHelper::deleteVector(m_gameObjects);
    m_gameObjects = new std::vector<CGameObject*>();
    CDIRECTOR->getCameraWithId(k_CDirector_DefaultCameraId)->reset();
}

//-----------------------------------------------------------------------------

QList<CGameObject> CGameScene::gameObjects() {
    QList<CGameObject> vec = QList<CGameObject>();
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject obj = *(*it);
        vec.append(obj);
    }
    return vec;
}

//-----------------------------------------------------------------------------

void CGameScene::setGameObjects(QList<CGameObject> vec) {
    clear();
    for (int i = 0; i < vec.count(); i++) {
        CGameObject* gameobj_copy = new CGameObject(vec[i]);
        gameobj_copy->autorelease();
        addGameObject(gameobj_copy);
    }
}

//-----------------------------------------------------------------------------

void CGameScene::run() {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* obj = (*it);

        int type_id = QMetaType::type(obj->CObjectTypeName().toStdString().c_str());
        RE_ASSERT(type_id);
        CGameObject *new_obj = NULL;
        new_obj = static_cast<CGameObject*>(QMetaType::create(type_id, obj));
        new_obj->setParent(this);
        new_obj->bulletProperties = obj->bulletProperties;
        GenericsHelper::replaceObjectInVector(m_gameObjects, obj, new_obj);
        if(new_obj->bulletProperties.physicsEnabled()) {
            new_obj->deployIntoDynamicsWorld();
        }
        SAFE_RELEASE(new_obj);
    }
    start();
}

//-----------------------------------------------------------------------------

void CGameScene::updateWorld() {
    BT_PROFILE("update");
    m_dynamicsWorld->stepSimulation (1.f/60.f,10);
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* obj = *it;
        if (obj->bulletProperties.physicsEnabled()) {
            btRigidBody* body = obj->bulletProperties.rigidBody();
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);
            obj->transform().setLocalPosition(trans.getOrigin() / k_QGLGameEditorViewport_SizeFactor);
            obj->transform().setLocalRotation(trans.getRotation());
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::postUpdateWorld() {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        (*it)->postWorldUpdate();
    }
}

//-----------------------------------------------------------------------------

void CGameScene::deleteWorld() {
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ;i--) {
        btCollisionObject* obj = m_dynamicsWorld ->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body ->getMotionState()) {
            SAFE_RELEASE(body->getMotionState());
            SAFE_RELEASE(body->getCollisionShape());
        }
        m_dynamicsWorld ->removeCollisionObject( obj );
        SAFE_RELEASE(obj);
    }

    //delete collision shapes
    for (int j=0;j<m_collisionShapes.size();j++) {
        btCollisionShape* shape = m_collisionShapes[j];
        m_collisionShapes[j] = 0;
        SAFE_RELEASE(shape);
    }

    //delete dynamics world
    SAFE_RELEASE(m_dynamicsWorld);

    //delete solver
    SAFE_RELEASE(m_solver);

    //delete broadphase
    SAFE_RELEASE(m_overlappingPairCache);

    //delete dispatcher
    SAFE_RELEASE(m_dispatcher);

    SAFE_RELEASE(m_collisionConfiguration);

    m_collisionShapes.clear();
}

void CGameScene::stopAllActions() {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        (*it)->stopAllActions();
        for (QList<QObject*>::const_iterator inner_it = (*it)->children().begin();
             inner_it != (*it)->children().end();
             ++inner_it) {
            if (qobject_cast<CGameObject*>(*inner_it) != NULL) {
                static_cast<CGameObject*>(*inner_it)->stopAllActions();
            }
        }
    }
}

//-----------------------------------------------------------------------------

void CGameScene::performCollisionCallbacks() {
    int numManifolds = dynamicsWorld()->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++)
    {
        btPersistentManifold* contactManifold =  dynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        int numContacts = contactManifold->getNumContacts();
        for (int j=0;j<numContacts;j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                CGameObject* objectA = static_cast<CGameObject*>(obA->getUserPointer());
                CGameObject* objectB = static_cast<CGameObject*>(obB->getUserPointer());

                RE_ASSERT(objectA->isValidCObject());
                RE_ASSERT(objectB->isValidCObject());

                objectA->onCollisionWithObject(objectB);
                objectB->onCollisionWithObject(objectA);
            }
        }
    }
}

//-----------------------------------------------------------------------------

CGameObject* CGameScene::getGameObjectHavingTag(int tag) const {
    for (std::vector<CGameObject*>::iterator it = m_gameObjects->begin();
         it != m_gameObjects->end();
         ++it) {
        CGameObject* game_obj = *it;
        if (game_obj->tag() == tag) {
            return game_obj;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------

CGameScene::~CGameScene() {
    deleteWorld();

    // rigid bodies were deleted in deleteWorld(). So we need to set their references in bulletProperties to NULL so they don't get overreleased.
    for (uint i = 0; i < m_gameObjects->size(); i++) {
        CGameObject* obj = m_gameObjects->at(i);
        obj->bulletProperties.setRigidBody(NULL);
    }

    GenericsHelper::deleteVector(m_gameObjects);
    GenericsHelper::deleteVector(m_gameObjectsPendingToBeAdded);
    GenericsHelper::deleteVector(m_gameObjectsPendingToBeRemoved);
}
