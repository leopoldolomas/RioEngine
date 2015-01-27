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
#include "misc/matrixstack.h"
#include "misc/stringhelper.hpp"
#include "misc/directoryhelper.hpp"
#include "cengine/time_.h"
#include "cengine/cgameobject.h"
#include "cengine/ccamera.h"
#include "cengine/cdirector.h"
#include "cengine/cgamescene.h"
#include "clengine/classetsrepository.h"
#include "clengine/clrenderer.h"
#include "editor/QGLGameViewport/bdebugdraw.h"
#include "editor/qglgameviewport/qglgameviewport.h"

//-----------------------------------------------------------------------------

QGLGameViewport::QGLGameViewport(CLScene* collada_scene,
                                 QGLWidget*  shared_widget) : QGLBaseViewport(this, shared_widget) {
    setColladaScene(collada_scene);
    m_bDebugDraw = NULL;
}

//-----------------------------------------------------------------------------

void QGLGameViewport::setColladaScene(CLScene* collada_scene) {
    RE_ASSERT(collada_scene);
    m_colladaScene = collada_scene;
}

//-----------------------------------------------------------------------------

CLScene* QGLGameViewport::colladaScene() const {
    return m_colladaScene;
}

//-----------------------------------------------------------------------------

void QGLGameViewport::drawSkybox() {
    CLNode* skybox_node = m_colladaScene->getNodeHavingId(k_CLScene_SkyboxNodeId);
    if (skybox_node) {
        CLRenderer::drawNode(m_selectedProgram, m_colladaScene, skybox_node, NULL);
    }
}

//-----------------------------------------------------------------------------

void QGLGameViewport::drawWorldAxis() {
    CLRenderer::drawLine(m_texturedProgram, btVector3(-k_QGLGameViewport_WorldAxisLength, 0, 0),
                         btVector3(k_QGLGameViewport_WorldAxisLength, 0, 0), &QColor(255, 0, 0));

    CLRenderer::drawLine(m_texturedProgram, btVector3(0, -k_QGLGameViewport_WorldAxisLength, 0),
                         btVector3(0, k_QGLGameViewport_WorldAxisLength, 0), &QColor(0, 255, 0));

    CLRenderer::drawLine(m_texturedProgram, btVector3(0, 0, -k_QGLGameViewport_WorldAxisLength),
                         btVector3(0, 0, k_QGLGameViewport_WorldAxisLength), &QColor(0, 0, 255));
}

//-----------------------------------------------------------------------------

void QGLGameViewport::drawGameObject(CGameObject* node) {
    btScalar raw_matrix[16];
    node->transform().global().getOpenGLMatrix(raw_matrix);
    Matrix4 matrix(raw_matrix);
    MATRIXSTACK->setMatrix(matrix.getTranspose());
    CLRenderer::drawNode(m_selectedProgram, m_colladaScene, node, NULL);
    QObjectList children = node->children();

    for (QObjectList::iterator it = children.begin();
         it != children.end();
         ++it) {
        CGameObject* gameobj_child = qobject_cast<CGameObject*>(*it);
        if (gameobj_child != NULL) {
            drawGameObject(gameobj_child);
        }
    }
}

//-----------------------------------------------------------------------------

void QGLGameViewport::step() {
    CDIRECTOR->step();
}

//-----------------------------------------------------------------------------

void QGLGameViewport::initializeGL() {
    QGLBaseViewport::initializeGL();
    if (!CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer()) {
        m_bDebugDraw = new bDebugDraw(m_texturedProgram);
        CDIRECTOR->currentGameScene()->dynamicsWorld()->setDebugDrawer(m_bDebugDraw);
    }
    bindTextures();

    emit afterInitializeGL(this);
}

//-----------------------------------------------------------------------------

void QGLGameViewport::preDraw() {
    QGLBaseViewport::preDraw();

    if (CDIRECTOR->gameState() == CDirector::RUNNING) {
        Time::___update();
    }
}

//-----------------------------------------------------------------------------

void QGLGameViewport::customDraw() {
    if (CDIRECTOR->currentGameScene()) {
        for (std::vector<CGameObject*>::iterator it = CDIRECTOR->currentGameScene()->getGameObjects()->begin();
             it != CDIRECTOR->currentGameScene()->getGameObjects()->end();
             ++it) {
            MATRIXSTACK->pushMatrix();
            drawGameObject(*it);
            MATRIXSTACK->popMatrix();
        }
    }

    // draw sky box
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->translate(CDIRECTOR->getActiveCamera()->transform().localPosition());
    MATRIXSTACK->rotate(DEGTORAD(90.0f), 0, 0);
    MATRIXSTACK->scale(5000.0f);
    drawSkybox();
    MATRIXSTACK->popMatrix();

    //  draw debug lines
    CDIRECTOR->currentGameScene()->dynamicsWorld()->debugDrawWorld(); //  generates debug lines
    glDisable(GL_DEPTH_TEST);
    MATRIXSTACK->pushMatrix();
    bDebugDraw* debug_drawer = static_cast<bDebugDraw*>(CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer());
    RE_ASSERT(debug_drawer != NULL);
    CLRenderer::drawDebugLines(m_selectedProgram,
                               debug_drawer->lines());

    MATRIXSTACK->popMatrix();
    debug_drawer->clearLines();
    MATRIXSTACK->pushMatrix();
    drawWorldAxis();
    MATRIXSTACK->popMatrix();
    glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void QGLGameViewport::postDraw() {
    if (CDIRECTOR->gameState() == CDirector::RUNNING) {
        CDIRECTOR->currentGameScene()->postUpdate();
    }

    drawCrosshair(); // HACK
}

//-----------------------------------------------------------------------------

void QGLGameViewport::bindTextures() {
    const ImagesMapType* images_map = m_colladaScene->images();
    for (ImagesMapType::const_iterator i = images_map->begin();
         i != images_map->end();
         ++i) {
        CLImageDetails* img_detail = i->second;
        if (img_detail->texture == 0) {
            std::string fixed_filename = DirectoryHelper::getAssetsPath() + img_detail->filename;
            GLuint texture_id = bindTexture(QPixmap(QString::fromStdString(fixed_filename)),
                                            GL_TEXTURE_2D);
            img_detail->texture = texture_id;
        }
    }
}

//-----------------------------------------------------------------------------

void QGLGameViewport::drawCrosshair() {
    MATRIXSTACK->pushMatrix();
    MATRIXSTACK->identity();
    m_selectedProgram->setUniformValue(
                "cameraToClipMatrix", QMatrix4x4());
    CLRenderer::drawLine(
                m_texturedProgram,
                btVector3(-k_QGLGameViewport_CrosshairXLength, 0, 0),
                btVector3(k_QGLGameViewport_CrosshairXLength, 0, 0),
                &QColor(255, 255, 255));
    CLRenderer::drawLine(
                m_texturedProgram,
                btVector3(0, -k_QGLGameViewport_CrosshairYLength, 0),
                btVector3(0, k_QGLGameViewport_CrosshairYLength, 0),
                &QColor(255, 255, 255));
    MATRIXSTACK->popMatrix();
}

//-----------------------------------------------------------------------------

QGLGameViewport::~QGLGameViewport() {
    SAFE_RELEASE(m_bDebugDraw);
}
