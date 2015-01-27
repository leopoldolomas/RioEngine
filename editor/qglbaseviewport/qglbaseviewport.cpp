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

#include <QtOpenGL>
#include "constants.h"
#include "misc/btcustom.h"
#include "misc/directoryhelper.hpp"
#include "cengine/ccamera.h"
#include "cengine/cdirector.h"
#include "clengine/clrenderer.h"
#include "editor/qglbaseviewport/qglbaseviewport.h"

//-----------------------------------------------------------------------------

QGLBaseViewport::QGLBaseViewport(QWidget*, QGLWidget* shared_widget)
    : QGLWidget(0, shared_widget) {
    m_drawMode            = TEXTURED;
    projectionZNear      = k_QGLViewport_zNear;
    projectionZFar       = k_QGLViewport_zFar;
    projectionFovYAngle  = k_QGLViewport_fovYAngle;
    m_clearColor         = QColor(160, 200, 250); // TODO move this values to constants.h
    m_selectedProgram    = 0;
    m_texturedProgram    = 0;
    m_solidColorsProgram = 0;

    setFocusPolicy(Qt::ClickFocus);
    setHasFocus(false);
    recalculateAspectRatio();
}

//-----------------------------------------------------------------------------

QGLBaseViewport::QGLViewportDrawMode QGLBaseViewport::drawMode() const {
    return m_drawMode;
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::setDrawMode(const QGLViewportDrawMode &value) {
    m_drawMode = value;
}

//-----------------------------------------------------------------------------

bool QGLBaseViewport::hasFocus() const {
    return m_hasFocus;
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::recalculateAspectRatio() {
    m_projectionRatio = 1.0f * width() / height();
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE    0
#define PROGRAM_TEXCOORD_ATTRIBUTE  1

    QString path = QString::fromStdString(DirectoryHelper::getProjectPath()) + k_QGLViewport_ShadersFolderName + "\\";

    QGLShader *solidColor_vshader = new QGLShader(QGLShader::Vertex, this);
    bool result = solidColor_vshader->compileSourceFile(path + "solidcolor.vert");

    QGLShader *solidColor_fshader = new QGLShader(QGLShader::Fragment, this);
    result = solidColor_fshader->compileSourceFile(path + "solidcolor.frag");

    m_solidColorsProgram = new QGLShaderProgram(this);
    m_solidColorsProgram->addShader(solidColor_vshader);
    m_solidColorsProgram->addShader(solidColor_fshader);
    m_solidColorsProgram->bindAttributeLocation("vertex", 0);
    m_solidColorsProgram->bindAttributeLocation("texCoord", 1);
    m_solidColorsProgram->bindAttributeLocation("color", 2);
    result = m_solidColorsProgram->link();
    result = m_solidColorsProgram->bind();

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    result = vshader->compileSourceFile(path + "simple.vert");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    result = fshader->compileSourceFile(path + "simple.frag");

    m_texturedProgram = new QGLShaderProgram(this);
    m_texturedProgram->addShader(vshader);
    m_texturedProgram->addShader(fshader);
    m_texturedProgram->bindAttributeLocation("vertex",   PROGRAM_VERTEX_ATTRIBUTE);
    m_texturedProgram->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    m_texturedProgram->bindAttributeLocation("solidColor", 10);
    result = m_texturedProgram->link();
    result = m_texturedProgram->bind();
    m_texturedProgram->setUniformValue("texture", 0);
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::preDraw() {
    m_selectedProgram = m_texturedProgram;
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::step() {
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::paintGL() {
    preDraw();
    step();

    qglClearColor(m_clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CCamera *active_camera = CDIRECTOR->getActiveCamera();
    Matrix4 q_model, q_clip, q_camera, q_cameraToClip;
    q_camera = btCustom::btQuaternionToMatrix4(active_camera->transform().localRotation());
    q_clip.setPerspective(projectionFovYAngle, m_projectionRatio, projectionZNear, projectionZFar);

    const btVector3* camera_position = &active_camera->transform().localPosition();

    btVector3 axis_x = btVector3(q_camera[0], q_camera[4], q_camera[8]);
    btVector3 axis_y = btVector3(q_camera[1], q_camera[5], q_camera[9]);
    btVector3 axis_z = btVector3(q_camera[2], q_camera[6], q_camera[10]);

    btVector3 fixed_location(-axis_x.dot(*camera_position),
                             -axis_y.dot(*camera_position),
                             -axis_z.dot(*camera_position));

    q_camera[12] = fixed_location.x();
    q_camera[13] = fixed_location.y();
    q_camera[14] = fixed_location.z();
    q_cameraToClip = q_camera * q_clip;

    QMatrix4x4 qmatrix_model(QMatrix4x4(q_model.get()));
    QMatrix4x4 qmatrix_camera_to_clip(QMatrix4x4(q_cameraToClip.getTranspose()));

    m_selectedProgram->setUniformValue("modelMatrix",        qmatrix_model);
    m_selectedProgram->setUniformValue("cameraToClipMatrix", qmatrix_camera_to_clip);

    m_modelViewMatrix = q_camera;

    customDraw();
    postDraw();
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::customDraw() {
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::postDraw() {
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::resizeGL(int width, int height) {
    recalculateAspectRatio();
    glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::focusInEvent(QFocusEvent*) {
    setHasFocus(true);
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::focusOutEvent(QFocusEvent*) {
    setHasFocus(false);
}

//-----------------------------------------------------------------------------

void QGLBaseViewport::setHasFocus(bool has_focus) {
    m_hasFocus = has_focus;
}

//-----------------------------------------------------------------------------

QGLBaseViewport::~QGLBaseViewport() {
}
