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

#include "cengine/cdirector.h"
#include "shootingrange/cenemylauncher.h"
#include "shootingrange/cplayer.h"
#include "shootingrange/ccustomscene.h"

//-----------------------------------------------------------------------------

CCustomScene::CCustomScene() : CGameScene() {
    m_player = NULL;
}

//-----------------------------------------------------------------------------

void CCustomScene::deployPlayer() {
    m_player = static_cast<CPlayer*>(createGameObjectWithNode("Cube", true, "CPlayer"));
    m_player->setVisible(false);
    m_player->setName("__player");
    m_player->setPosition(btVector3(0, 0.2f, 0));
    m_player->setbMass(1.0f);
    m_player->calcSize();
    m_player->deployIntoDynamicsWorld();
}

//-----------------------------------------------------------------------------

void CCustomScene::start() {
    CGameScene::start();

    // Enemy Launcher 1
    CEnemyLauncher* enemy = qobject_cast<CEnemyLauncher*>(getGameObjectHavingTag(1));
    if(enemy != NULL) {
        enemy->launchEnemy(1.0f, 1.5f, false);
    }

    // Enemy Launcher 2
    enemy = qobject_cast<CEnemyLauncher*>(getGameObjectHavingTag(2));
    if(enemy != NULL) {
        enemy->launchEnemy(2.5f, 1.75f, true);
        enemy->launchEnemy(3.7f, 1.75f, true);
        enemy->launchEnemy(4.9f, 1.75f, true);
    }

    // Enemy Launcher 3
    enemy = qobject_cast<CEnemyLauncher*>(getGameObjectHavingTag(3));
    if(enemy != NULL) {
        enemy->launchEnemy(5.3f, 1.25f, true);
        enemy->launchEnemy(6.1f, 1.5f, false);
    }

    // Enemy Launcher 4
    enemy = qobject_cast<CEnemyLauncher*>(getGameObjectHavingTag(4));
    if(enemy != NULL) {
        enemy->launchEnemy(7.5f, 1.0f, true);
        enemy->launchEnemy(9.5f, 1.0f, true);
    }

    // Enemy Launcher 5
    enemy = qobject_cast<CEnemyLauncher*>(getGameObjectHavingTag(5));
    if(enemy != NULL) {
        enemy->launchEnemy(9.7f, 1.5f, false);
        enemy->launchEnemy(11.2f, 0.85f, true);
        enemy->launchEnemy(11.7f, 0.85f, true);
    }
}

//-----------------------------------------------------------------------------

void CCustomScene::preUpdate() {
    if (!m_player) {
        deployPlayer();
    }

    CDIRECTOR->setActiveCameraHavingId(m_player->getCameraId());

    CGameScene::preUpdate();
}

//-----------------------------------------------------------------------------

void CCustomScene::clear() {
    CGameScene::clear();
    m_player = NULL;
}
