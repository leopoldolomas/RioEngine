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

#ifndef RIOENGINE_CENGINE_CDIRECTOR_H_
#define RIOENGINE_CENGINE_CDIRECTOR_H_

#include <map>
#include "cengine/cobject.h"
#include "cengine/csingleton.h"

#define CDIRECTOR CSingleton<CDirector>::getSharedInstance()

class CCamera;
class CActionManager;
class CGameScene;

/**
 * Controls the lifetime of the running game scene and its behaviour
 */
class CDirector : public CObject {
    Q_OBJECT

public:
    enum GameState { RUNNING, PAUSED, STOPPED };

    CDirector();
    explicit CDirector(const CDirector&);
    CDirector& operator= (const CDirector&);

    GameState gameState() const;

    CGameScene* currentGameScene() const;
    virtual void runWithScene(CGameScene* game_scene);

    /**
     * TODO A queue (LIFO) would be perfect to manage the list of game scenes
     * to reproduce.
     */

    virtual void pushScene(CGameScene* game_scene);

    /**
     * TODO Forces the removal of the current game scene (e.g. game over)
     */
    virtual void popScene();
    virtual void replaceScene();
    virtual void resume();
    virtual void pause();

    /**
     * Performs a step. If you were to cap the game engine to 30FPS, then at
     * least 30 step() calls per second would be needed.
     */
    virtual void step();

    const std::string& getActiveCameraId() const;

    CActionManager* actionManager() const;

    bool     existsCameraWithId (const std::string& camera_id) const;
    CCamera* getCameraWithId    (const std::string& camera_id) const;
    CCamera* createCamera       (const std::string& camera_id);
    CCamera* getActiveCamera    () const;
    void     resetCameras       ();

    CCamera* setActiveCameraHavingId(const std::string& camera_id);
    bool     isActiveCameraWithId   (const std::string& camera_id) const;

    ~CDirector();

protected:
    virtual void setGameState(GameState game_state);
    void createDefaultCamera();
    void releaseCameras();

    GameState       m_gameState;
    CGameScene*     m_gameScene;
    CActionManager* m_actionManager;
    std::string     m_activeCameraId;
    std::map<std::string, CCamera*>* m_cameras;

signals:
    virtual void onRunScene     (CGameScene* game_scene);
    virtual void onPauseScene   (CGameScene* game_scene);
    virtual void onResumeScene  (CGameScene* game_scene);
    virtual void onEndScene     (CGameScene* game_scene);
};

#endif  // RIOENGINE_CENGINE_CDIRECTOR_H_
