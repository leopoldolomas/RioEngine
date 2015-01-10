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

#ifndef RIOENGINE_EDITOR_QGLGAMEEDITORVIEWPORT_QGLGAMEEDITORVIEWPORT_H_
#define RIOENGINE_EDITOR_QGLGAMEEDITORVIEWPORT_QGLGAMEEDITORVIEWPORT_H_

#include "misc/transform.h"
#include "editor/qglgameviewport/qglgameviewport.h"
#include "editor/qglgameeditorviewport/qcolorprovider.h"

class CGameObject;

class QGLGameEditorViewport : public QGLGameViewport {
    Q_OBJECT
 public:
    enum AxisTypes        { NOAXIS, X, Y, Z };
    enum ManipulatorTypes { NOMANIPULATOR, TRANSLATION, ROTATION, SCALE, UNIFORMSCALE };
    enum DrawMode         { TEXTURED, SOLIDCOLORS };

    QGLGameEditorViewport(CLScene* collada_scene, CGameScene* game_scene);

    void setSelectedGameObjectUID(uint);
    uint selectedGameObjectUID() const;

    int cameraSpeed() const;
    int nextCameraSpeed();

    bool autoRecalcAABBOnResize() const;
    void setAutoRecalcAABBOnResize(bool);

    ManipulatorTypes selectedManipulatorType() const;
    void setSelectedManipulatorType(const ManipulatorTypes &value);

    Transform::TransformMode selectedTransformMode() const;
    void setSelectedTransformMode(const Transform::TransformMode &value);

    ~QGLGameEditorViewport();

protected:
    /**
     * @brief Determines how parallel is the mouse movement to a manipulator axis
     * @return 0 if not parallel, 1 if completely parallel
     */
    float getMotionThroughAxis(btVector3&, Vector2);

    /**
     * @brief Calculates the differential translation vector based upon the
     * object's position, rotation, and the motion vector
     * @return The differential translation vector
     */
    btVector3 calcTranslationVec(btVector3&, Vector2, btMatrix3x3&, float, Transform::TransformMode, AxisTypes);

    /**
     * @brief Calculates the differential scale vector based upon the
     * given axis direction and differential size
     */
    btVector3 calcScaleVec(AxisTypes, float, bool);

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

    void drawSolidCone(GLdouble, GLdouble, GLint, GLint);

    /**
     * @brief Draws the Translation, Rotation or Scale manipulators for the
     * given game object's unique id
     */
    void drawManipulatorsForGameObject(uint uid);
    void drawCommonManipulatorsForNode(CLNode* node,
                                       ManipulatorTypes manipulator_type,
                                       Transform::TransformMode transform_mode,
                                       btVector3 &scale);
    void drawRotationManipulatorsForNode(CLNode* node,
                                         Transform::TransformMode transform_mode,
                                         btVector3 &scale);
    void manipulatorMoved(ManipulatorTypes manipulator_type,
                          int dx,
                          int dy);

    /**
     * @brief Returns the picked axis by the user
     */
    AxisTypes selectAxis(int x, int y);

    /**
     * @brief Returns the selected game object (if any) at the given coordinates
     */
    uint selectObject(int x, int y);

    // from QGLBaseViewport
    virtual void paintGL();
    virtual void step();
    virtual void preDraw();
    virtual void customDraw();
    virtual void postDraw();
    // ---------------------

    // from QGLGameViewport
    virtual void drawGameObject(CGameObject * game_object);
    virtual void drawSkybox();
    virtual void drawCrosshair();
    // ---------------------

    uint            m_selectedObjectUID;
    QPoint          m_lastPos;
    AxisTypes       m_selectedAxis;
    DrawMode        m_drawMode;
    QColorProvider  m_colorProvider;
    CGameObject*    m_selectedGameObject;
    Transform       m_oldTransform;
    std::string     m_previousActiveCameraId;
    QMap<uint, QColor> m_nodesColor;

private:
   int  m_cameraSpeed;
   bool m_autoRecalcAABBOnResize;
   bool m_isFirstStep;
   btVector3 x_axis, y_axis, z_axis;

   ManipulatorTypes m_selectedManipulatorType;
   Transform::TransformMode m_selectedTransformMode;

signals:
    void newGameObjectSelected(uint);
    void selectedGameObjectTransformChanged(QGLGameEditorViewport::ManipulatorTypes);
};

#endif  //  RIOENGINE_EDITOR_QGLGAMEEDITORVIEWPORT_QGLGAMEEDITORVIEWPORT_H_
