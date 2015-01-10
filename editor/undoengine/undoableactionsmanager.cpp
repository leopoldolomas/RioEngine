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

#include "undoableaction.h"
#include "undoableactionsmanager.h"
#include "misc/genericshelper.hpp"

//-----------------------------------------------------------------------------

UndoableActionsManager::UndoableActionsManager() {
    m_undoActions = new std::vector<IUndoableAction*>();
    m_redoActions = new std::vector<IUndoableAction*>();
}

//-----------------------------------------------------------------------------

unsigned int UndoableActionsManager::getUndoActionsCount() {
    return m_undoActions->size();
}

//-----------------------------------------------------------------------------

unsigned int UndoableActionsManager::getRedoActionsCount() {
    return m_redoActions->size();
}

//-----------------------------------------------------------------------------

bool UndoableActionsManager::hasUndoActions() {
    return getUndoActionsCount() > 0;
}

//-----------------------------------------------------------------------------

bool UndoableActionsManager::hasRedoActions() {
    return getRedoActionsCount() > 0;
}

//-----------------------------------------------------------------------------

bool UndoableActionsManager::performUndo() {
    if (!hasUndoActions()) {
        return false;
    }

    // get the last undo action
    IUndoableAction* undo_action = m_undoActions->at(m_undoActions->size() - 1);

    // perform the undo action
    undo_action->performAction();

    // create the redo action
    IUndoableAction* redo_action = undo_action->getOppositeAction();

    // push redo action
    pushRedoAction(redo_action);

    // now we're done with the undo action, remove it from the vector and release it
    GenericsHelper::deleteObjectFromVector(m_undoActions, undo_action);

    return true;
}

//-----------------------------------------------------------------------------

bool UndoableActionsManager::performRedo() {
    if (!hasRedoActions())
        return false;

    // get the last undo action
    IUndoableAction* redo_action = m_redoActions->at(m_redoActions->size() - 1);

    // perform the undo action
    redo_action->performAction();

    // create the redo action
    IUndoableAction* undo_action = redo_action->getOppositeAction();

    // push redo action
    pushUndoAction(undo_action, false);

    // now we're done with the redo action, remove it from the vector and release it
    GenericsHelper::deleteObjectFromVector(m_redoActions, redo_action);

    return true;
}

//-----------------------------------------------------------------------------

const char* UndoableActionsManager::getHintForCurrentUndoAction() {
    if (!hasUndoActions()) {
        return "";
    }

    IUndoableAction* undo_action = m_undoActions->at(m_undoActions->size() - 1);
    return undo_action->getHint();
}

//-----------------------------------------------------------------------------

const char* UndoableActionsManager::getHintForCurrentRedoAction() {
    if (!hasRedoActions()) {
        return "";
    }

    IUndoableAction* redo_action = m_redoActions->at(m_redoActions->size() - 1);
    return redo_action->getHint();
}

//-----------------------------------------------------------------------------

void UndoableActionsManager::pushUndoAction(IUndoableAction* undo_action, bool clear_redo_stack) {
    if (clear_redo_stack && hasRedoActions()) {
        GenericsHelper::deleteVector(m_redoActions);
        m_redoActions = new std::vector<IUndoableAction*>();
    }

    m_undoActions->push_back(undo_action);
}

//-----------------------------------------------------------------------------

void UndoableActionsManager::pushUndoAction(IUndoableAction* undo_action) {
    pushUndoAction(undo_action, true);
}

//-----------------------------------------------------------------------------

void UndoableActionsManager::pushRedoAction(IUndoableAction* redo_action) {
    // TODO don't duplicate a redo action
    m_redoActions->push_back(redo_action);
}

//-----------------------------------------------------------------------------

void UndoableActionsManager::clear() {
    GenericsHelper::deleteVector(m_undoActions);
    GenericsHelper::deleteVector(m_redoActions);

    m_undoActions = new std::vector<IUndoableAction*>();
    m_redoActions = new std::vector<IUndoableAction*>();
}

//-----------------------------------------------------------------------------

UndoableActionsManager::~UndoableActionsManager() {
    GenericsHelper::deleteVector(m_undoActions);
    GenericsHelper::deleteVector(m_redoActions);
}
