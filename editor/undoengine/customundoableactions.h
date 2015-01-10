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

#ifndef CUSTOMUNDOABLEACTIONS_H
#define CUSTOMUNDOABLEACTIONS_H

#include "cengine/cgameobject.h"
#include "cengine/cgamescene.h"
#include "editor/undoengine/undoableaction.h"
#include "editor/editorwindow.h"

/**
 * TranslateAction is created when the user modifies the position of any game
 * object in the editor.
 */
class TranslateAction : public UndoableAction<CGameObject*, btVector3> {
public:
    TranslateAction(CGameObject* game_obj, btVector3 translate_vec) :
        UndoableAction(game_obj, translate_vec) {
        m_oldValue = game_obj->transform().localPosition();
    }

    virtual const char* getHint() const {
        return "Translate";
    }

    virtual void performAction() {
        m_target->transform().setLocalPosition(m_value);
    }

    virtual IUndoableAction* getOppositeAction() {
        return new TranslateAction(m_target, m_oldValue);
    }

protected:
    btVector3 m_oldValue;
};


/**
 * ScaleAction is created when the user modifies the scale of any game object
 * in the editor.
 */
class ScaleAction : public UndoableAction<CGameObject*, btVector3> {
public:
    ScaleAction(CGameObject* game_obj, btVector3 translate_vec, bool calc_size_on_execute = false) : UndoableAction(game_obj, translate_vec) {
        m_oldValue = game_obj->transform().localScale();
        m_calcSizeOnExecute = calc_size_on_execute;
    }

    virtual const char* getHint() const {
        return "Scale";
    }

    virtual void performAction() {
        m_target->transform().setLocalScale(m_value);
        if (m_calcSizeOnExecute && m_target->bPhysicsEnabled()) {
            m_target->removeFromDynamicsWorld();
            m_target->calcSize();
            if(m_target->bulletProperties.physicsEnabled()) {
                m_target->deployIntoDynamicsWorld();
            }
        }
    }

    virtual IUndoableAction* getOppositeAction() {
        return new ScaleAction(m_target, m_oldValue, m_calcSizeOnExecute);
    }

protected:
    btVector3 m_oldValue;
    bool      m_calcSizeOnExecute;
};

/**
 * AddOrRemoveGameObjectAction is created when the user creates or deletes a
 * game object in the editor.
 */
class AddOrRemoveGameObjectAction: public UndoableAction<CGameScene*, CGameObject*> {
public:
    AddOrRemoveGameObjectAction(EditorWindow* editor_window, CGameScene* scene,
                                CGameObject* game_obj, bool add_mode, std::string hint = std::string()) :
        UndoableAction(scene, game_obj) {
        m_editorWindow = editor_window;
        m_addMode = add_mode;
        m_hint = hint;
    }

    virtual void performAction() {
        if (m_addMode) {
            m_target->addGameObject(m_value);
            m_editorWindow->selectGameObject(m_value);
        } else {
            m_target->removeGameObject(m_value->uid());
            m_editorWindow->repopulatePropertyBrowser();
            m_editorWindow->selectMostRecentGameObject();
        }
    }

    virtual IUndoableAction* getOppositeAction() {
        return new AddOrRemoveGameObjectAction(m_editorWindow, m_target, m_value, !m_addMode, m_hint);
    }

    virtual const char* getHint() const {
        return m_hint.c_str();
    }

    virtual void setHint(const std::string& hint) {
        m_hint = hint;
    }

    virtual void* getTarget() const {
        return m_target;
    }

    /**
    Call this method until IUndoableAction goes out of scope
    */
    void releaseGameObject() {
        SAFE_RELEASE(m_value);
    }

private:
    bool m_addMode;
    std::string m_hint;
    EditorWindow* m_editorWindow;
};

/**
 * DuplicateGameObject is created when the user duplicates a game object in the
 * editor.
 * TODO Remove EditorWindow var from CActions (use Qt Signals & Slots)
 */
class DuplicateGameObject: public AddOrRemoveGameObjectAction {
private:
    bool m_addMode;
    EditorWindow* m_editorWindow;

public:
    DuplicateGameObject(EditorWindow* editor_window, CGameScene* scene, CGameObject* game_obj, bool add_mode) :
        AddOrRemoveGameObjectAction(editor_window, scene, game_obj, add_mode)
    {}

    virtual const char* getHint() const {
        return "Duplicate GameObject";
    }
};

/**
 * RotateAction is created when the user rotates a game object in the editor.
 */
class RotateAction : public UndoableAction<CGameObject*, btQuaternion> {
public:
    RotateAction(CGameObject* game_obj, btQuaternion rotation) : UndoableAction(game_obj, rotation) {
        m_oldValue = game_obj->transform().localRotation();
    }

    virtual const char* getHint() const {
        return "Rotation";
    }

    virtual void performAction() {
        m_target->transform().setLocalRotation(m_value);
    }

    virtual IUndoableAction* getOppositeAction() {
        return new RotateAction(m_target, m_oldValue);
    }

protected:
    btQuaternion m_oldValue;
};

/**
 * InspectorAction is created when the user modifies a game object's property
 * in the Property Inspector.
 */
class InspectorAction : public UndoableAction<QObject*, QVariant> {
    Q_OBJECT
public:
    InspectorAction(QObject* game_obj, QString property_name, QVariant value) : UndoableAction(game_obj, value) {
        m_propertyName = property_name;
        m_oldValue = game_obj->property(qPrintable(property_name));

        connect(this, SIGNAL(actionPerformed(QObject*, QString, QVariant, QVariant)),
                EditorWindow::currentInstance, SLOT(onInspectorActionPeformed(QObject*, QString, QVariant, QVariant)));
    }

    virtual const char* getHint() const {
        return "Inspector";
    }

    virtual void performAction() {
        m_target->setProperty(qPrintable(m_propertyName), m_value);
        emit actionPerformed(m_target, m_propertyName, m_value, m_oldValue);
    }

    virtual IUndoableAction* getOppositeAction() {
        return new InspectorAction(m_target, m_propertyName, m_oldValue);
    }

protected:
    QString m_propertyName;
    QVariant m_oldValue;

Q_SIGNALS:
    void actionPerformed(QObject*, QString, QVariant, QVariant);
};

#endif  // CUSTOMUNDOABLEACTIONS_H
