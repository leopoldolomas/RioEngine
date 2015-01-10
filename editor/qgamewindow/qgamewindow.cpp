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

#include <QBoxLayout>
#include <QTimer>
#include "clengine/classetsrepository.h"
#include "cengine/cgamescene.h"
#include "cengine/cdirector.h"
#include "editor/qglgameviewport/qglgameviewport.h"
#include "ui_qgamewindow.h"
#include "qgamewindow.h"

//-----------------------------------------------------------------------------

QGameWindow::QGameWindow(QWidget *parent,
                         CLScene* collada_scene,
                         CGameScene* game_scene,
                         QGLWidget* shared_widget) :
    QMainWindow(parent),
    m_ui(new Ui::QGameWindow) {
    m_ui->setupUi(this);

    m_ui->actionWired->setEnabled(false); // not supported yet
    m_ui->actionSolid->setEnabled(false); // not supported yet

    createGameViewport(collada_scene, shared_widget);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimeOut()));
    m_timer->start(17);  // 60 fps

    CDIRECTOR->resume();
}

//-----------------------------------------------------------------------------

void QGameWindow::setDrawMode(QGLBaseViewport::QGLViewportDrawMode draw_mode) {
    m_gameViewport->setDrawMode(draw_mode);
    m_ui->actionWired->setChecked(draw_mode == WIRED);
    m_ui->actionSolid->setChecked(draw_mode == SOLID);
    m_ui->actionTextured->setChecked(draw_mode == TEXTURED);
}

//-----------------------------------------------------------------------------

void QGameWindow::createGameViewport(CLScene* collada_scene,
                                     QGLWidget* shared_widget) {
    m_gameViewport = new QGLGameViewport(collada_scene, shared_widget);
    setCentralWidget(m_gameViewport);
}

//-----------------------------------------------------------------------------

void QGameWindow::updateGUIState() {
}

//-----------------------------------------------------------------------------

void QGameWindow::TimeOut(void) {
    if (m_gameViewport) {
        m_gameViewport->updateGL();
    }
}

//-----------------------------------------------------------------------------

void QGameWindow::on_actionExit_triggered() {
    close();
}

//-----------------------------------------------------------------------------

void QGameWindow::on_actionWired_triggered() {
    setDrawMode(QGLBaseViewport::WIRED);
}

//-----------------------------------------------------------------------------

void QGameWindow::on_actionSolid_triggered() {
    setDrawMode(QGLBaseViewport::SOLID);
}

//-----------------------------------------------------------------------------

void QGameWindow::on_actionTextured_triggered() {
    setDrawMode(QGLBaseViewport::TEXTURED);
}

//-----------------------------------------------------------------------------

void QGameWindow::on_actionFull_Screen_triggered(bool checked) {
    if (checked) {
        showFullScreen();
    }
    else {
        showNormal();
    }
}

//-----------------------------------------------------------------------------

void QGameWindow::closeEvent(QCloseEvent *) {
    m_timer->stop();
}

//-----------------------------------------------------------------------------

QGameWindow::~QGameWindow() {
    SAFE_RELEASE(m_ui);
}
