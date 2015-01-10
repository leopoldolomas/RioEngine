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

#ifndef RIOENGINE_EDITOR_QGLBASEVIEWPORT_QGBASEVIEWPORT_H_
#define RIOENGINE_EDITOR_QGLBASEVIEWPORT_QGBASEVIEWPORT_H_

#include <QGLWidget>
#include "bullet/matrix/matrices.h"

class QGLShaderProgram;

/**
 * Provides basic functionality for any Viewport. See also QGLGameViewport and
 * QGLGameEditorViewport
 */
class QGLBaseViewport : public QGLWidget {
    Q_OBJECT
public:
    enum QGLViewportDrawMode { WIRED, SOLID, TEXTURED };

    QGLBaseViewport(QWidget* parent, QGLWidget* shared_widget);

    QGLViewportDrawMode drawMode() const;
    void setDrawMode(const QGLViewportDrawMode &value);

    bool hasFocus() const;

    virtual void preDraw();
    virtual void paintGL();
    virtual void step();
    virtual void customDraw();
    virtual void postDraw();

    ~QGLBaseViewport();

protected:
    void recalculateAspectRatio();

    virtual void initializeGL();
    virtual void resizeGL(int, int);

    void mousePressEvent(QMouseEvent *event);

    virtual void focusInEvent(QFocusEvent*);
    virtual void focusOutEvent(QFocusEvent*);

    GLdouble m_projectionRatio;
    QPoint   m_lastPos;
    QColor   m_clearColor;
    Matrix4  m_modelViewMatrix;

    QGLShaderProgram *m_selectedProgram;
    QGLShaderProgram *m_texturedProgram;
    QGLShaderProgram *m_solidColorsProgram;

private:
    void setHasFocus(bool);

    bool     m_hasFocus;
    GLdouble projectionFovYAngle;
    GLdouble projectionZNear;
    GLdouble projectionZFar;

    QGLViewportDrawMode m_drawMode;
};

#endif  // RIOENGINE_EDITOR_QGLBASEVIEWPORT_QGBASEVIEWPORT_H_
