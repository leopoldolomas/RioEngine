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

#ifndef RIOENGINE_CENGINE_CGAMEOBJECT_H_
#define RIOENGINE_CENGINE_CGAMEOBJECT_H_

#include <QString>
#include "misc/transform.h"
#include "clengine/clnode.h"
#include "editor/qpropertyeditor/customtypes.h"
#include "editor/qproperty/qproperty.h"
#include "cengine/cbulletproperties.h"
#include "bullet/btbulletdynamicscommon.h"
#include "bullet/btbulletcollisioncommon.h"

class CActionManager;
class CAction;
class CGameScene;

class CGameObject : public CLNode {
    Q_OBJECT
    Q_PROPERTY(QList<QProperty> DynamicProperties READ dynamicProperties WRITE setDynamicProperties DESIGNABLE false)
    Q_PROPERTY(QString      ScriptName       READ scriptName         WRITE setScriptName                            )
    Q_PROPERTY(int          CObjectType      READ CObjectType        WRITE setCObjectType                           )
    Q_PROPERTY(int          Tag              READ tag                WRITE setTag                                   )
    Q_CLASSINFO("CObjectType", "CustomEnum=CClasses") // TODO improve this
    Q_PROPERTY(btVector3    Position         READ position           WRITE setPosition)
    Q_PROPERTY(btQuaternion RotationQuat     READ rotationQuat       WRITE setRotation          DESIGNABLE false    )
    Q_PROPERTY(btVector3    Rotation         READ rotation           WRITE setRotation          STORED     false    )
    Q_PROPERTY(btVector3    Scale            READ scale              WRITE setScale                                 )
    Q_PROPERTY(QString      CObjectTypeName  READ CObjectTypeName    WRITE setCObjectTypeName   DESIGNABLE false    )

    // bullet physics properties
    Q_PROPERTY(CBulletProperties::ActivationState ActivationState READ activationState WRITE setActivationState     )
    Q_PROPERTY(bool PhysicsEnabled              READ bPhysicsEnabled            WRITE setbPhysicsEnabled            )
    Q_PROPERTY(float Mass                       READ bMass                      WRITE setbMass                      )
    Q_PROPERTY(float Restitution                READ bRestitution               WRITE setbRestitution               )
    Q_PROPERTY(float Friction                   READ bFriction                  WRITE setbFriction                  )
    Q_PROPERTY(btVector3 LinearFactor           READ bLinearFactor              WRITE setbLinearFactor              )
    Q_PROPERTY(btVector3 AngularFactor          READ bAngularFactor             WRITE setbAngularFactor             )
    Q_PROPERTY(CBulletProperties::ShapeTypes ShapeType READ shapeType           WRITE setShapeType                  )
    Q_PROPERTY(float ShapeType_Radius           READ bShapeTypeRadius           WRITE setbShapeTypeRadius           )
    Q_PROPERTY(float ShapeType_Height           READ bShapeTypeHeight           WRITE setbShapeTypeHeight           )
    Q_PROPERTY(btVector3    ShapeType_Size      READ bShapeTypeSize             WRITE setbShapeTypeSize             )
    Q_PROPERTY(QString ShapeType_Vertices       READ bShapeTypeVertices         WRITE setbShapeTypeVertices         )
    Q_PROPERTY(btVector3 ShapeType_PlaneNormal  READ bShapeTypePlaneNormal      WRITE setbShapeTypePlaneNormal      )
    Q_PROPERTY(float ShapeType_PlaneConstant    READ bShapeTypePlaneConstant    WRITE setbShapeTypePlaneConstant    )
    // -----------------

public:
    CGameObject();
    CGameObject(const CGameObject& game_object);
    const CGameObject& operator=(const CGameObject& game_object);

    QString scriptName() const;
    void setScriptName(const QString &scriptName);

    int CObjectType() const;
    void setCObjectType(int type);

    const QString& CObjectTypeName() const;
    void setCObjectTypeName(const QString& name);

    void setName(const std::string& name);
    std::string name() const;

    int tag() const;
    void setTag(int tag);

    const bool bPhysicsEnabled() const;
    void setbPhysicsEnabled(bool enable_physics);

    const float bMass() const;
    void setbMass(float mass);

    const float bRestitution() const;
    void setbRestitution(float restitution);

    const float bFriction() const;
    void setbFriction(float friction);

    const btVector3& bLinearFactor() const;
    void setbLinearFactor(const btVector3& linear_factor);

    const btVector3& bAngularFactor() const;
    void setbAngularFactor(const btVector3& angular_factor);

    const CBulletProperties::ShapeTypes shapeType() const;
    void setShapeType(CBulletProperties::ShapeTypes shape_type);

    const CBulletProperties::ActivationState activationState() const;
    void setActivationState(CBulletProperties::ActivationState activation_state);

    void setActionManager(CActionManager* action_manager);

    CActionManager*   actionManager() const;
    CGameScene*       parentScene()   const;
    btCollisionShape* collisionShape();

    /**
     * Returns the object's unique id
     */
    unsigned int uid() const;

    void addChild(CGameObject* child);

    /**
     * Method used to remove the game object from the parent game scene.
     * Don't release the game object unless you really know what you are
     * doing.
     */
    virtual void remove();

    void deployIntoDynamicsWorld();
    void removeFromDynamicsWorld();

    /**
     * Called once per game object.
     */
    virtual void start();

    /**
     * Called every frame before updating every game object in the game scene.
     */
    virtual void preUpdate();

    /**
     * Called every frame. This is the most common method to implement any behaviour.
     */
    virtual void update();

    virtual void postWorldUpdate();
    virtual void postUpdate();

    /**
     * Called when the game object collides with another object.
     */
    virtual void onCollisionWithObject(CGameObject* object);

    /**
     * Called before the game object is removed from the game scene.
     */
    virtual void aboutToBeRemoved();

    /**
     * Calculates the object's bounding box size.
     */
    virtual const btVector3& calcSize();

    // -------------- Custom setters and getters for Q_PROPERTYs
    const btVector3 position(Transform::UnitType unit_type = Transform::UnitType::WORLD);
    void setPosition(const btVector3 &value);
    void setPosition(float x, float y, float z);

    const btVector3 scale();
    void setScale(btVector3 &scale);
    void setScale(float x, float y, float z);
    void setScale(float scale);

    btVector3 rotation();
    btQuaternion rotationQuat();
    void setRotation(const btVector3& rotation);
    void setRotation(const btQuaternion& rotation);
    void setRotation(float x, float y, float z);
    void setRotation(float w, float x, float y, float z);

    const QList<QProperty> dynamicProperties() const;
    void setDynamicProperties(QList<QProperty> dynamic_properties);

    const float bShapeTypeRadius() const;
    void setbShapeTypeRadius(float radius);

    const float bShapeTypeHeight() const;
    void setbShapeTypeHeight(float height);

    const btVector3 bShapeTypeSize() const;
    void setbShapeTypeSize(btVector3& size);

    const QString bShapeTypeVertices() const;
    void setbShapeTypeVertices(QString vertices);

    const btVector3 bShapeTypePlaneNormal() const;
    void setbShapeTypePlaneNormal(btVector3& normal);

    const float bShapeTypePlaneConstant() const;
    void setbShapeTypePlaneConstant(float plane_constant);

    // actions

    /**
     * Runs the specified action. The game object becomes the action's target.
     */
    void runAction(CAction* action);

    /**
     * Removes all actions from the running action list
     */
    void stopAllActions();

    /**
     * Removes the action from the running action list
     */
    void stopAction(CAction* action);

    /**
     * Removes an action from the running action list given its tag
     */
    void stopActionByTag(unsigned int tag);

    /**
     * Gets an action from the running action list given its tag
     * @return the Action the with the given tag
     */
    CAction* getActionByTag(unsigned int tag);

    /**
     * Returns the numbers of actions that are running plus the ones that are schedule to run (actions in actionsToAdd and actions arrays).
     * Composable actions are counted as 1 action. Example:
     *    If you are running 1 Sequence of 7 actions, it will return 1.
     *    If you are running 7 Sequences of 2 actions, it will return 7.
     */
    unsigned int numberOfRunningActions();

    ~CGameObject();

    CBulletProperties bulletProperties; // TODO create setter & getter

protected:
    virtual void copyValuesFromObject(const CGameObject& game_object);
    virtual void __init();

    CActionManager* m_actionManager;
    int             m_tag;

private:
    void updateBulletProperties();

    int  m_CObjectType;
    unsigned int m_uid;

    QString m_scriptName;
};
Q_DECLARE_METATYPE(CGameObject)

#endif  // RIOENGINE_CENGINE_CGAMEOBJECT_H_
