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

#include <string>
#include "misc/btcustom.h"
#include "misc/mathhelper.hpp"
#include "inputmanager/inputmanager.h"
#include "cengine/cactioninterval.h"
#include "cengine/ccamera.h"
#include "cengine/csoundplayer.h"
#include "cengine/cgamescene.h"
#include "cengine/cdirector.h"
#include "cengine/csingleton.h"
#include "cengine/cenemy.h"
#include "shootingrange/cplayer.h"

//-----------------------------------------------------------------------------

const std::string CPlayer::k_PlayerCameraId = "__PlayerCamera__";

//-----------------------------------------------------------------------------

CPlayer::CPlayer(void) : CAliveEntity() {
    m_weapon      = NULL;
    recoil_action = NULL;
    m_previousMousePosition = new USERINPUT::MousePosition();
    CDIRECTOR->createCamera(k_PlayerCameraId);
}

//-----------------------------------------------------------------------------

void CPlayer::start() {
    // let's create the weapon
    m_weapon = parentScene()->createGameObjectWithNode("AK-47", false);
    addChild(m_weapon);
    m_weapon->setRotation(0, -90.0f, 0);
    m_weapon->transform().setLocalPosition(1.0f, -0.5f,-1.2f);
    m_weapon->setScale(0.5f);
    CDIRECTOR->getCameraWithId(k_PlayerCameraId)->reset();
}

//-----------------------------------------------------------------------------

void CPlayer::update() {
    updatePositionAndOrientation();
}

//-----------------------------------------------------------------------------

void CPlayer::updatePositionAndOrientation() {
    CCamera*    players_camera      = CDIRECTOR->getCameraWithId(k_PlayerCameraId);
    Transform*  camera_transform    = &players_camera->transform();

    // -------------------- Joystick controls
    float left_joystick_y   = INPUTMANAGER->getLeftJoystick()->getY();
    float left_joystick_x   = INPUTMANAGER->getLeftJoystick()->getX();

    float right_joystick_y  = INPUTMANAGER->getRightJoystick()->getY();
    float right_joystick_x  = INPUTMANAGER->getRightJoystick()->getX();

    float rot_factor = 2.5f;

    left_joystick_y -= (INPUTMANAGER->getKeyDown('W') ? 1.0f : 0.0f);
    left_joystick_y += (INPUTMANAGER->getKeyDown('S') ? 1.0f : 0.0f);
    left_joystick_x -= (INPUTMANAGER->getKeyDown('A') ? 1.0f : 0.0f);
    left_joystick_x += (INPUTMANAGER->getKeyDown('D') ? 1.0f : 0.0f);

    const USERINPUT::MousePosition* current_mouse_position = INPUTMANAGER->getMousePosition();
    float dx = (float)(current_mouse_position->x - m_previousMousePosition->x) * k_InputManager_MouseSensitivity;
    float dy = (float)(current_mouse_position->y - m_previousMousePosition->y) * k_InputManager_MouseSensitivity;

    dx = MathHelper::clampf(dx, -5.0f, 5.0f);
    dy = MathHelper::clampf(dy, -5.0f, 5.0f);
    right_joystick_x += dx;
    right_joystick_y += dy * 0.7f; // y axis movement must be slower


    if (right_joystick_x != 0) {
        transform().rotateY(-right_joystick_x * rot_factor, Transform::LOCAL);
        camera_transform->rotateY(right_joystick_x* rot_factor);
    }

    if (right_joystick_y != 0) {
        transform().rotateX(-right_joystick_y  * rot_factor, Transform::LOCAL);
        camera_transform->rotateX(right_joystick_y* rot_factor);
    }

    if (left_joystick_y != 0) {
        transform().moveZ(-left_joystick_y * k_QGLGameEditorViewport_SizeFactor, Transform::LOCAL);
    }

    if (left_joystick_x != 0) {
        transform().moveX(left_joystick_x * k_QGLGameEditorViewport_SizeFactor, Transform::LOCAL);
    }

    // -------------------- Keyboard & Mouse controls

    if(nextShotReady() && (INPUTMANAGER->getJoystickButtonDown(6) || INPUTMANAGER->getMouseButtonDown(USERINPUT::LeftButton))) {
        fire();
    }

    m_previousMousePosition->x = current_mouse_position->x;
    m_previousMousePosition->y = current_mouse_position->y;
}

//-----------------------------------------------------------------------------

void CPlayer::postWorldUpdate() {
    transform().setLocalPosition(transform().localPosition().x(), 2.0f, transform().localPosition().z());
    CDIRECTOR->getCameraWithId(k_PlayerCameraId)->transform().setLocalPosition(transform().localPosition() * k_QGLGameEditorViewport_SizeFactor);
}

//-----------------------------------------------------------------------------

std::string CPlayer::getCameraId() const {
    return k_PlayerCameraId;
}

//-----------------------------------------------------------------------------

bool CPlayer::nextShotReady() {
    return (recoil_action == NULL);
}

//-----------------------------------------------------------------------------

void CPlayer::fire() {
    CMoveBy* recoil = new CMoveBy(0.04f, btVector3(0, 0, 0.1f));
    recoil->autorelease();
    CMoveBy* reversed_recoil = static_cast<CMoveBy*>(recoil->reverse());
    reversed_recoil->autorelease();
    recoil_action = new CSequence();
    recoil_action->autorelease();
    recoil_action->initHavingActions(recoil, reversed_recoil);
    connect(recoil_action, SIGNAL(onActionDone(CAction*)),
            this, SLOT(onActionDone(CAction*)));
    m_weapon->runAction(recoil_action);
    CSOUNDPLAYER->play2D("ak47show.wav");

    // let's create the bullet
    CGameObject* bullet = parentScene()->createGameObjectWithNode("bullet", true, "CBullet");
    bullet->transform().setLocalPosition(transform().localPosition());
    bullet->transform().setLocalRotation(transform().localRotation() * btQuaternion(0, DEGTORAD(180.0f), 0));
    bullet->transform().setLocalScale(0.2f);
    bullet->bulletProperties.setMass(0.1f);
    bullet->deployIntoDynamicsWorld();
    bullet->bulletProperties.rigidBody()->setGravity(btCustom::zero_vector());
    bullet->bulletProperties.rigidBody()->activate(true);
    bullet->setPosition(bullet->position());
    bullet->transform().moveZ(-6.0f, Transform::TransformMode::LOCAL);
    bullet->transform().moveY(0.5f, Transform::TransformMode::LOCAL);

    // raycast
    btVector3 forward = btMatrix3x3(transform().localRotation()) * btVector3(0,0,-100.0f);
    btVector3 start   = position(Transform::UnitType::PHYSICSWORLD);
    btVector3 end     = start + forward;

    btCollisionWorld::ClosestRayResultCallback RayCallback( start, end );
    parentScene()->dynamicsWorld()->rayTest(start, end, RayCallback);

    if(RayCallback.hasHit()) {
        CGameObject* game_obj = static_cast<CGameObject*>(RayCallback.m_collisionObject->getUserPointer());
        LOG("Bullet hit object named: %s", qPrintable(game_obj->objectName()));
        if(game_obj) {
            CEnemy* enemy = qobject_cast<CEnemy*>(game_obj);
            bool is_enemy = enemy != NULL;
            if (is_enemy) {
                enemy->damageReceived(1);
            }
        }
    }

    btVector3 centerOfMass = bullet->bulletProperties.rigidBody()->getCenterOfMassPosition();
    btVector3 force = btMatrix3x3(transform().localRotation()) * btVector3(0,0,-5.0f);
    btVector3 hitPoint = RayCallback.m_hitPointWorld;
    bullet->bulletProperties.rigidBody()->applyImpulse(force * 4.0f, hitPoint - centerOfMass);
}

//-----------------------------------------------------------------------------

void CPlayer::onActionDone(CAction* action) {
    recoil_action = NULL;
}

//-----------------------------------------------------------------------------

CPlayer::~CPlayer(void) {
    SAFE_RELEASE(m_weapon);
    SAFE_RELEASE(m_previousMousePosition);
}
