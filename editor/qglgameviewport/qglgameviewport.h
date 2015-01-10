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

#ifndef RIOENGINE_EDITOR_QGLGAMEVIEWPORT_QGLGAMEVIEWPORT_H_
#define RIOENGINE_EDITOR_QGLGAMEVIEWPORT_QGLGAMEVIEWPORT_H_

#include "editor/qglbaseviewport/qglbaseviewport.h"

class bDebugDraw;
class CCamera;
class CGameObject;
class CGameScene;
class CLScene;

class QGLGameViewport : public QGLBaseViewport {
    Q_OBJECT

public:
    QGLGameViewport(CLScene* collada_scene, QGLWidget* shared_widget);

    void setColladaScene(CLScene* collada_scene);
    CLScene* colladaScene() const;

    ~QGLGameViewport();

protected:
    // From QGLBaseViewport:
    virtual void initializeGL();
    virtual void preDraw();
    virtual void step();
    virtual void customDraw();
    virtual void postDraw();
    // ---------------------

    virtual void drawSkybox();
    virtual void drawWorldAxis();
    virtual void drawGameObject(CGameObject* game_object);

    /**
     *  Draws the crosshair during gameplay
     *  TODO The crosshair must be drawn using a SpriteRenderer/GUIManager or
     *  something more appropiate, this is just a workaround.
     */
    virtual void drawCrosshair();

    /**
     * Binds all textures to the OpenGL context. It works but it is not the best
     * approach. Most of the textures may never be used in the game scene.
     * This method is just a leftover, now I don't have time to code a better
     * approach.
     */
    void bindTextures();

    CLScene* m_colladaScene;

private:
    bDebugDraw* m_bDebugDraw;

signals:
    void afterInitializeGL(QGLGameViewport* game_viewport);
};

#endif  // RIOENGINE_EDITOR_QGLGAMEVIEWPORT_QGLGAMEVIEWPORT_H_
