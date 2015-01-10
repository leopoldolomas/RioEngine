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

#include "misc/genericshelper.hpp"
#include "cengine/cgameobject.h"
#include "caction.h"
#include "cactionmanager.h"

//-----------------------------------------------------------------------------

CActionManager::CActionManager(void) : CObject() {
    m_currentTarget = m_targets = NULL;
}

//-----------------------------------------------------------------------------

void CActionManager::removeActionAtIndex(unsigned int index, tHashElement* element) {
    CAction* action = element->actions.at(index);

    if ( action == element->currentAction && !element->currentActionSalvaged ) {
        element->currentAction->retain();
        element->currentActionSalvaged = true;
    }

    GenericsHelper::deleteObjectFromVector(&element->actions, action);

    // update actionIndex in case we are in tick:, looping over the actions
    if ( element->actionIndex >= index )
        element->actionIndex--;

    if ( element->actions.size() == 0 ) {
        if ( m_currentTarget == element )
            m_currentTargetSalvaged = true;
        else
            deleteHashElement(element);
    }
}

//-----------------------------------------------------------------------------

void CActionManager::deleteHashElement(tHashElement* element) {
    HASH_DEL(m_targets, element);
    SAFE_RELEASE(element->target);
    SAFE_RELEASE(element);
}

//-----------------------------------------------------------------------------

void CActionManager::pauseTarget(CGameObject* target) {
    tHashElement* element = NULL;
    HASH_FIND_INT(m_targets, &target, element);
    if (element)
        element->paused = true;
}

//-----------------------------------------------------------------------------

void CActionManager::resumeTarget(CGameObject* target) {
    tHashElement* element = NULL;
    HASH_FIND_INT(m_targets, &target, element);
    if (element)
        element->paused = false;
}

//-----------------------------------------------------------------------------

// TODO
//CPointerArray<CGameObject>* CActionManager::pauseAllRunningActions() {
//    std::vector<CGameObject*>* objects_paused = new std::vector<CGameObject*>();

//    for ( tHashElement *element=m_targets;
//        element != NULL;
//        element = (tHashElement*)element->hh.next ) {
//        if ( !element->paused ) {
//            element->paused = true;
//            objects_paused->push_back(element->target);
//        }
//    }
//    return CPointerArray<CGameObject>::fromStdVector( objects_paused );
//}

//-----------------------------------------------------------------------------

void CActionManager::addAction(CAction* action, CGameObject* target, bool paused) {
    RE_ASSERT(action);
    RE_ASSERT(target);

    SAFE_RETAIN(action);

    tHashElement *element = NULL;
    HASH_FIND_INT(m_targets, &target, element);
    if ( ! element ) {
        element = new tHashElement;
        element->paused = paused;
        element->target = (CGameObject*)target->retain();
        HASH_ADD_INT(m_targets, target, element);
    }

    element->actions.push_back(action);
    action->startWithTarget(target);
}

//-----------------------------------------------------------------------------

void CActionManager::removeAllActions() {
    for (tHashElement *element=m_targets; element != NULL; ) {
        CGameObject* target = element->target;
        element = (tHashElement*)element->hh.next;
        removeAllActionsFromTarget(target);
    }
}

//-----------------------------------------------------------------------------

void CActionManager::removeAllActionsFromTarget(CGameObject* target) {
    // explicit NULL handling
    if ( target == NULL )
        return;

    tHashElement *element = NULL;
    HASH_FIND_INT(m_targets, &target, element);
    if ( element ) {
        //std::vector<CAction*> element_actions = element->actions;
        if ( std::find(element->actions.begin(),
            element->actions.end(),
            element->currentAction) != element->actions.end() ) {
            element->currentAction->retain();
            m_currentTargetSalvaged = true;
        }
        for (std::vector<CAction*>::iterator it = element->actions.begin();
            it != element->actions.end();
            ++it) {
            SAFE_RELEASE(*it);
        }
        if ( m_currentTarget == element)
            m_currentTargetSalvaged = true;
        else
            deleteHashElement(element);
    }
}

//-----------------------------------------------------------------------------

void CActionManager::removeAction(CAction* action) {
    // explicit NULL handling
    if (!action)
        return;

    tHashElement* element = NULL;
    CGameObject* target = action->getOriginalTarget();
    HASH_FIND_INT(m_targets, &target, element);

    if ( element ) {
        int index = GenericsHelper::getIndexOfObjectInVector<CAction>(&element->actions, action);
        if ( index > -1 )
            removeActionAtIndex(index, element);
    }
}

//-----------------------------------------------------------------------------

void CActionManager::removeActionByTag(unsigned int tag, CGameObject* target) {
    //RE_ASSERT( aTag != kCCActionTagInvalid, @"Invalid tag");
    RE_ASSERT( target );

    tHashElement* element = NULL;
    HASH_FIND_INT(m_targets, &target, element);

    if ( element ) {
        CAction* a = NULL;
        for (std::vector<CAction*>::iterator it = element->actions.begin();
            it != element->actions.end();
            ++it) {
            a = *it;

            if ( a->tag == tag && a->getOriginalTarget() == target) {
                removeActionAtIndex(std::distance(element->actions.begin(), it), element);
                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------

CAction* CActionManager::getActionByTag(unsigned int tag, CGameObject* target) {
    //RE_ASSERT( aTag != kCCActionTagInvalid, @"Invalid tag");

    tHashElement* element = NULL;
    HASH_FIND_INT(m_targets, &target, element);

    if ( element ) {
        if ( element->actions.size() == 0 ) {
            CAction* iterated_action = NULL;
            for (std::vector<CAction*>::iterator it = element->actions.begin();
            it != element->actions.end();
            ++it) {
                iterated_action = *it;
                if ( iterated_action->tag == tag )
                    return iterated_action;
            }
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------

unsigned int CActionManager::numberOfRunningActionsInTarget(CGameObject* target) {
    tHashElement* element = NULL;
    HASH_FIND_INT(m_targets, &target, element);

    if ( element )
        return ( element->actions.size() > 0 ? element->actions.size() : 0 );

    return 0;
}

//-----------------------------------------------------------------------------

void CActionManager::update(double dt) {
    for ( tHashElement *elt = m_targets; elt != NULL; ) {
        m_currentTarget = elt;
        m_currentTargetSalvaged = false;

        if ( !m_currentTarget->paused ) {
            // The 'actions' ccArray may change while inside this loop.
            for ( m_currentTarget->actionIndex = 0;
                m_currentTarget->actionIndex < m_currentTarget->actions.size();
                m_currentTarget->actionIndex++) {
                m_currentTarget->currentAction = m_currentTarget->actions.at(m_currentTarget->actionIndex);
                m_currentTarget->currentActionSalvaged = false;

                m_currentTarget->currentAction->step(dt);

                if ( m_currentTarget->currentActionSalvaged ) {
                    // The currentAction told the node to remove it. To prevent the action from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    SAFE_RELEASE(m_currentTarget->currentAction);
                } else if ( m_currentTarget->currentAction->isDone() )
                {
                    m_currentTarget->currentAction->stop();

                    CAction *a = m_currentTarget->currentAction;
                    // Make currentAction NULL to prevent removeAction from salvaging it.
                    m_currentTarget->currentAction = NULL;
                    removeAction(a);
                }

                m_currentTarget->currentAction = NULL;
            }
        }
        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashElement*)elt->hh.next;

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if ( m_currentTargetSalvaged && m_currentTarget->actions.size() == 0 )
            deleteHashElement(m_currentTarget);
    }
    // issue #635
    m_currentTarget = NULL;
}

//-----------------------------------------------------------------------------

CActionManager::~CActionManager(void) {
    removeAllActions();
}
