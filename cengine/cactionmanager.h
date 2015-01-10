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

#ifndef RIOENGINE_CENGINE_CACTIONMANAGER_H_
#define RIOENGINE_CENGINE_CACTIONMANAGER_H_

#include <vector>
#include "misc/uthash.h"

class CGameObject;
class CAction;

typedef struct _hashElement {
    std::vector<CAction*> actions;
    unsigned int actionIndex;
    bool currentActionSalvaged;
    bool paused;
    UT_hash_handle    hh;
    CGameObject* target;
    CAction* currentAction;
} tHashElement;

class CActionManager : public CObject {
public:
    CActionManager(void);

    // actions
    /**
    * Adds an action with a target.
    * If the target is already present, then the action will be added to the
    * existing target. If the target is not present, a new instance of this
    * target will be created either paused or paused, and the action will be
    * added to the newly created target.
    * When the target is paused, the queued actions won't be 'ticked'.
    */
    void addAction(CAction* action, CGameObject* target, bool paused);

    /**
     * Removes all actions from all the targers.
     */
    void removeAllActions();

    /**
     * Removes all actions from a certain target.
     * All the actions that belongs to the target will be removed.
     */
    void removeAllActionsFromTarget(CGameObject* target);

    /**
     * Removes an action given an action reference.
     */
    void removeAction(CAction* action);

    /**
     * Removes an action given its tag and the target
     */
    void removeActionByTag(unsigned int tag, CGameObject* target);

    /**
     * Gets an action given its tag an a target
     * @return the Action the with the given tag
     */
    CAction* getActionByTag(unsigned int tag, CGameObject* game_object);

    /**
     * Returns the numbers of actions that are running in a certain target
     * Composable actions are counted as 1 action. Example:
     *     If you are running 1 Sequence of 7 actions, it will return 1.
     *     If you are running 7 Sequences of 2 actions, it will return 7.
     */
    unsigned int numberOfRunningActionsInTarget(CGameObject* target);

    /**
     * Pauses the target: all running actions and newly added actions will be paused.
     */
    void pauseTarget(CGameObject* target);

    /**
     * Resumes the target. All queued actions will be resumed.
     */
    void resumeTarget(CGameObject* target);

    /**
     * Pauses all running actions, returning a list of targets whose actions were paused.
     */
    //    CPointerArray<CGameObject>* pauseAllRunningActions();

    void update(double dt);

    /**
     * Resume a set of targets (convenience function to reverse a pauseAllRunningActions call)
     */
    //-(void) resumeTargets:(NSSet *)targetsToResume;

    ~CActionManager(void);

private:
    void removeActionAtIndex(unsigned int index, tHashElement* element);
    void deleteHashElement(tHashElement* element);

    tHashElement*   m_targets;
    tHashElement*   m_currentTarget;
    bool            m_currentTargetSalvaged;
};

#endif RIOENGINE_CENGINE_CACTIONMANAGER_H_
