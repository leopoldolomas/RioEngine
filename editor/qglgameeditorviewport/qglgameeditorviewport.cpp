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

#include <QtOpenGL/QtOpenGL>
#include <math.h>
#include "constants.h"
#include "misc/transform.h"
#include "misc/matrixstack.h"
#include "inputmanager/inputmanager.h"
#include "editor/undoengine/customundoableactions.h"
#include "editor/undoengine/undoableactionsmanager.h"
#include "cengine/ccamera.h"
#include "cengine/cdirector.h"
#include "clengine/clscene.h"
#include "clengine/clrenderer.h"
#include "editor/qglgameeditorviewport/qglgameeditorviewport.h"

//-----------------------------------------------------------------------------

// WARNING! This file has lots of leftovers, I am aware of that, I really don't
// have time to code better solutions anymore. 
// Continue reading at your own risk :B

//-----------------------------------------------------------------------------

QGLGameEditorViewport::QGLGameEditorViewport(CLScene* collada_scene, CGameScene* game_scene) : QGLGameViewport(collada_scene, NULL) {
    m_selectedAxis = NOAXIS;
    m_selectedManipulatorType = NOMANIPULATOR;
    m_selectedTransformMode = Transform::GLOBAL;
    m_drawMode = TEXTURED;
    m_selectedGameObject = NULL;
    m_selectedObjectUID = 0;
    m_previousActiveCameraId = "";
    m_cameraSpeed = 1;
    m_autoRecalcAABBOnResize = true;
    m_isFirstStep = true;

    CDIRECTOR->createCamera(k_QGLGameEditorViewport_CameraId);
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::setSelectedGameObjectUID(uint gameobj_uid) {
    m_selectedObjectUID = gameobj_uid;
}

//-----------------------------------------------------------------------------

uint QGLGameEditorViewport::selectedGameObjectUID() const {
    return m_selectedObjectUID;
}

//-----------------------------------------------------------------------------

int QGLGameEditorViewport::cameraSpeed() const {
    return m_cameraSpeed;
}

//-----------------------------------------------------------------------------

int QGLGameEditorViewport::nextCameraSpeed() {
    if (++m_cameraSpeed == 4) {
        m_cameraSpeed = 1;
    }
    return m_cameraSpeed;
}

//-----------------------------------------------------------------------------

bool QGLGameEditorViewport::autoRecalcAABBOnResize() const {
    return m_autoRecalcAABBOnResize;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::setAutoRecalcAABBOnResize(bool v) {
    m_autoRecalcAABBOnResize = v;
}

//-----------------------------------------------------------------------------

Transform::TransformMode QGLGameEditorViewport::selectedTransformMode() const {
    return m_selectedTransformMode;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::setSelectedTransformMode(const Transform::TransformMode &value) {
    m_selectedTransformMode = value;
}

//-----------------------------------------------------------------------------

QGLGameEditorViewport::ManipulatorTypes QGLGameEditorViewport::selectedManipulatorType() const {
    return m_selectedManipulatorType;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::setSelectedManipulatorType(const ManipulatorTypes &value) {
    m_selectedManipulatorType = value;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::paintGL() {
    if(CDIRECTOR->gameState() != CDirector::RUNNING) {
        QGLGameViewport::paintGL();
    } else {
        qglClearColor(Qt::black);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::preDraw() {
    QGLGameViewport::preDraw();
    m_selectedGameObject = NULL;

    if (CDIRECTOR->getActiveCameraId() != m_previousActiveCameraId) {
        m_previousActiveCameraId = CDIRECTOR->getActiveCameraId();
    }

    CCamera* active_camera = CDIRECTOR->setActiveCameraHavingId(k_QGLGameEditorViewport_CameraId);
    m_selectedProgram = m_drawMode == TEXTURED ? m_texturedProgram : m_solidColorsProgram;
    m_selectedProgram->bind();

    if (m_drawMode == SOLIDCOLORS) {
        m_nodesColor = QMap<uint, QColor>();
        m_selectedObjectUID = 0;
        m_colorProvider.reset();
        m_clearColor = Qt::black;
    }

    if (hasFocus()) {
        float camera_speed = 0.0f;
        switch (m_cameraSpeed) {
        case 1: camera_speed = k_QGLGameEditorViewport_CameraSpeed_Level1; break;
        case 2: camera_speed = k_QGLGameEditorViewport_CameraSpeed_Level2; break;
        case 3: camera_speed = k_QGLGameEditorViewport_CameraSpeed_Level3; break;
        default:
            camera_speed = k_QGLGameEditorViewport_CameraSpeed_Level1;
        }

        if (INPUTMANAGER->getWheelVelocity() > 0.0f) {
            active_camera->transform().moveZ(k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed * 7.0f, Transform::LOCAL);
        } else if (INPUTMANAGER->getWheelVelocity() < 0.0f) {
            active_camera->transform().moveZ(-k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed * 7.0f, Transform::LOCAL);
        }

        if (INPUTMANAGER->getKeyDown('W')) {
            active_camera->transform().moveZ(k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed, Transform::LOCAL);
        } else if (INPUTMANAGER->getKeyDown('S')) {
            active_camera->transform().moveZ(-k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed, Transform::LOCAL);
        }

        if (INPUTMANAGER->getKeyDown('A')) {
            active_camera->transform().moveX(-k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed, Transform::LOCAL);
        } else if (INPUTMANAGER->getKeyDown('D'))
        {
            active_camera->transform().moveX(k_QGLGameEditorViewport_DefaultCameraDisplacement * camera_speed, Transform::LOCAL);
        }

        if (INPUTMANAGER->getKeyDown(VK_UP)) {
            active_camera->transform().rotateX(-1.0f);
        } else if (INPUTMANAGER->getKeyDown(VK_DOWN)) {
            active_camera->transform().rotateX(1.0f);
        }

        if (INPUTMANAGER->getKeyDown(VK_LEFT)) {
            active_camera->transform().rotateY(-1.0f);
        } else if (INPUTMANAGER->getKeyDown(VK_RIGHT)) {
            active_camera->transform().rotateY(1.0f);
        }
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::customDraw() {
    QGLGameViewport::customDraw();

    if (m_selectedObjectUID != 0) {
        drawManipulatorsForGameObject(m_selectedObjectUID);
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::postDraw() {
    m_drawMode = TEXTURED;
    m_clearColor = QColor(160, 200, 250); // TODO move these values to constants.h?

    if (m_previousActiveCameraId != "") {
        CDIRECTOR->setActiveCameraHavingId(m_previousActiveCameraId);
        m_previousActiveCameraId = "";
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawGameObject(CGameObject *node) {
    if (m_drawMode == SOLIDCOLORS) {
        QColor color = m_colorProvider.nextColor();
        m_nodesColor.insert(node->uid(), color);

        GLfloat* color_array = new GLfloat[3];
        color_array[0] = (float)color.red()   / 255.0f;
        color_array[1] = (float)color.green() / 255.0f;
        color_array[2] = (float)color.blue()  / 255.0f;
        m_selectedProgram->setAttributeValue(2, (const GLfloat*)color_array, 1, 3);
        SAFE_RELEASE(color_array);
    }
    QGLGameViewport::drawGameObject(node);

    if (node->uid() == m_selectedObjectUID) {
        m_selectedGameObject = node;
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawManipulatorsForGameObject(uint uid) {
    glDisable(GL_DEPTH_TEST);
    const CGameObject* game_obj = CDIRECTOR->currentGameScene()->getGameObject(uid);
    RE_ASSERT(game_obj);

    CLNode* manipulator_trans = m_colladaScene->getNodeHavingId("__hidden_modifier_trans");

    if (!manipulator_trans) {
        LOG("Error!: Couldn't find node '__hidden_modifier_trans', manipulator won't be drawn");
        glEnable(GL_DEPTH_TEST);
        return;
    }

    btVector3 scale = btVector3(k_CLRenderManager_ManipulatorsScaleValue,
                                k_CLRenderManager_ManipulatorsScaleValue,
                                k_CLRenderManager_ManipulatorsScaleValue);

    CCamera* active_camera = CDIRECTOR->setActiveCameraHavingId(k_QGLGameEditorViewport_CameraId);

    float distance_to_camera = (active_camera->constTransform().localPosition() - (game_obj->constTransform().localPosition() * k_QGLGameEditorViewport_SizeFactor)).length();
    scale *= (distance_to_camera * k_QGLGameEditorViewport_SizeFactor);

    switch (m_selectedManipulatorType) {
    case TRANSLATION:
        drawCommonManipulatorsForNode((CLNode*)game_obj,
                                      TRANSLATION, m_selectedTransformMode, scale);
        break;
    case SCALE:
        drawCommonManipulatorsForNode((CLNode*)game_obj,
                                      SCALE, m_selectedTransformMode, scale);
        break;
    case UNIFORMSCALE:
        drawCommonManipulatorsForNode((CLNode*)game_obj,
                                      UNIFORMSCALE, m_selectedTransformMode, scale);
        break;
    case ROTATION:
        drawRotationManipulatorsForNode((CLNode*)game_obj,
                                        m_selectedTransformMode, scale);
        break;
    }
    glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawCommonManipulatorsForNode(CLNode* node, ManipulatorTypes manipulator_type, Transform::TransformMode transform_mode, btVector3 &scale) {
    if (manipulator_type == NOMANIPULATOR || manipulator_type == ROTATION) {
        return;
    }

    QColor color_x = Qt::red;
    QColor color_y = Qt::green;
    QColor color_z = Qt::blue;

    std::string manipulator_node_id = "";
    switch (manipulator_type) {
    case TRANSLATION:
        manipulator_node_id = "__hidden_modifier_trans"; break;
    case SCALE:
        manipulator_node_id = "__hidden_modifier_scale"; break;
    case UNIFORMSCALE:
        manipulator_node_id = "__hidden_modifier_scale";
        color_x = QColor(0, 0, 254);
        color_y = QColor(0, 0, 253);
        color_z = Qt::blue;
        break;
    }

    CLNode* manipulator_node = m_colladaScene->getNodeHavingId(manipulator_node_id);

    if (!manipulator_node) {
        LOG("Error!: Couldn't find node '%s', manipulator won't be drawn", manipulator_node_id.c_str());
        glEnable(GL_DEPTH_TEST);
        return;
    }
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL ||
            manipulator_type == SCALE ||
            manipulator_type == UNIFORMSCALE) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }

    MATRIXSTACK->rotate(0, DEGTORAD(90.0f),0);
    MATRIXSTACK->scale(scale);
    MATRIXSTACK->translate(0.0f,0.0f,2.0f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_x);
    MATRIXSTACK->popMatrix();

    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL ||
            manipulator_type == SCALE ||
            manipulator_type == UNIFORMSCALE) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }
    MATRIXSTACK->rotate(DEGTORAD(-90.0f), 0, 0);
    MATRIXSTACK->scale(scale);
    MATRIXSTACK->translate(0, 0, 2.0f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_y);
    MATRIXSTACK->popMatrix();

    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL ||
            manipulator_type == SCALE ||
            manipulator_type == UNIFORMSCALE) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }
    MATRIXSTACK->scale(scale);
    MATRIXSTACK->translate(0, 0, 2.0f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_z);
    MATRIXSTACK->popMatrix();
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawRotationManipulatorsForNode(CLNode* node, Transform::TransformMode transform_mode, btVector3 &scale) {
    QColor color_x = Qt::red;
    QColor color_y = Qt::green;
    QColor color_z = Qt::blue;

    std::string manipulator_node_id = "__hidden_modifier_rotation";
    CLNode* manipulator_node = m_colladaScene->getNodeHavingId(manipulator_node_id);

    if (!manipulator_node) {
        LOG("Error!: Couldn't find node '%s', manipulator won't be drawn", manipulator_node_id.c_str());
        glEnable(GL_DEPTH_TEST);
        return;
    }

    // Z
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }
    MATRIXSTACK->rotate(DEGTORAD(90.0f), 0, 0);
    MATRIXSTACK->scale(scale * 1.5f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_z);
    MATRIXSTACK->popMatrix();

    // Y
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }
    MATRIXSTACK->scale(scale * 1.5f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_y);
    MATRIXSTACK->popMatrix();

    // X
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(node->transform().localPosition());
    if (transform_mode == Transform::LOCAL) {
        MATRIXSTACK->rotate(node->transform().localRotation());
    }
    MATRIXSTACK->rotate(0, DEGTORAD(90.0f), 0);
    MATRIXSTACK->rotate(DEGTORAD(90.0f), 0, 0);
    MATRIXSTACK->scale(scale * 1.5f);
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, manipulator_node, &color_x);
    MATRIXSTACK->popMatrix();
}

//-----------------------------------------------------------------------------

float QGLGameEditorViewport::getMotionThroughAxis(btVector3 &axis, Vector2 motion_vec) {
    float _x = btCustom::right().dot(axis);
    float _y = btCustom::up().dot(axis);
    float _z = btCustom::forward().dot(axis);
    btVector3 rp = btVector3(_x, _y, _z);
    rp.normalize();
    float dot_p = btVector3(motion_vec.x, motion_vec.y, 1).dot(rp);

    return dot_p;
}

//-----------------------------------------------------------------------------

btVector3 QGLGameEditorViewport::calcScaleVec(AxisTypes direction, float dot_p, bool uniform) {
    btVector3 scale;
    if (!uniform) {
        switch (direction) {
        case X:
            scale = btVector3(dot_p, 0, 0);
            break;
        case Y:
            scale = btVector3(0, dot_p, 0);
            break;
        case Z:
            scale = btVector3(0, 0, dot_p);
            break;
        }
    } else {
        scale = btVector3(1,1,1) * dot_p;
    }
    scale *= 0.05f;
    return scale;
}

//-----------------------------------------------------------------------------

btVector3 QGLGameEditorViewport::calcTranslationVec(btVector3& main_axis,
                                                    Vector2 motion_vec,
                                                    btMatrix3x3& object_rotation,
                                                    float distance_to_camera,
                                                    Transform::TransformMode transform_mode,
                                                    AxisTypes axis) {
    btVector3 translation_vec;

    if(main_axis.x() == 0.0f &&
            main_axis.y() == 0.0f &&
            main_axis.z() != 0.0f) {
        main_axis.setY(main_axis.z());
        main_axis.setZ(0.0f);
    }

    Vector2 main_axis_2d = Vector2(main_axis.x(), main_axis.y());
    if(!(main_axis_2d.x == 0 && main_axis_2d.y == 0)) {
        main_axis_2d.normalize();
    }

    float dot_p = main_axis_2d.dot(motion_vec);

    if (transform_mode == Transform::LOCAL) {
        switch (axis) {
        case X: translation_vec = btCustom::getAxisX(object_rotation); break;
        case Y: translation_vec = btCustom::getAxisY(object_rotation); break;
        case Z: translation_vec = btCustom::getAxisZ(object_rotation); break;
        }
        translation_vec *= dot_p;
    } else {
        switch (axis) {
        case X: translation_vec = btVector3(dot_p, 0, 0); break;
        case Y: translation_vec = btVector3(0, dot_p, 0); break;
        case Z: translation_vec = btVector3(0, 0, dot_p); break;
        }

    }
    float distance_factor = (distance_to_camera / 2.0f) / 100.0f;
    translation_vec *= distance_factor;

    return translation_vec;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::manipulatorMoved(ManipulatorTypes manipulator_type, int dx, int dy) {
    if (manipulator_type == NOMANIPULATOR) {
        return;
    }

    Transform* transform = &m_selectedGameObject->transform();
    Vector4 tmp;

    btQuaternion quat_diff;
    Vector2 motion_vec;
    motion_vec.set(dx, dy);

    btVector3 translation_vec;
    btMatrix3x3 rot_mat = btMatrix3x3();
    if(m_selectedTransformMode == Transform::LOCAL) {
        rot_mat = transform->local().getInnerTransform().getBasis();
    } else {
        rot_mat.setIdentity();
    }

    rot_mat = rot_mat.transpose();

    if(m_isFirstStep) {
        x_axis = btCustom::getAxisX(rot_mat);
        x_axis.setZ(x_axis.getZ() * -1.0f);
        tmp = m_modelViewMatrix * Vector4(x_axis.getX(), x_axis.getY(), x_axis.getZ(), 0);
        x_axis = btVector3(tmp.x, tmp.y, tmp.z);
        x_axis.normalize();

        y_axis = btCustom::getAxisY(rot_mat);
        y_axis.setZ(y_axis.getZ() * -1.0f);
        tmp = m_modelViewMatrix * Vector4(y_axis.getX(), y_axis.getY(), y_axis.getZ(), 0);
        y_axis = btVector3(tmp.x, tmp.y, tmp.z);
        y_axis.normalize();

        z_axis = btCustom::getAxisZ(rot_mat);
        z_axis.setZ(z_axis.getZ() * -1.0f);
        tmp = m_modelViewMatrix * Vector4(z_axis.getX(), z_axis.getY(), z_axis.getZ(), 0);
        z_axis = btVector3(tmp.x, tmp.y, tmp.z);
        z_axis.normalize();
    }

    btVector3 a = transform->localPosition();
    btVector3 b = CDIRECTOR->getActiveCamera()->transform().localPosition();

    float distance_to_camera = sqrtf(pow(b.x() - a.x(), 2) + pow(b.y() - a.y(), 2) + pow(b.z() - a.z(), 2));

    switch (manipulator_type) {
    case TRANSLATION: {
        switch (m_selectedAxis) {
        case X: {
            translation_vec = calcTranslationVec(x_axis, motion_vec, rot_mat, distance_to_camera,
                                                 m_selectedTransformMode, m_selectedAxis);
        } break;
        case Y: {
            translation_vec = calcTranslationVec(y_axis, motion_vec, rot_mat,distance_to_camera,
                                                 m_selectedTransformMode, m_selectedAxis);
        } break;
        case Z: {
            translation_vec = calcTranslationVec(z_axis, motion_vec, rot_mat,distance_to_camera,
                                                 m_selectedTransformMode, m_selectedAxis);
        } break;
        }
        transform->setLocalPosition(transform->localPosition() + translation_vec);
    } break;
    case SCALE:
    case UNIFORMSCALE: {
        btVector3 axis;
        switch (m_selectedAxis) {
        case X: {
            axis = x_axis;
        } break;
        case Y: {
            axis = y_axis;
        } break;
        case Z: {
            axis = z_axis;
        } break;
        }
        float dot_p = getMotionThroughAxis(axis, motion_vec);
        transform->setLocalScale(transform->localScale() + calcScaleVec(m_selectedAxis, dot_p, manipulator_type == UNIFORMSCALE));
    } break;

    case ROTATION: {
        switch (m_selectedAxis) {
        case X: {
            if(m_isFirstStep) {
                y_axis = btCustom::getAxisY(rot_mat);
                y_axis.setZ(y_axis.z() * -1.0f);
            }
            tmp = m_modelViewMatrix * Vector4(y_axis.x(), y_axis.y(), y_axis.z(), 0);
            y_axis = btVector3(tmp.x, tmp.y, tmp.z);
            y_axis.normalize();

            float dot_p = getMotionThroughAxis(y_axis, motion_vec);
            quat_diff = btQuaternion(0, DEGTORAD(dot_p), 0);
        } break;
        case Y: {
            if(m_isFirstStep) {
                x_axis = btCustom::getAxisX(rot_mat);
                x_axis.setZ(x_axis.z() * -1.0f);
            }
            tmp = m_modelViewMatrix * Vector4(x_axis.x(), x_axis.y(), x_axis.z(), 0);
            x_axis = btVector3(tmp.x, tmp.y, tmp.z);
            x_axis.normalize();

            float dot_p = getMotionThroughAxis(x_axis, motion_vec);
            quat_diff = btQuaternion(DEGTORAD(dot_p), 0, 0);
        } break;
        case Z: {
            if(m_isFirstStep) {
                z_axis = btCustom::getAxisZ(rot_mat);
                z_axis.setZ(z_axis.z() * -1.0f);
            }
            tmp = m_modelViewMatrix * Vector4(z_axis.x(), z_axis.y(), z_axis.z(), 0);
            z_axis = btVector3(tmp.x, tmp.y, tmp.z);
            z_axis.normalize();

            float dot_p = getMotionThroughAxis(z_axis, motion_vec);
            quat_diff = btQuaternion(0, 0, DEGTORAD(dot_p));
        } break;
        }
        if (m_selectedTransformMode == Transform::LOCAL) {
            transform->setLocalRotation(transform->localRotation() * quat_diff);
        } else {
            transform->setLocalRotation(quat_diff * transform->localRotation());
        }
    }
        break;
    }
    m_isFirstStep = false;
}

//-----------------------------------------------------------------------------

QGLGameEditorViewport::AxisTypes QGLGameEditorViewport::selectAxis(int x, int y) {
    GLubyte *data = new GLubyte[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    QColor picked_color = QColor(data[0], data[1], data[2]);
    SAFE_RELEASE_ARRAY(data);

    if (picked_color == Qt::red || picked_color == QColor(0, 0, 254)) {
        return X;
    }
    if (picked_color == Qt::green || picked_color == QColor(0, 0, 253)) {
        return Y;
    }
    if (picked_color == Qt::blue) {
        return Z;
    }

    return NOAXIS;
}

//-----------------------------------------------------------------------------

uint QGLGameEditorViewport::selectObject(int x, int y) {
    uint selected_game_obj = 0;
    m_drawMode = SOLIDCOLORS;

    paintGL(); // redraw the scene using only solid colors

    GLubyte *data = new GLubyte[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    QColor picked_color = QColor(data[0], data[1], data[2]);
    SAFE_RELEASE_ARRAY(data);

    for (QMap<uint, QColor>::Iterator it = m_nodesColor.begin();
         it != m_nodesColor.end();
         it++) {
        if (it.value() == picked_color) {
            selected_game_obj = (uint)it.key();
        }
    }

    return selected_game_obj;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawSkybox() {
    if (m_drawMode == SOLIDCOLORS) {
        GLfloat* color_array = new GLfloat[3];
        color_array[0] = color_array[1] = color_array[2] = 0.0f;
        m_selectedProgram->setAttributeValue(2, (const GLfloat*)color_array, 1, 3);
        SAFE_RELEASE(color_array);
    }
    QGLGameViewport::drawSkybox();
}

void QGLGameEditorViewport::step() {
    // CDirector->step() will call InputManager->update(), we don't want that to
    // happen since it is already being called from the GameWindow's Viewport.
    // So let's call CDIRECTOR->step() only if the game is not running yet.
    if(CDIRECTOR->gameState() != CDirector::RUNNING) {
        CDIRECTOR->step();
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::drawCrosshair() {
    // Intentionally overridden, we don't want the crosshair drawn on the Editor
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::mousePressEvent(QMouseEvent *event) {
    int x = event->pos().x();
    int y = height() - event->pos().y();  // "y" needs to be inverted for OpenGL

    if (CDIRECTOR->gameState() != CDirector::RUNNING &&
            event->button() == Qt::LeftButton) {
        m_selectedAxis = selectAxis(x, y);

        if (m_selectedAxis == NOAXIS) {
            m_selectedObjectUID = selectObject(x, y);
            emit(newGameObjectSelected(m_selectedObjectUID));
        } else {
            m_oldTransform = (m_selectedGameObject->transform());
        }
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    // clamp dx & dy between -10 and +10
    dx = (dx > 10  ? 10  : dx);
    dx = (dx < -10 ? -10 : dx);
    dy = (dy > 10  ? 10  : dy);
    dy = (dy < -10 ? -10 : dy);

    if (CDIRECTOR->getActiveCameraId() != m_previousActiveCameraId) {
        m_previousActiveCameraId = CDIRECTOR->getActiveCameraId();
    }
    CCamera* active_camera = CDIRECTOR->setActiveCameraHavingId(k_QGLGameEditorViewport_CameraId);

    if (m_selectedAxis != NOAXIS) {
        manipulatorMoved(m_selectedManipulatorType, dx, -dy);
    } else if (event->buttons() & Qt::MiddleButton) { // if the middle button is pressed then pan the camera
        active_camera->transform().moveX((float)dx * 0.05f, Transform::LOCAL);
        active_camera->transform().moveY((float)-dy * 0.05f, Transform::LOCAL);
    } else if (event->buttons() & Qt::RightButton) {
        active_camera->transform().rotateX((float)dy * 0.25f);
        active_camera->transform().rotateY((float)dx * 0.25f);
    }

    m_lastPos = event->pos();

    if (m_previousActiveCameraId != "") {
        CDIRECTOR->setActiveCameraHavingId(m_previousActiveCameraId);
        m_previousActiveCameraId = "";
    }
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::mouseReleaseEvent(QMouseEvent*) {
    TranslateAction* translate_action = NULL;
    ScaleAction* scale_action = NULL;
    RotateAction* rotate_action = NULL;

    if (m_selectedAxis != NOAXIS) {
        ManipulatorTypes selected_manipulator = NOMANIPULATOR;
        switch (m_selectedManipulatorType) {
        case TRANSLATION:
            selected_manipulator = TRANSLATION;
            translate_action = new TranslateAction(m_selectedGameObject, m_oldTransform.localPosition());
            UNDOABLEACTIONSMANAGER->pushUndoAction((IUndoableAction*)translate_action);
            break;
        case SCALE:
        case UNIFORMSCALE:
            selected_manipulator = SCALE;
            scale_action = new ScaleAction(m_selectedGameObject, m_oldTransform.localScale(), m_autoRecalcAABBOnResize);
            UNDOABLEACTIONSMANAGER->pushUndoAction((IUndoableAction*)scale_action);
            break;
        case ROTATION:
            selected_manipulator = ROTATION;
            rotate_action = new RotateAction(m_selectedGameObject, m_oldTransform.localRotation());
            UNDOABLEACTIONSMANAGER->pushUndoAction((IUndoableAction*)rotate_action);
            break;
        }
        emit(selectedGameObjectTransformChanged(selected_manipulator));
    }
    m_selectedAxis = NOAXIS;
    m_isFirstStep = true;
}

//-----------------------------------------------------------------------------

void QGLGameEditorViewport::wheelEvent(QWheelEvent* event) {
    INPUTMANAGER->setWheelVelocity((float)event->delta());
}

//-----------------------------------------------------------------------------

QGLGameEditorViewport::~QGLGameEditorViewport() {
}
