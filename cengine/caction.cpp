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

#include "cengine/caction.h"

// ----------------------------------------------------- CAction implementation

CAction::CAction() : CObject() {
}

//-----------------------------------------------------------------------------

CGameObject* CAction::getOriginalTarget() const {
    return m_originalTarget;
}

//-----------------------------------------------------------------------------

void CAction::startWithTarget(CGameObject *target) {
    m_originalTarget = m_target = target;
}

//-----------------------------------------------------------------------------

bool CAction::isDone() {
    return true;
}

//-----------------------------------------------------------------------------

void CAction::stop() {
    m_target = NULL;
}

//-----------------------------------------------------------------------------

void CAction::step(float) {
}

//-----------------------------------------------------------------------------

void CAction::update(float) {
}

// ------------------------------------------- CFiniteTimeAction implementation

CFiniteTimeAction::CFiniteTimeAction() : CAction() {
}

//-----------------------------------------------------------------------------

float CFiniteTimeAction::getDuration() const {
    return m_duration;
}

//-----------------------------------------------------------------------------

CFiniteTimeAction* CFiniteTimeAction::reverse() {
    return NULL;
}

//-----------------------------------------------------------------------------

CAction::~CAction() {
}
