//--------------------------------------------------------------- @License begins
// cocos2d for iPhone: http://www.cocos2d-iphone.org
// -> This file was ported to C++ by Leopoldo Lomas Flores just for fun.
//    I suggest you get the official C++ version of this file from:
//    http://www.cocos2d-x.org/
//
// Copyright (c) 2008-2011 Ricardo Quesada
// Copyright (c) 2011 Zynga Inc.
// Copyright (c) 2013-2014 Cocos2D Authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//--------------------------------------------------------------- @License ends

#include "cengine/cgameobject.h"
#include "cengine/cactioninterval.h"

// --------------------------------------------- CActionInterval implementation

CActionInterval::CActionInterval(float duration) : CFiniteTimeAction() {
    initWithDuration(duration);
}

//-----------------------------------------------------------------------------

void CActionInterval::initWithDuration(float duration) {
    m_duration = duration;

    // prevent division by 0
    // This comparison could be in step:, but it might decrease the performance
    // by 3% in heavy based action games.
    if ( m_duration == 0 )
        m_duration = FLT_EPSILON;
    m_elapsed = 0;
    m_firstTick = true;
}

//-----------------------------------------------------------------------------

void CActionInterval::startWithTarget(CGameObject* target) {
    CFiniteTimeAction::startWithTarget(target);

    m_elapsed = 0.0f;
    m_firstTick = true;

    emit(onActionStart(this));
}

//-----------------------------------------------------------------------------

bool CActionInterval::isDone() {
    return (m_elapsed >= m_duration);
}

//-----------------------------------------------------------------------------

void CActionInterval::step(float dt) {
    if (m_firstTick) {
        m_firstTick = false;
        m_elapsed = 0;
    } else
        m_elapsed += dt;

    update(std::max<float>(0, std::min<float>(1, m_elapsed / std::max<float>(m_duration, FLT_EPSILON))));

    if(isDone()) {
        emit onActionDone(this);
    }
}

//-----------------------------------------------------------------------------

CActionInterval* CActionInterval::reverse() {
    LOG("CActionInterval::reverse() not implemented");
    return NULL;
}

// -------------------------------------------------- CDelayTime implementation

CDelayTime::CDelayTime(float duration) : CActionInterval(duration) {
}

// ----------------------------------------------------- CMoveTo implementation

CMoveTo::CMoveTo(float duration, btVector3 &position) : CActionInterval(duration) {
    m_endPosition = position;
}

//-----------------------------------------------------------------------------

void CMoveTo::startWithTarget(CGameObject* target) {
    CActionInterval::startWithTarget(target);

    m_startPosition = target->transform().localPosition();
    m_delta = m_endPosition - m_startPosition;
}

void CMoveTo::update(float t) {
    btVector3 new_position = btVector3(m_startPosition.x() + m_delta.x() * t,
                                       m_startPosition.y() + m_delta.y() * t,
                                       m_startPosition.z() + m_delta.z() * t);

    m_target->transform().setLocalPosition(new_position);
}

// ----------------------------------------------------- CMoveBy implementation

CMoveBy::CMoveBy(float duration, btVector3 &position) {
    initWithDuration(duration);
    m_delta = position;
}

//-----------------------------------------------------------------------------

void CMoveBy::startWithTarget(CGameObject* target) {
    btVector3 d_tmp = m_delta;
    CMoveTo::startWithTarget(target);
    m_delta = d_tmp;
}

//-----------------------------------------------------------------------------

CActionInterval* CMoveBy::reverse() {
    return new CMoveBy(m_duration, m_delta * -1.0f);
}

// --------------------------------------------------- CRotateTo implementation

CRotateTo::CRotateTo(float duration, btVector3 &angle) {
    initWithDuration(duration, angle);
}

//-----------------------------------------------------------------------------

void CRotateTo::initWithDuration(float duration, btVector3& angle) {
    CActionInterval::initWithDuration(duration);
    m_dstAngle = angle;
}

//-----------------------------------------------------------------------------

void CRotateTo::startWithTarget(CGameObject *target) {
    CActionInterval::startWithTarget(target);
    m_startAngle = target->transform().localRotation();

    m_diffAngle = btVector3( m_dstAngle.y() - m_startAngle.y(),
                             m_dstAngle.x() - m_startAngle.x(),
                             m_dstAngle.z() - m_startAngle.z() );
}

//-----------------------------------------------------------------------------

void CRotateTo::update(float t) {
    float heading_diff  = m_diffAngle.y() * t;
    float pitch_diff    = m_diffAngle.x() * t;
    float bank_diff     = m_diffAngle.z() * t;

    btQuaternion y_rot = btQuaternion(btVector3(1, 0, 0), DEGTORAD(heading_diff));
    btQuaternion x_rot = btQuaternion(btVector3(0, 1, 0), DEGTORAD(pitch_diff));
    btQuaternion z_rot = btQuaternion(btVector3(0, 0, 1), DEGTORAD(bank_diff));

    btQuaternion final_rot = m_startAngle;

    final_rot = final_rot * y_rot;
    final_rot = x_rot     * final_rot;
    final_rot = final_rot * z_rot;

    m_target->transform().setLocalRotation(final_rot);
}

// --------------------------------------------------- CRotateBy implementation

CRotateBy::CRotateBy(float duration, btVector3 &angle) {
    initWithDuration(duration, angle);
}

//-----------------------------------------------------------------------------

void CRotateBy::initWithDuration(float duration, btVector3& angle) {
    CActionInterval::initWithDuration(duration);
    m_dstAngle = angle;
}

//-----------------------------------------------------------------------------

void CRotateBy::startWithTarget(CGameObject *target) {
    CActionInterval::startWithTarget(target);
    m_startAngle = target->transform().localRotation();
}

//-----------------------------------------------------------------------------

void CRotateBy::update(float t) {
    float heading_diff = m_dstAngle.y() * t;
    float pitch_diff   = m_dstAngle.x() * t;
    float bank_diff    = m_dstAngle.z() * t;

    btQuaternion y_rot = btQuaternion(btVector3(1, 0, 0), DEGTORAD(heading_diff));
    btQuaternion x_rot = btQuaternion(btVector3(0, 1, 0), DEGTORAD(pitch_diff));
    btQuaternion z_rot = btQuaternion(btVector3(0 ,0, 1), DEGTORAD(bank_diff));

    btQuaternion final_rot = m_startAngle;

    final_rot = final_rot * y_rot;
    final_rot = x_rot     * final_rot;
    final_rot = final_rot * z_rot;

    m_target->transform().setLocalRotation(final_rot);
}

// ---------------------------------------------------- CScaleTo implementation

CScaleTo::CScaleTo(float duration, float sx, float sy, float sz) {
    initWithDuration(duration, sx, sy, sz);
}

//-----------------------------------------------------------------------------

CScaleTo::CScaleTo(float duration, btVector3 &scale) {
    initWithDuration(duration, scale.x(), scale.y(), scale.z());
}

//-----------------------------------------------------------------------------

void CScaleTo::initWithDuration(float duration, float sx, float sy, float sz) {
    CActionInterval::initWithDuration(duration);
    m_endScale = btVector3(sx, sy, sz);
}

//-----------------------------------------------------------------------------

void CScaleTo::startWithTarget(CGameObject *target) {
    CActionInterval::startWithTarget(target);
    m_startScale = target->transform().localScale();
    m_deltaScale = m_endScale - m_startScale;
}

//-----------------------------------------------------------------------------

void CScaleTo::update(float t) {
    m_target->transform().setLocalScale(m_startScale + m_deltaScale * t);
}

// ---------------------------------------------------- CScaleBy implementation

CScaleBy::CScaleBy(float duration, float sx, float sy, float sz) :
    CScaleTo(duration, sx, sy, sz)
{}

//-----------------------------------------------------------------------------

CScaleBy::CScaleBy(float duration, btVector3 &scale) :
    CScaleTo(duration, scale)
{}

//-----------------------------------------------------------------------------

void CScaleBy::startWithTarget(CGameObject *target) {
    CScaleTo::startWithTarget(target);

    float dx = m_startScale.x() * m_endScale.x() - m_startScale.x();
    float dy = m_startScale.y() * m_endScale.y() - m_startScale.y();
    float dz = m_startScale.z() * m_endScale.z() - m_startScale.z();

    m_deltaScale = btVector3(dx, dy, dz);
}

//-----------------------------------------------------------------------------

CActionInterval* CScaleBy::reverse() {
    return new CScaleBy(m_duration,/*1*/ m_endScale);  // TODO verify this
}

// --------------------------------------------------- CSequence implementation

CFiniteTimeAction* CSequence::actions(CFiniteTimeAction* action, ...) {
    va_list params;
    va_start(params,action);

    CFiniteTimeAction* now, *prev_old = NULL;
    CFiniteTimeAction* prev = action;
    while ( action ) {
        now = va_arg(params,CFiniteTimeAction*);
        if ( now ) {
            prev_old = prev;
            prev = new CSequence();
            prev->autorelease();
            ((CSequence*)prev)->initHavingActions(prev_old, now);
        } else {
            break;
        }
    }
    va_end(params);
    return prev;
}

//-----------------------------------------------------------------------------

CSequence::CSequence() : CActionInterval() {
    m_actions[0] = NULL;
    m_actions[1] = NULL;
}

//-----------------------------------------------------------------------------

CSequence* CSequence::initHavingActions(CFiniteTimeAction* firstAction, CFiniteTimeAction* secondAction) {
    RE_ASSERT(firstAction && secondAction);
    RE_ASSERT(firstAction != m_actions[0] && firstAction != m_actions[1]);
    RE_ASSERT(secondAction != m_actions[0] && secondAction != m_actions[1]);

    float d = firstAction->getDuration() + secondAction->getDuration();

    initWithDuration(d);

    SAFE_RELEASE(m_actions[0]);
    SAFE_RELEASE(m_actions[1]);

    m_actions[0] = static_cast<CFiniteTimeAction*>(firstAction->retain());
    m_actions[1] = static_cast<CFiniteTimeAction*>(secondAction->retain());

    return this;
}

//-----------------------------------------------------------------------------

void CSequence::startWithTarget(CGameObject *target) {
    CActionInterval::startWithTarget(target);
    m_split = m_actions[0]->getDuration() / std::max(m_duration, FLT_EPSILON);
    m_last = -1;
}

//-----------------------------------------------------------------------------

void CSequence::update(float t) {
    int found = 0;
    float new_t = 0.0f;

    if ( t < m_split ) {
        // action[0]
        found = 0;
        if ( m_split != 0 )
            new_t = t / m_split;
        else
            new_t = 1;

    } else {
        // action[1]
        found = 1;
        if ( m_split == 1 )
            new_t = 1;
        else
            new_t = (t-m_split) / (1 - m_split );
    }

    if ( found==1 ) {

        if ( m_last == -1 ) {
            // action[0] was skipped, execute it.
            m_actions[0]->startWithTarget(m_target);
            m_actions[0]->update(1.0f);
            m_actions[0]->stop();
        } else if ( m_last == 0 )
        {
            // switching to action 1. stop action 0.
            m_actions[0]->update(1.0f);
            m_actions[0]->stop();
        }
    }

    // New action. Start it.
    if ( found != m_last )
        m_actions[found]->startWithTarget(m_target);

    m_actions[found]->update(new_t);
    m_last = found;
}

//-----------------------------------------------------------------------------

void CSequence::stop() {
    if ( m_last != -1 ) {
        m_actions[m_last]->stop();
    }

    CActionInterval::stop();
}

//-----------------------------------------------------------------------------

CActionInterval* CSequence::reverse() {
    CSequence* reversed_sequence = new CSequence();
    reversed_sequence->initHavingActions(m_actions[1]->reverse(), m_actions[0]->reverse());

    return reversed_sequence;
}

//-----------------------------------------------------------------------------

CSequence::~CSequence() {
    SAFE_RELEASE(m_actions[0]);
    SAFE_RELEASE(m_actions[1]);
}

// -------------------------------------------------- CCallFuncN implementation

CCallFuncN::CCallFuncN(CallbackMethod callback_method) {
    initWithDuration(0.0f);
    m_callbackMethod = callback_method;
}

//-----------------------------------------------------------------------------

void CCallFuncN::startWithTarget(CGameObject *target) {
    CActionInterval::startWithTarget(target);
}

//-----------------------------------------------------------------------------

void CCallFuncN::update(float) {
    m_callbackMethod();
}
