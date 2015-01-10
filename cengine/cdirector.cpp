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
#include "constants.h"
#include "misc/genericshelper.hpp"
#include "inputmanager/inputmanager.h"
#include "cengine/cgamescene.h"
#include "cengine/cactionmanager.h"
#include "cengine/ccamera.h"
#include "cengine/time_.h"
#include "cengine/cdirector.h"

//-----------------------------------------------------------------------------

CDirector::CDirector(void) {
    m_cameras = new std::map<std::string, CCamera*>();
    m_actionManager = new CActionManager();

    createDefaultCamera();
}

//-----------------------------------------------------------------------------

CActionManager* CDirector::actionManager() const {
    return m_actionManager;
}

//-----------------------------------------------------------------------------

CDirector::GameState CDirector::gameState() const {
    return m_gameState;
}

//-----------------------------------------------------------------------------

void CDirector::setGameState(GameState game_state) {
    m_gameState = game_state;
}

//-----------------------------------------------------------------------------

CGameScene* CDirector::currentGameScene() const {
    return m_gameScene;
}

//-----------------------------------------------------------------------------

void CDirector::createDefaultCamera() {
    createCamera(k_CDirector_DefaultCameraId);
    setActiveCameraHavingId(k_CDirector_DefaultCameraId);
}

//-----------------------------------------------------------------------------

void CDirector::releaseCameras() {
    std::map<std::string, CCamera*>::iterator it;
    for(it = m_cameras->begin(); it != m_cameras->end(); ++it) {
        SAFE_RELEASE(it->second);
    }
}

//-----------------------------------------------------------------------------

CCamera* CDirector::createCamera(const std::string& camera_id) {
    if (existsCameraWithId(camera_id)) {
        return NULL; // TODO return the existing camera?
    }

    //  if the camera does not exist yet...
    CCamera* new_camera = new CCamera();
    m_cameras->insert(std::pair<std::string, CCamera*>(camera_id, new_camera));

    return new_camera;
}

//-----------------------------------------------------------------------------

CCamera* CDirector::getCameraWithId(const std::string& camera_id) const {
    if (!existsCameraWithId(camera_id)) { //  if the camera does not exist yet
        return getCameraWithId(k_CDirector_DefaultCameraId);  //  return the default camera
    }

    return m_cameras->at(camera_id);
}

//-----------------------------------------------------------------------------

const std::string& CDirector::getActiveCameraId() const {
    return m_activeCameraId;
}

//-----------------------------------------------------------------------------

bool CDirector::isActiveCameraWithId(const std::string& camera_id) const {
    return getActiveCameraId() == camera_id;
}

//-----------------------------------------------------------------------------

bool CDirector::existsCameraWithId(const std::string& camera_id) const {
    return m_cameras->count(camera_id) > 0;
}

//-----------------------------------------------------------------------------

CCamera* CDirector::getActiveCamera() const {
    return getCameraWithId(getActiveCameraId());
}

//-----------------------------------------------------------------------------

CCamera* CDirector::setActiveCameraHavingId(const std::string& camera_id) {
    m_activeCameraId = (existsCameraWithId(camera_id) ? camera_id : k_CDirector_DefaultCameraId);

    return getActiveCamera();
}

//-----------------------------------------------------------------------------

void CDirector::resetCameras() {
    std::map<std::string, CCamera*>::iterator it;
    for(it = m_cameras->begin(); it != m_cameras->end(); ++it) {
        it->second->reset();
    }
}

//-----------------------------------------------------------------------------

void CDirector::runWithScene(CGameScene* game_scene) {
    RE_ASSERT(game_scene);
    SAFE_RETAIN(game_scene);
    m_gameScene = game_scene;
    emit onRunScene(m_gameScene);
}

//-----------------------------------------------------------------------------

void CDirector::pushScene(CGameScene* game_scene) {
    //  TODO
}

//-----------------------------------------------------------------------------

void CDirector::popScene() {
    //  TODO
}

//-----------------------------------------------------------------------------

void CDirector::replaceScene() {
    //  TODO
}

//-----------------------------------------------------------------------------

void CDirector::resume() {
    setGameState(CDirector::RUNNING);
    emit onResumeScene(m_gameScene);
}

//-----------------------------------------------------------------------------

void CDirector::pause() {
    setGameState(CDirector::PAUSED);
    emit onPauseScene(m_gameScene);
}

//-----------------------------------------------------------------------------

void CDirector::step() {
    INPUTMANAGER->update();
    if (gameState() == RUNNING) {
        actionManager()->update(Time::getDeltaTime());
        currentGameScene()->preUpdate();
        currentGameScene()->update();
    }
}

//-----------------------------------------------------------------------------

CDirector::~CDirector() {
    SAFE_RELEASE(m_gameScene);
    SAFE_RELEASE(m_actionManager);
    GenericsHelper::deleteMap(m_cameras);
}
