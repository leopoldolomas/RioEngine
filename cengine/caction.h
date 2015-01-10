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

#ifndef RIOENGINE_CENGINE_CACTION_H_
#define RIOENGINE_CENGINE_CACTION_H_

#include "cengine/CObject.h"

class CGameObject;

/** Base class for CCAction objects.
*/
class CAction : public CObject {
    Q_OBJECT
public:
    CAction();

    /** The action tag. An identifier of the action */
    unsigned int tag;

    CGameObject* getOriginalTarget() const;

    /**
    * return YES if the action has finished
    */
    virtual bool isDone();

    /**
    * called before the action start. It will also set the target.
    */
    virtual void startWithTarget(CGameObject* target);

    /**
    * called after the action has finished. It will set the 'target' to nil.
    * IMPORTANT: You should never call "[action stop]" manually.
    * Instead, use: "[target stopAction:action];"
    */
    virtual void stop();

    /**
    * called every frame with its delta time. DON'T override unless you know
    * what you are doing.
    */
    virtual void step(float dt);

    /**
    * called once per frame. time a value between 0 and 1
    * For example:
    * 0 means that the action just started
    * 0.5 means that the action is in the middle
    * 1 means that the action is over
    */
    virtual void update(float t);

    virtual ~CAction();

protected:
    /**
     * The original target, since target can be nil.
     */
    CGameObject* m_originalTarget;

    /**
     * The "target". The action will modify the target properties.
     * The target will be set with the 'startWithTarget' method.
     * When the 'stop' method is called, target will be set to nil.
     * The target is 'assigned', it is not 'retained'.
     */
    CGameObject* m_target;

signals:
    /**
     * onActionStart callback
     */
    void onActionStart(CAction* action);

    /**
     * onActionDone callback
     */
    void onActionDone(CAction* action);
};


/** Base class actions that do have a finite time duration.
 * Possible actions:
 * - An action with a duration of 0 seconds
 * - An action with a duration of 35.5 seconds
 * Infitite time actions are valid
 */
class CFiniteTimeAction : public CAction {
    Q_OBJECT
public:
    CFiniteTimeAction();

    float getDuration() const;

    /** returns a reversed action */
    virtual CFiniteTimeAction* reverse();

protected:
    // duration in seconds
    float m_duration;
};

#endif // RIOENGINE_CENGINE_CACTION_H_
