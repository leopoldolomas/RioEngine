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

#ifndef RIOENGINE_CENGINE_CGAMESCENE_H_
#define RIOENGINE_CENGINE_CGAMESCENE_H_

#include <vector>
#include <string>
#include "cengine/cobject.h"
#include "cengine/cgameobject.h"

class CGameScene : public CObject {
    Q_OBJECT
    Q_PROPERTY(QList<CGameObject> GameObjects READ gameObjects WRITE setGameObjects) /// used to serialize the GameScene

public:
    CGameScene();
    CGameScene(const CGameScene& game_scene);
    CGameScene& operator=(const CGameScene& game_scene);
    virtual void copyValuesFromObject(const CGameScene& game_scene);

    /**
     * @brief Creates a GameObject based upon the specified Node id
     * @param node_id The Node id
     * @param add_to_scene Whether the GameObject must be immediately added to the GameScene
     * @param object_type (optional) A special type can be specified for the
     * GameObject (it must inherit from CGameObject)
     */
    CGameObject* createGameObjectWithNode(const std::string& node_id, bool add_to_scene = true, const char* object_type = NULL);

    /**
     * Adds the GameObject to the GameScene
     */
    void addGameObject(CGameObject* game_object);

    /**
     * Returns the GameObject with the specified name
     */
    const CGameObject* getGameObject(const std::string& name) const;

    /**
     * Returns the GameObject with the specified uid
     */
    const CGameObject* getGameObject(uint uid) const;

    std::vector<CGameObject*>* getGameObjects() const;

    /**
     * Returns the GameObject with the specified tag
     */
    CGameObject* getGameObjectHavingTag(int tag) const;

    /**
     * Indicates whether a GameObject with the specified name already exists
     */
    bool existsGameObjectWithName(const std::string& name);

    /**
     * Returns a duplicate of the GameObject
     */
    CGameObject* duplicateGameObject(const CGameObject* game_object);

    /**
     * Immediately removes the GameObject with the specified uid
     * Note: You should call this method only if the GameScene is NOT running
     * (e.g. from the Editor).
     */
    bool removeGameObject(uint uid);

    /**
     * Once the GameScene is running you should remove GameObjects by calling
     * this method.
     */
    void removeGameObjectWhenAble(CGameObject* game_object);

    /**
     * Clears the GameScene
     */
    virtual void clear();

    /**
     * Runs the GameScene
     */
    virtual void run();

    /**
     * Calls preUpdate() on every CGameObject in the GameScene
     */
    virtual void preUpdate();

    /**
     * Calls update() on every CGameObject in the GameScene
     */
    virtual void update();

    /**
     * Calls postUpdate() on every CGameObject in the GameScene
     */
    virtual void postUpdate();

    /**
     * Calls stepSimulation() on btDiscreteDynamicsWorld and then updates all
     * CGameObject Transforms so they match their btRigidBody Transform.
     */
    virtual void updateWorld();

    /**
     * Calls postUpdateWorld() on every CGameObject in the GameScene
     */
    virtual void postUpdateWorld();

    virtual void deleteWorld();

    virtual void performCollisionCallbacks();

    QList<CGameObject> gameObjects();
    void setGameObjects(QList<CGameObject> game_objects);

    btDiscreteDynamicsWorld* dynamicsWorld() const;

    virtual ~CGameScene();

protected:
    std::string& createUniqueGameObjectName(std::string& original_name);

    virtual void start();

    /**
     * Stops all CActions
     */
    virtual void stopAllActions();

    bool m_isRunningUpdateLoops;

    std::vector<CGameObject*>* m_gameObjects;

    /// stores GameObjects added during the update loop
    std::vector<CGameObject*>* m_gameObjectsPendingToBeAdded;

    /// stores GameObjects removed during the update loop
    std::vector<CGameObject*>* m_gameObjectsPendingToBeRemoved;

    // ------------------ bullet physics
    btAlignedObjectArray<btCollisionShape*>    m_collisionShapes;
    btDefaultCollisionConfiguration*           m_collisionConfiguration;
    btCollisionDispatcher*                     m_dispatcher;
    btBroadphaseInterface*                     m_overlappingPairCache;
    btSequentialImpulseConstraintSolver*       m_solver;
    btDiscreteDynamicsWorld*                   m_dynamicsWorld;

private:
    std::string& fixName(std::string& name);

    /**
     * Calls start() on the GameObject and on all of its children
     */
    void callStart_R(CGameObject* child);

    /**
     * Calls preUpdate() on the GameObject and on all of its children
     */
    void callPreUpdate_R(CGameObject* child);

    /**
     * Calls update() on the GameObject and on all of its children
     */
    void callUpdate_R(CGameObject* child);

    /**
     * Calls postUpdate() on the GameObject and on all of its children
     */
    void callPostUpdate_R(CGameObject* child);
};

#endif  // RIOENGINE_CENGINE_CGAMESCENE_H_
