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

#include <irrklang/irrKlang.h>
#include "cengine/cactioninterval.h"
#include "cengine/csoundplayer.h"
#include "shootingrange/cbullet.h"
#include "shootingrange/cenemytarget.h"

//-----------------------------------------------------------------------------

CEnemyTarget::CEnemyTarget() : CEnemy() {
    setDelayTime(0.0f);
    setExposureTime(1.0f);
    setSlideLength(0.0f);
    setDoSlide(false);
    m_slideSoundId = -1;
}

//-----------------------------------------------------------------------------

float CEnemyTarget::delayTime() const {
    return m_delayTime;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::setDelayTime(float delayTime) {
    m_delayTime = delayTime;
}

//-----------------------------------------------------------------------------

float CEnemyTarget::exposureTime() const {
    return m_exposureTime;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::setExposureTime(float exposureTime) {
    m_exposureTime = exposureTime;
}

//-----------------------------------------------------------------------------

float CEnemyTarget::slideLength() const {
    return m_slideLength;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::setSlideLength(float slideLength) {
    m_slideLength = slideLength;
}

//-----------------------------------------------------------------------------

bool CEnemyTarget::doSlide() const {
    return m_doSlide;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::setDoSlide(bool doSlide) {
    m_doSlide = doSlide;
}

//-----------------------------------------------------------------------------

float CEnemyTarget::launcherHeight() const {
    return m_launcherHeight;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::setLauncherHeight(float launcherHeight) {
    m_launcherHeight = launcherHeight;
}

//-----------------------------------------------------------------------------

void CEnemyTarget::launch() {
    if(doSlide()) {
        m_transform.moveX(-slideLength() / 2.0f);
    }

    bulletProperties.setActivationState(CBulletProperties::ActivationState::DisableDeactivation);

    CActionInterval* delay_action = new CDelayTime(delayTime());
    CActionInterval* go_up   = new CMoveBy(0.2f, btVector3(0, size().y(), 0.0f));
    connect(go_up, SIGNAL(onActionStart(CAction*)), this, SLOT(onGoUpActionStart(CAction*)));

    CActionInterval* slide   = new CMoveBy(exposureTime(), btVector3(slideLength(), 0, 0.0f));

    if(doSlide()) {
        connect(slide, SIGNAL(onActionStart(CAction*)), this, SLOT(onSlideActionStart(CAction*)));
    }

    CActionInterval* go_down = go_up->reverse();
    connect(go_down, SIGNAL(onActionStart(CAction*)), this, SLOT(onGoDownActionStart(CAction*)));

    CFiniteTimeAction* sequence = CSequence::actions(delay_action, go_up, slide, go_down, NULL);
    connect(sequence, SIGNAL(onActionDone(CAction*)), this, SLOT(onSequenceDone(CAction*)));

    runAction(sequence);

    SAFE_RELEASE(delay_action);
    SAFE_RELEASE(go_up);
    SAFE_RELEASE(slide);
    SAFE_RELEASE(go_down);
}

//-----------------------------------------------------------------------------

void CEnemyTarget::onGoUpActionStart(CAction* action) {
    deployIntoDynamicsWorld();
    CSOUNDPLAYER->play2D("enter.wav");
}

//-----------------------------------------------------------------------------

void CEnemyTarget::stopAllSounds() {
    if(doSlide() && m_slideSoundId > -1) {
        irrklang::ISound* sound = CSOUNDPLAYER->getISound(m_slideSoundId);
        sound->stop();
        sound->drop();
        m_slideSoundId = -1;
    }
}

//-----------------------------------------------------------------------------

void CEnemyTarget::onGoDownActionStart(CAction* action) {
    stopAllSounds();
    CSOUNDPLAYER->play2D("exit.wav");
}

//-----------------------------------------------------------------------------

void CEnemyTarget::onSequenceDone(CAction* action) {
    remove();
}

//-----------------------------------------------------------------------------

void CEnemyTarget::onSlideActionStart(CAction* action) {
    m_slideSoundId = CSOUNDPLAYER->play2D("slide.wav", true, false, true);
}

//-----------------------------------------------------------------------------

void CEnemyTarget::aboutToBeRemoved() {
    stopAllSounds();
}

//-----------------------------------------------------------------------------

void CEnemyTarget::damageReceived(int damage) {
    CSOUNDPLAYER->play2D("bombexplosion.wav");
    remove();
}
