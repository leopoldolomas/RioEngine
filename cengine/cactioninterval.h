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

#ifndef RIOENGINE_CENGINE_CACTIONINTERVAL_H_
#define RIOENGINE_CENGINE_CACTIONINTERVAL_H_

#include "bullet/btBulletDynamicsCommon.h"
#include "cengine/fastdelegate.h"
#include "cengine/caction.h"

class CGameObject;

class CActionInterval : public CFiniteTimeAction {    
    Q_OBJECT
public:
    CActionInterval() : CFiniteTimeAction() { }
    explicit CActionInterval(float duration);

    virtual void initWithDuration(float duration);
    virtual void startWithTarget(CGameObject* target);
    virtual bool isDone();
    virtual void step(float dt);
    virtual CActionInterval* reverse();

private:
    float m_elapsed;
    bool  m_firstTick;
};

class CDelayTime : public CActionInterval {
public:
    CDelayTime() : CActionInterval() {}
    CDelayTime(float duration);
};

class CMoveTo : public CActionInterval {
public:
    CMoveTo() : CActionInterval() {}
    CMoveTo(float duration, btVector3& position);

    virtual void startWithTarget(CGameObject* target);
    virtual void update(float t);

protected:
    btVector3 m_endPosition;
    btVector3 m_startPosition;
    btVector3 m_delta;
};

class CMoveBy : public CMoveTo {
public:
    CMoveBy() : CMoveTo() {}
    CMoveBy(float duration, btVector3& position);

    virtual void startWithTarget(CGameObject* target);
    virtual CActionInterval* reverse();
};

class CRotateTo : public CActionInterval {
public:
    CRotateTo() { RE_ASSERT(false); /*Default constructor not supported*/ }
    CRotateTo(float duration, btVector3& angle);

    virtual void initWithDuration(float duration, btVector3& angle);
    virtual void startWithTarget(CGameObject* target);
    virtual void update(float t);

private:
    btVector3    m_dstAngle;
    btQuaternion m_startAngle;
    btVector3    m_diffAngle;
};

class CRotateBy : public CRotateTo {
public:
    CRotateBy() { RE_ASSERT(false); /*Default constructor not supported*/ }
    CRotateBy(float duration, btVector3& angle);

    virtual void initWithDuration(float duration, btVector3& angle);
    virtual void startWithTarget(CGameObject* target);
    virtual void update(float t);

private:
    btVector3    m_dstAngle;
    btQuaternion m_startAngle;
};

class CScaleTo : public CActionInterval {
public:
    CScaleTo() { RE_ASSERT(false); /*Default constructor not supported*/ }
    CScaleTo(float duration, float sx, float sy, float sz);
    CScaleTo(float duration, btVector3& scale);

    virtual void initWithDuration(float duration, float sx, float sy, float sz);
    virtual void startWithTarget(CGameObject* target);
    virtual void update(float);

protected:
    btVector3 m_startScale;
    btVector3 m_endScale;
    btVector3 m_deltaScale;
};

class CScaleBy : public CScaleTo {
public:
    CScaleBy(float duration, float sx, float sy, float sz);
    CScaleBy(float duration, btVector3& scale);

    virtual void startWithTarget(CGameObject* target);
    virtual CActionInterval* reverse();
};

class CSequence : public CActionInterval {
    Q_OBJECT
public:
    CSequence();
    ~CSequence();

    static CFiniteTimeAction* actions(CFiniteTimeAction* action, ...);

    CSequence* initHavingActions(CFiniteTimeAction* firstAction, CFiniteTimeAction* secondAction);
    virtual void startWithTarget(CGameObject* target);
    virtual void update(float t);
    virtual void stop();
    virtual CActionInterval* reverse();

private:
    CFiniteTimeAction *m_actions[2];
    float m_split;
    unsigned int m_last;
};

typedef fastdelegate::FastDelegate0<> CallbackMethod;
#define MAKE_CALLBACK_METHOD(x) fastdelegate::MakeDelegate(this, x)

class CCallFuncN : public CActionInterval {
public:
    CCallFuncN() { LOG("Default constructor not supported"); }
    explicit CCallFuncN(CallbackMethod);

    virtual void startWithTarget(CGameObject * target);
    virtual void update(float);

private:
    CallbackMethod m_callbackMethod;
};

#endif // RIOENGINE_CENGINE_CACTIONINTERVAL_H_
