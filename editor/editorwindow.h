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

#ifndef RIOENGINE_EDITOR_EDITORWINDOW_H_
#define RIOENGINE_EDITOR_EDITORWINDOW_H_

#include <QStack>
#include <QString>
#include <QMainWindow>
#include "editor/qpropertybrowser/qpropertybrowser.h"
#include "editor/qglgameeditorviewport/qglgameeditorviewport.h"

class QGLGameEditorViewport;
class QScriptListWidget;
class UndoableActionsManager;
class QGameWindow;

namespace Ui {
class EditorWindow;
}

struct PropertyChange {
    QObject* obj;
    QString name;
    QVariant value, oldValue;

    PropertyChange() {
        obj = NULL;
        name = QString();
        value = oldValue = QVariant();
    }
};

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    enum State { EDITING, PLAYING };

    explicit EditorWindow(QWidget *parent = 0);

    State currentState() const;

    static EditorWindow* currentInstance;

    bool areResourcesLoaded();
    void loadResourcesIfNeeded();

    void selectGameObjectHavingUID(uint gameobj_uid);
    void selectGameObject(CGameObject* game_object);

    void setSelectedTransformMode(Transform::TransformMode transform_mode);
    void setSelectedManipulatorType(QGLGameEditorViewport::ManipulatorTypes manipulator_type);

    void updateGUIState();
    void clearSelection();
    void repopulatePropertyBrowser();
    void selectMostRecentGameObject();
    void removeChildFromInspector(QString child_name);
    void addSceneFilenameToRecentSceneList(QString scene_filename);

    ~EditorWindow();

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    CGameObject* addNodeHavingId(std::string node_id);
    const CGameObject* getSelectedGameObject();

    void setState(State);
    void loadScene(QString filename);
    void saveLayout();
    void restoreLayout();
    void setUpBulletToolbar();
    void deployOrRemoveGameObjInDynamicsWorld(CGameObject* game_obj, bool deploy);
    void createActionForDebugModeMenu(QMenu* menu, QString text, int bitmask, int option);
    void regenerateBodyIfNeeded(CGameObject* game_object, QString property_name);
    void addDefaultPlane();
    void stopGame();
    void showCreateGameObjectMenu();
    void updateMemoryLeaksDetectionStatus();
    void loadResources();

    std::string m_currentSceneFilename; // TODO change to QString?

    Ui::EditorWindow *ui;
    State m_state;
    QTimer* iTimer;
    QPropertyBrowser* m_propertyBrowser;
    QGLGameEditorViewport* m_glViewport;
    QGameWindow* m_currentGameWindow;
    PropertyChange m_lastPropertyChange;
    QList<CGameObject> m_previousSceneGameObjects;
    QStack<QString> m_recentScenes;
    QTimer* m_timer;
    QTimer* m_tmrBulletProfiler;
    QScriptListWidget* m_scriptListWidget;

    bool m_isInitialized;
    bool m_isEditingProperty;
    bool m_ignoreOnInspectorActionPerformed;
    bool m_isClosing;
    bool m_memoryLeaksDetection;

public slots:
    void onGameWindowDestroyedCallback(QObject* obj = 0);
    void propertyChanged(QObject * obj, QString property_name, QVariant value, QVariant old_value);
    void selectGameObject(QString);
    void onEditPropertyBegin();
    void onEditPropertyEnd();

protected slots:
    void timeout(void);

    // TODO remove unused slots
private slots:
    void on_actionNewScene_triggered();
    void on_actionSave_Scene_2_triggered();
    void on_actionExit_triggered();
    void on_actionSave_As_triggered();
    void on_actionAbout_triggered();
    void on_actionReset_Camera_triggered();
    void on_actionPlay_triggered();
    void on_actionStop_triggered();
    void on_actionCreate_new_triggered();
    void on_actionDuplicate_triggered();
    void on_actionRecalc_AABB_triggered();
    void on_actionToggleCameraSpeed_triggered();
    void on_menuRecent_scenes_aboutToShow();
    void on_menuEdit_aboutToShow();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionShader_Designer_triggered();
    void on_actionScale_triggered();
    void on_actionTranslate_triggered();
    void on_actionRotate_triggered();
    void on_actionNone_triggered();
    void on_actionUniform_Scale_triggered();
    void on_actionMenuGlobal_triggered();
    void on_actionMenuLocal_triggered();
    void on_menuGAME_OBJECT_aboutToShow();
    void on_actionAutoRecalc_AABB_on_resize_triggered(bool);
    void on_actionMemory_Leaks_detection_triggered();
    void on_actionDelete_triggered();
    void on_actionQuery_bullet_profiler_triggered();
    void on_actionOpen_triggered();
    void on_actionScripts_triggered();

    void afterViewportInitializeGLCall(QGLGameViewport* game_viewport);
    void onBulletProfilerTimeout();
    void onInspectorActionPeformed(QObject* obj, QString property_name, QVariant value, QVariant old_value);
    void onChangeBulletProfilerInterval(int value);
    void onNewGameObjectSelected(uint selected_gameobj_uid);
    void onModeButtonTriggered();
    void onSelectedGameObjectTransformChanged(QGLGameEditorViewport::ManipulatorTypes selected_manipulator);
    void onRecentSceneSelected();
};

#endif // RIOENGINE_EDITOR_EDITORWINDOW_H_
