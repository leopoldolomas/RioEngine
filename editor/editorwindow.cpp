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

#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QToolButton>
#include "constants.h"
#include "misc/transform.h"
#include "misc/matrixstack.h"
#include "inputmanager/inputmanager.h"
#include "clengine/classetsrepository.h"
#include "cengine/cautoreleasepool.h"
#include "cengine/csoundplayer.h"
#include "cengine/ccamera.h"
#include "cengine/cdirector.h"
#include "editor/undoengine/undoengine.h"
#include "editor/qgamewindow/qgamewindow.h"
#include "editor/qglgameeditorviewport/qglgameeditorviewport.h"
#include "editor/qpropertyeditor/customtypes.h"
#include "editor/qpropertyeditor/qpropertyeditorwidget.h"
#include "editor/serialization/qtcustomserialization.hpp"
#include "editor/serialization/qserializer.h"
#include "editor/about.h"
#include "shootingrange/ccustomscene.h"
#include "ui_editorwindow.h"
#include "editor/editorwindow.h"

#if VLD_ENABLED
#include <vld.h>
#endif

//-----------------------------------------------------------------------------

EditorWindow* EditorWindow::currentInstance = NULL;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow) {
    currentInstance = this;
    CustomTypes::registerTypes();
    QCustomSerialization::registerCustomStreamOperators();
    ui->setupUi(this);

    CSOUNDPLAYER;

    m_previousSceneGameObjects = QList<CGameObject>();
    m_isClosing = false;
    m_currentSceneFilename = "";
    m_isEditingProperty = false;

    m_glViewport = new QGLGameEditorViewport(CLASSETSREPOSITORY, NULL);
    QBoxLayout* tmp = new QBoxLayout(QBoxLayout::LeftToRight, 0);
    tmp->addWidget(m_glViewport);

    loadResources();

    CCustomScene* m_gameEditorScene = new CCustomScene();
    connect(m_glViewport, SIGNAL(newGameObjectSelected(uint)),
            this, SLOT(onNewGameObjectSelected(uint)));

    ui->wViewportContainer->setLayout(tmp);

    setCentralWidget(ui->wViewportContainer);

    INPUTMANAGER->initializeJoystick((HWND)winId());

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->start(17); // 60 fps

    m_propertyBrowser = new QPropertyBrowser(this);
    m_propertyBrowser->getQPropertyEditorWidget()->registerCustomPropertyCB(CustomTypes::createCustomProperty);
    connect(m_propertyBrowser, SIGNAL(selectedObjectOnInspector(QString)),
            this, SLOT(selectGameObject(QString)));

    QVBoxLayout* vertical_layout = ui->dwProperties->findChild<QVBoxLayout*>("mainLayout");
    vertical_layout->insertWidget(2, m_propertyBrowser);

    ui->dwProperties->setContentsMargins(0,0,0,0);
    ui->wViewportContainer->setFocus();

    setUpBulletToolbar();
    setFocusPolicy(Qt::ClickFocus);
    setState(EDITING);
    clearSelection();

    setSelectedManipulatorType(QGLGameEditorViewport::TRANSLATION);
    setSelectedTransformMode(Transform::GLOBAL);

    connect(m_glViewport, SIGNAL(selectedGameObjectTransformChanged(QGLGameEditorViewport::ManipulatorTypes)),
            this, SLOT(onSelectedGameObjectTransformChanged(QGLGameEditorViewport::ManipulatorTypes)));

    connect(m_glViewport, SIGNAL(afterInitializeGL(QGLGameViewport*)),
            this, SLOT(afterViewportInitializeGLCall(QGLGameViewport*)));

    CDIRECTOR->runWithScene(m_gameEditorScene);
    CDIRECTOR->pause();

    addDefaultPlane();

    SAFE_RELEASE(m_gameEditorScene);
}

//-----------------------------------------------------------------------------

void EditorWindow::setState(State state) {
    m_state = state;
    updateGUIState();
}

//-----------------------------------------------------------------------------

EditorWindow::State EditorWindow::currentState() const {
    return m_state;
}

//-----------------------------------------------------------------------------

bool EditorWindow::areResourcesLoaded() {
    return CLASSETSREPOSITORY->nodes()->size() > 0;
}

//-----------------------------------------------------------------------------

void EditorWindow::loadResources() {
    CLASSETSREPOSITORY->loadResources();
}

//-----------------------------------------------------------------------------

void EditorWindow::loadResourcesIfNeeded() {
    if (!areResourcesLoaded()) {
        loadResources();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::selectGameObjectHavingUID(uint gameobj_uid) {
    std::vector<CGameObject*>* gameobjects = CDIRECTOR->currentGameScene()->getGameObjects();
    std::vector<CGameObject*>::iterator it;
    for (it = gameobjects->begin();
         it != gameobjects->end();
         ++it) {
        CGameObject* gameobj = *it;
        if (gameobj->uid() == gameobj_uid) {
            selectGameObject(gameobj);
            return;
        }
    }
    selectGameObject(NULL);
}

//-----------------------------------------------------------------------------

void EditorWindow::selectGameObject(QString name) {
    std::vector<CGameObject*>* gameobjects = CDIRECTOR->currentGameScene()->getGameObjects();
    for (unsigned int i = 0; i < gameobjects->size(); i++) {
        CGameObject* gameobj = gameobjects->at(i);
        if (gameobj->objectName() == name) {
            selectGameObject(gameobj);
            return;
        }
    }
    selectGameObject(NULL);
}

//-----------------------------------------------------------------------------

void EditorWindow::setSelectedTransformMode(Transform::TransformMode transform_mode) {
    m_glViewport->setSelectedTransformMode(transform_mode);

    ui->actionMenuGlobal->setChecked(transform_mode == Transform::GLOBAL);
    ui->actionMenuLocal->setChecked(transform_mode == Transform::LOCAL);
}

//-----------------------------------------------------------------------------

void EditorWindow::setSelectedManipulatorType(QGLGameEditorViewport::ManipulatorTypes manipulator_type) {
    m_glViewport->setSelectedManipulatorType(manipulator_type);
    ui->actionTranslate->setChecked(manipulator_type == QGLGameEditorViewport::TRANSLATION);
    ui->actionNone->setChecked(manipulator_type == QGLGameEditorViewport::NOMANIPULATOR);
    ui->actionRotate->setChecked(manipulator_type == QGLGameEditorViewport::ROTATION);
    ui->actionScale->setChecked(manipulator_type == QGLGameEditorViewport::SCALE);
    ui->actionUniform_Scale->setChecked(manipulator_type == QGLGameEditorViewport::UNIFORMSCALE);
}

//-----------------------------------------------------------------------------

void EditorWindow::updateGUIState() {
    bool gameobj_selected = m_glViewport->selectedGameObjectUID() != 0;
    ui->actionDelete->setEnabled(gameobj_selected);
    ui->actionDuplicate->setEnabled(gameobj_selected);
    ui->actionRecalc_AABB->setEnabled(gameobj_selected);
    ui->actionAutoRecalc_AABB_on_resize->setChecked(m_glViewport->autoRecalcAABBOnResize());

    ui->actionPlay->setEnabled(m_state == EDITING);
    ui->actionStop->setEnabled(m_state == PLAYING);

    ui->actionUndo->setEnabled(UNDOABLEACTIONSMANAGER->hasUndoActions());
    ui->actionRedo->setEnabled(UNDOABLEACTIONSMANAGER->hasRedoActions());

    std::string undo_action_description = "Undo ";
    std::string tmp = UNDOABLEACTIONSMANAGER->getHintForCurrentUndoAction();
    undo_action_description += tmp;

    ui->actionUndo->setText(undo_action_description.c_str());

    undo_action_description = "Redo ";
    tmp = UNDOABLEACTIONSMANAGER->getHintForCurrentRedoAction();
    undo_action_description += tmp;

    ui->actionRedo->setText(undo_action_description.c_str());

    int camera_speed = m_glViewport->cameraSpeed();
    QString res = ":/images/metro/cameraspeed" + QString::number(camera_speed) + ".png";
    ui->actionToggleCameraSpeed->setIcon(QIcon(res));

    ui->actionMemory_Leaks_detection->setChecked(m_memoryLeaksDetection);
}

//-----------------------------------------------------------------------------

void EditorWindow::clearSelection() {
    selectGameObject(NULL);
}

//-----------------------------------------------------------------------------

void EditorWindow::repopulatePropertyBrowser() {
    m_propertyBrowser->clearModel();
    std::vector<CGameObject*>* game_objs = CDIRECTOR->currentGameScene()->getGameObjects();
    for (uint i = 0; i < game_objs->size(); i++) {
        m_propertyBrowser->addChild(game_objs->at(i));
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::selectMostRecentGameObject() {
    CGameScene* game_scene = CDIRECTOR->currentGameScene();
    unsigned int vec_size = game_scene->getGameObjects()->size();
    if (vec_size > 0) {
        CGameObject* last_added = game_scene->getGameObjects()->at(vec_size - 1);
        selectGameObject(last_added);
    } else {
        selectGameObject(NULL);
        clearSelection();
        m_propertyBrowser->clearModel();
        m_propertyBrowser->clearSelection();
    }
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::removeChildFromInspector(QString name) {
    m_propertyBrowser->removeChildInInspector(name.toStdString());
}

//-----------------------------------------------------------------------------

void EditorWindow::timeout(void) {
    if (!m_isInitialized) {
        if (areResourcesLoaded())
            m_isInitialized = true;
    }

    if (m_glViewport) {
        m_glViewport->updateGL();
    }

    CAUTORELEASEPOOL->drain();
}

//-----------------------------------------------------------------------------

void EditorWindow::selectGameObject(CGameObject* game_object) {
    if (game_object) {
        m_propertyBrowser->addChild(game_object, true);
        m_glViewport->setSelectedGameObjectUID(game_object->uid());
        updateGUIState();
    } else {
        m_glViewport->setSelectedGameObjectUID(0);
        m_propertyBrowser->clearSelection();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    // TODO ask for user confirmation
    //event->ignore(); cancels the event
    saveLayout();

    m_isClosing = true;
    stopGame();

    SAFE_RELEASE(m_glViewport);
    m_glViewport = NULL;

    m_timer->stop();
    SAFE_RELEASE(m_timer);
    m_tmrBulletProfiler->stop();
    SAFE_RELEASE(m_tmrBulletProfiler);
    // ---------------------------------
    CSingleton<CAutoreleasePool>::free();
    CAutoreleasePool::releasePools();
    // ---------------------------------
    CSingleton<CDirector>::free();
    CSingleton<USERINPUT::InputManager>::free();
    CSingleton<UndoableActionsManager>::free();
    CSingleton<Math::MatrixStack>::free();
    CSingleton<CSoundPlayer>::free();
    CSingleton<CLAssetsRepository>::free();

    SAFE_RELEASE(m_propertyBrowser);

    QApplication::quit();
}

//-----------------------------------------------------------------------------

void EditorWindow::saveLayout() {
    QSettings settings("rioengine.ini", QSettings::IniFormat);
    settings.setValue("MemoryLeaksDetection", QVariant::fromValue(m_memoryLeaksDetection));
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("WindowState", saveState());
    settings.setValue("bulletDebugState", QByteArray::number(
                          CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer()->getDebugMode()));

    QStringList sceneList = m_recentScenes.toList();
    settings.beginWriteArray("RecentScenes", sceneList.size());
    for (int j = 0; j < sceneList.size(); ++j) {
        settings.setArrayIndex(j);
        settings.setValue("scene", sceneList.at(j));
    }
    settings.endArray();
}

//-----------------------------------------------------------------------------

void EditorWindow::restoreLayout() {
    QSettings settings("rioengine.ini", QSettings::IniFormat);
    m_memoryLeaksDetection = settings.value("MemoryLeaksDetection").toBool();
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("WindowState").toByteArray());

    CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer()->setDebugMode(
                settings.value("bulletDebugState").toByteArray().toInt());

    int sceneCount = settings.beginReadArray("RecentScenes");
    for (int i = 0; i < sceneCount; ++i) {
        settings.setArrayIndex(i);
        m_recentScenes.push(settings.value("scene").toString());
    }
    settings.endArray();
    if(m_recentScenes.size() > 10) m_recentScenes.resize(10);

    updateMemoryLeaksDetectionStatus();
}

//-----------------------------------------------------------------------------

CGameObject* EditorWindow::addNodeHavingId(std::string node_id) {
    CGameObject* new_game_object = CDIRECTOR->currentGameScene()->createGameObjectWithNode(node_id);
    new_game_object->calcSize();
    if (new_game_object) {
        selectGameObject(new_game_object);
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("The node does not exist in the Assets Repository.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    AddOrRemoveGameObjectAction* action = new AddOrRemoveGameObjectAction(this, CDIRECTOR->currentGameScene(), new_game_object, true);
    action->setHint("Add GameObject");
    UNDOABLEACTIONSMANAGER->pushUndoAction(action->getOppositeAction());
    SAFE_RELEASE(action);
    updateGUIState();

    return new_game_object;
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionNewScene_triggered() {
    if (m_currentSceneFilename != "" || CDIRECTOR->currentGameScene()->getGameObjects()->size() > 0) {
        // ask for user confirmation
        int ret = QMessageBox::warning(this, tr("Create new scene"),
                                       tr("All unsaved changes will be lost. Are you sure you want to continue?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (ret == QMessageBox::Yes) {
            m_currentSceneFilename = "";
            clearSelection();
            m_propertyBrowser->clearModel();
            m_propertyBrowser->clearSelection();
            CDIRECTOR->currentGameScene()->clear();
            CDIRECTOR->getActiveCamera()->reset();
            UNDOABLEACTIONSMANAGER->clear();
            updateGUIState();
            addDefaultPlane();
        }
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionSave_Scene_2_triggered() {
    if (m_currentSceneFilename != "") {
        QSerializer::serialize(CDIRECTOR->currentGameScene(), QString::fromStdString(m_currentSceneFilename));  // save the updated content
    } else {
        on_actionSave_As_triggered();  // launch the Save As dialog
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionExit_triggered() {
    close();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionSave_As_triggered() {
    std::string filter = "RioEngine Game Scenes (*.";
    filter.append(k_CGameScene_DefaultExt);
    filter.append(")");

    QString file_name = QFileDialog::getSaveFileName(this, tr("Save Game Scene"), "/",
                                                     tr(filter.c_str()));

    if (file_name != "") {
        m_currentSceneFilename = file_name.toStdString();
        addSceneFilenameToRecentSceneList(QString::fromStdString(m_currentSceneFilename));
        QSerializer::serialize(CDIRECTOR->currentGameScene(), QString::fromStdString(m_currentSceneFilename));
        saveLayout();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionAbout_triggered() {
    About about = About(this);
    about.exec();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionReset_Camera_triggered() {
    CDIRECTOR->getCameraWithId(k_QGLGameEditorViewport_CameraId)->reset();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionPlay_triggered() {
    if (currentState() == PLAYING) {
        return;
    }

    // store the current game scene
    m_previousSceneGameObjects = CDIRECTOR->currentGameScene()->gameObjects();

    clearSelection();
    CDIRECTOR->currentGameScene()->run();
    repopulatePropertyBrowser();
    UNDOABLEACTIONSMANAGER->clear();

    m_currentGameWindow = new QGameWindow(this, CLASSETSREPOSITORY, CDIRECTOR->currentGameScene(), m_glViewport);
    m_currentGameWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    
    m_currentGameWindow->show();

    connect(m_currentGameWindow, SIGNAL(destroyed(QObject*)), this, SLOT(onGameWindowDestroyedCallback(QObject*)));

    setState(PLAYING);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionStop_triggered() {
    stopGame();
}

//-----------------------------------------------------------------------------

void EditorWindow::onGameWindowDestroyedCallback(QObject*) {
    m_currentGameWindow = NULL;
    stopGame();
}

//-----------------------------------------------------------------------------

void EditorWindow::addSceneFilenameToRecentSceneList(QString current_scene_filename) {
    int index = -1;
    for(int i = 0; i < m_recentScenes.size(); i++) {
        if(m_recentScenes[i] == current_scene_filename) {
            index = i;
            break;
        }
    }
    if(index != -1) {
        m_recentScenes.remove(index);
    }
    m_recentScenes.push(current_scene_filename);
    if(m_recentScenes.size() > 10) {
        m_recentScenes.resize(10);
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_menuRecent_scenes_aboutToShow() {
    QMenu* menu_recent_files = ui->menuFile->findChild<QMenu*>("menuRecent_scenes");
    menu_recent_files->clear();

    for(int i = m_recentScenes.size() - 1; i >= 0; i--) {
        QAction* action = new QAction(m_recentScenes[i], menu_recent_files);
        menu_recent_files->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), SLOT(onRecentSceneSelected()));
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_menuEdit_aboutToShow() {
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionUndo_triggered() {
    UNDOABLEACTIONSMANAGER->performUndo();
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionRedo_triggered() {
    UNDOABLEACTIONSMANAGER->performRedo();
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionShader_Designer_triggered() {
    // TODO remove
}

//-----------------------------------------------------------------------------

void EditorWindow::onNewGameObjectSelected(uint selected_gameobj_uid) {
    selectGameObjectHavingUID(selected_gameobj_uid);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionScale_triggered() {
    setSelectedManipulatorType(QGLGameEditorViewport::SCALE);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionTranslate_triggered() {
    setSelectedManipulatorType(QGLGameEditorViewport::TRANSLATION);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionRotate_triggered() {
    setSelectedManipulatorType(QGLGameEditorViewport::ROTATION);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionNone_triggered() {
    setSelectedManipulatorType(QGLGameEditorViewport::NOMANIPULATOR);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionUniform_Scale_triggered() {
    setSelectedManipulatorType(QGLGameEditorViewport::UNIFORMSCALE);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionMenuGlobal_triggered() {
    setSelectedTransformMode(Transform::GLOBAL);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionMenuLocal_triggered() {
    setSelectedTransformMode(Transform::LOCAL);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionDelete_triggered() {
    CGameObject* game_obj = (CGameObject*)getSelectedGameObject();
    RE_ASSERT(game_obj);
    std::string obj_name = game_obj->objectName().toStdString();
    if (CDIRECTOR->currentGameScene()->removeGameObject(game_obj->uid())) {
        m_propertyBrowser->removeChildInInspector(obj_name);
        AddOrRemoveGameObjectAction* action = new AddOrRemoveGameObjectAction(this, CDIRECTOR->currentGameScene(),
                                                                              game_obj, false);
        action->setHint("Remove GameObject");
        UNDOABLEACTIONSMANAGER->pushUndoAction(action->getOppositeAction());
        SAFE_RELEASE(action);
        selectMostRecentGameObject();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionQuery_bullet_profiler_triggered() {
    CProfileManager::dumpAll();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionOpen_triggered() {
    std::string filter = "RioEngine Game Scenes (*.";
    filter.append(k_CGameScene_DefaultExt);
    filter.append(")");

    QString file_name = QFileDialog::getOpenFileName(this, tr("Load Game Scene"), "",
                                                     tr(filter.c_str()));

    if (file_name != "") {
        loadScene(file_name);
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_menuGAME_OBJECT_aboutToShow() {
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::propertyChanged(QObject * obj, QString property_name, QVariant value, QVariant old_value) {
    CGameObject* game_obj = static_cast<CGameObject*>(obj);
    if (!m_isEditingProperty) {
        std::vector<CGameObject*>* game_objs = CDIRECTOR->currentGameScene()->getGameObjects();
        for (uint i = 0; i < game_objs->size(); i++) {
            if (game_objs->at(i) == obj) {
                m_lastPropertyChange.obj = obj;
                m_lastPropertyChange.name = property_name;
                m_lastPropertyChange.value = value;
                m_lastPropertyChange.oldValue = old_value;
            }
        }
        m_isEditingProperty = true;
    }
    if (property_name == "EnablePhysics") {
        deployOrRemoveGameObjInDynamicsWorld(game_obj, value.toBool());
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionCreate_new_triggered() {
    showCreateGameObjectMenu();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionDuplicate_triggered() {
    const CGameObject* gameobj = getSelectedGameObject();
    RE_ASSERT(gameobj);
    CGameObject* clone = CDIRECTOR->currentGameScene()->duplicateGameObject(gameobj);
    clone->transform().setLocalPosition(clone->transform().localPosition() + btVector3(0.1f, 0, 0));
    repopulatePropertyBrowser();
    selectGameObject(clone);

    DuplicateGameObject* action = new DuplicateGameObject(this, CDIRECTOR->currentGameScene(), clone, true);
    UNDOABLEACTIONSMANAGER->pushUndoAction(action->getOppositeAction());
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionRecalc_AABB_triggered() {
    CGameObject* selected_gameobj = (CGameObject*)getSelectedGameObject();
    RE_ASSERT(selected_gameobj);
    if (selected_gameobj == NULL || !selected_gameobj->bPhysicsEnabled()) {
        return;
    }
    selected_gameobj->removeFromDynamicsWorld();
    selected_gameobj->calcSize();
    if(selected_gameobj->bulletProperties.physicsEnabled()) {
        selected_gameobj->deployIntoDynamicsWorld();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionAutoRecalc_AABB_on_resize_triggered(bool state) {
    m_glViewport->setAutoRecalcAABBOnResize(state);
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionToggleCameraSpeed_triggered() {
    m_glViewport->nextCameraSpeed();
    updateGUIState();
}

//-----------------------------------------------------------------------------

void EditorWindow::on_actionMemory_Leaks_detection_triggered() {
    m_memoryLeaksDetection = !m_memoryLeaksDetection;
    updateMemoryLeaksDetectionStatus();
}

//-----------------------------------------------------------------------------

void EditorWindow::onInspectorActionPeformed(QObject* obj, QString property_name, QVariant value, QVariant old_value) {
    if (property_name == "EnablePhysics") {
        deployOrRemoveGameObjInDynamicsWorld(reinterpret_cast<CGameObject*>(obj), value.toBool());
    }
    regenerateBodyIfNeeded(static_cast<CGameObject*>(obj), property_name);
    m_propertyBrowser->update();
}

//-----------------------------------------------------------------------------

void EditorWindow::onEditPropertyBegin() {
}

//-----------------------------------------------------------------------------

void EditorWindow::onEditPropertyEnd() {
    if (m_lastPropertyChange.obj != NULL &&
            m_lastPropertyChange.oldValue != m_lastPropertyChange.obj->property(qPrintable(m_lastPropertyChange.name))) {
        InspectorAction* action = new InspectorAction(reinterpret_cast<CGameObject*>(m_lastPropertyChange.obj), m_lastPropertyChange.name, m_lastPropertyChange.oldValue);
        UNDOABLEACTIONSMANAGER->pushUndoAction(reinterpret_cast<IUndoableAction*>(action));
        m_propertyBrowser->update();
    }
    regenerateBodyIfNeeded(static_cast<CGameObject*>(m_lastPropertyChange.obj), m_lastPropertyChange.name);
    m_isEditingProperty = false;
}

//-----------------------------------------------------------------------------

void EditorWindow::onBulletProfilerTimeout() {
    if (ui->actionBulletProfiler->isChecked()) {
        CProfileManager::dumpAll();
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::onChangeBulletProfilerInterval(int value) {
    m_tmrBulletProfiler->start(value * 1000);
}

//-----------------------------------------------------------------------------

void EditorWindow::setUpBulletToolbar() {
    QSpinBox* profiler_interval = new QSpinBox();
    QToolButton* mode_button = new QToolButton();
    mode_button->setObjectName("btnDebugMode");
    mode_button->setText("Debug Mode");
    profiler_interval->setMinimum(1);
    profiler_interval->setValue(5);
    profiler_interval->setMaximumWidth(50);
    profiler_interval->setMaximumHeight(ui->tbBullet->height());
    connect(profiler_interval, SIGNAL(valueChanged(int)),
            this, SLOT(onChangeBulletProfilerInterval(int)));
    ui->tbBullet->addWidget(profiler_interval);
    ui->actionBulletProfiler->setEnabled(true);
    ui->tbBullet->addAction(ui->actionBulletProfiler);
    ui->tbBullet->addWidget(mode_button);
    connect(mode_button, SIGNAL(clicked()),
            this, SLOT(onModeButtonTriggered()));

    m_tmrBulletProfiler = new QTimer();
    connect(m_tmrBulletProfiler, SIGNAL(timeout()),
            this, SLOT(onBulletProfilerTimeout()));
    m_tmrBulletProfiler->start(profiler_interval->value() * 1000);
}

//-----------------------------------------------------------------------------

void EditorWindow::deployOrRemoveGameObjInDynamicsWorld(CGameObject* game_obj, bool deploy) {
    ( deploy ? game_obj->deployIntoDynamicsWorld() : game_obj->removeFromDynamicsWorld() );
}

//-----------------------------------------------------------------------------

void EditorWindow::createActionForDebugModeMenu(QMenu* menu, QString text, int bitmask, int option) {
    QAction* action = new QAction(text, menu);
    action->setCheckable(true);
    action->setChecked(bitmask & option);
    menu->addAction(action);
}

//-----------------------------------------------------------------------------

void EditorWindow::onModeButtonTriggered() {
    QToolButton* btn_debug_mode = findChild<QToolButton*>("btnDebugMode");
    RE_ASSERT(btn_debug_mode);
    QPoint global_pos = btn_debug_mode->mapToGlobal(QPoint(10, 10));
    QMenu mode_menu;

    int dbg_mode = CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer()->getDebugMode();

    createActionForDebugModeMenu(&mode_menu, "DrawWireframe", dbg_mode,         (int)btIDebugDraw::DBG_DrawWireframe);
    createActionForDebugModeMenu(&mode_menu, "DrawAabb", dbg_mode,              (int)btIDebugDraw::DBG_DrawAabb);
    createActionForDebugModeMenu(&mode_menu, "DrawFeaturesText", dbg_mode,      (int)btIDebugDraw::DBG_DrawFeaturesText);
    createActionForDebugModeMenu(&mode_menu, "DrawContactPoints", dbg_mode,     (int)btIDebugDraw::DBG_DrawContactPoints);
    createActionForDebugModeMenu(&mode_menu, "NoDeactivation", dbg_mode,        (int)btIDebugDraw::DBG_NoDeactivation);
    createActionForDebugModeMenu(&mode_menu, "NoHelpText", dbg_mode,            (int)btIDebugDraw::DBG_NoHelpText);
    createActionForDebugModeMenu(&mode_menu, "DrawText", dbg_mode,              (int)btIDebugDraw::DBG_DrawText);
    createActionForDebugModeMenu(&mode_menu, "ProfileTimings", dbg_mode,        (int)btIDebugDraw::DBG_ProfileTimings);
    createActionForDebugModeMenu(&mode_menu, "EnableSatComparison", dbg_mode,   (int)btIDebugDraw::DBG_EnableSatComparison);
    createActionForDebugModeMenu(&mode_menu, "DisableBulletLCP", dbg_mode,      (int)btIDebugDraw::DBG_DisableBulletLCP);
    createActionForDebugModeMenu(&mode_menu, "EnableCCD", dbg_mode,             (int)btIDebugDraw::DBG_EnableCCD);
    createActionForDebugModeMenu(&mode_menu, "DrawContraints", dbg_mode,        (int)btIDebugDraw::DBG_DrawConstraints);
    createActionForDebugModeMenu(&mode_menu, "DrawContraintsLimits", dbg_mode,  (int)btIDebugDraw::DBG_DrawConstraintLimits);
    createActionForDebugModeMenu(&mode_menu, "FastWireframe", dbg_mode,         (int)btIDebugDraw::DBG_FastWireframe);
    createActionForDebugModeMenu(&mode_menu, "DrawNormals", dbg_mode,           (int)btIDebugDraw::DBG_DrawNormals);

    mode_menu.addSeparator();
    mode_menu.addAction("Default");
    mode_menu.addAction("Select All");
    mode_menu.addAction("No Debug");
    // ...

    QAction* selectedItem = mode_menu.exec(global_pos);
    // TODO is there an easier way to get the index of the action in the QMenu?
    int index = -1;
    if (selectedItem) {
        for (int i = 0; i < mode_menu.actions().size(); i++) {
            if (selectedItem == mode_menu.actions()[i]) {
                index = i;
                break;
            }
        }
    }
    if (index < 0) {
        return;
    }

    switch (index) {
    case 0: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawWireframe; break;
    case 1: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawAabb; break;
    case 2: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawFeaturesText; break;
    case 3: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawContactPoints; break;
    case 4: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_NoDeactivation; break;
    case 5: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_NoHelpText; break;
    case 6: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawText; break;
    case 7: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_ProfileTimings; break;
    case 8: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_EnableSatComparison; break;
    case 9: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DisableBulletLCP; break;
    case 10: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_EnableCCD; break;
    case 11: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawConstraints; break;
    case 12: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawConstraintLimits; break;
    case 13: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_FastWireframe; break;
    case 14: dbg_mode = dbg_mode ^ btIDebugDraw::DBG_DrawNormals; break;
    case 16: dbg_mode = k_bdebugdraw_DefaultDebugMode; break;
    case 17: dbg_mode = INT_MAX; break;
    case 18: dbg_mode = 0; break;
    default:
        dbg_mode = dbg_mode ^ btIDebugDraw::DBG_NoDebug;
    }
    CDIRECTOR->currentGameScene()->dynamicsWorld()->getDebugDrawer()->setDebugMode(dbg_mode);
}

//-----------------------------------------------------------------------------

void EditorWindow::regenerateBodyIfNeeded(CGameObject* game_object, QString property_name) {
    if (
            property_name == "Mass"                  ||
            property_name == "Restitution"           ||
            property_name == "Friction"              ||
            property_name == "LinearFactor"          ||
            property_name == "AngularFactor"         ||
            property_name == "ShapeType"             ||
            property_name == "ShapeType_Radius"     ||
            property_name == "ShapeType_Height"      ||
            property_name == "ShapeType_Size"        ||
            property_name == "ShapeType_Vertices"    ||
            property_name == "ShapeType_PlaneNormal" ||
            property_name == "ShapeType_PlaneConstant"
            ) {
        if (m_lastPropertyChange.obj->property("EnablePhysics").toBool()) {
            CGameObject* game_obj = static_cast<CGameObject*>(m_lastPropertyChange.obj);
            game_obj->removeFromDynamicsWorld();
            game_obj->deployIntoDynamicsWorld();
        }
    }
}

//-----------------------------------------------------------------------------

void EditorWindow::addDefaultPlane() {
    CGameObject* plane = addNodeHavingId("Grass");
    plane->setbPhysicsEnabled(true);
    plane->setShapeType(CBulletProperties::StaticPlane);
    plane->setbShapeTypePlaneNormal(btVector3(0,0,1));
    plane->setRotation(-90.0f, 0, 0);
    plane->setbRestitution(1.0f);
    plane->setbFriction(1.5f);
    plane->setScale(50.0f, 50.0f, 50.0f);
    plane->deployIntoDynamicsWorld();
    clearSelection();
}

//-----------------------------------------------------------------------------

void EditorWindow::stopGame() {
    if (m_state != PLAYING) {
        return;
    }

    if (m_currentGameWindow != NULL) {
        m_currentGameWindow->close();
    }

    CDIRECTOR->pause();
    m_glViewport->setSelectedGameObjectUID(0);

    CDIRECTOR->currentGameScene()->clear();
    // restore the original scene's game objects
    if (!m_isClosing) {
        CDIRECTOR->currentGameScene()->setGameObjects(m_previousSceneGameObjects);
    }
    m_previousSceneGameObjects.clear();
    repopulatePropertyBrowser();
    CDIRECTOR->resetCameras();
    setState(EDITING);
}

//-----------------------------------------------------------------------------

void EditorWindow::showCreateGameObjectMenu() {
    QMenu menu(this);

    std::vector<std::string> nodes_list = CLASSETSREPOSITORY->getNodeIdList();
    for (unsigned int i = 0; i < nodes_list.size(); i++) {
        std::string node_id = nodes_list.at(i);
        std::string prefix = "__hidden";
        if (node_id.compare(0, 8, prefix) != 0) {
            menu.addAction(QString::fromStdString(nodes_list.at(i)));
        }
    }

    QAction* res = menu.exec(QCursor::pos());

    if (res) {
        std::string selected_node_str = res->text().toStdString();
        addNodeHavingId(selected_node_str);
    }
}

//-----------------------------------------------------------------------------

const CGameObject* EditorWindow::getSelectedGameObject() {
    return static_cast<const CGameObject*>(CDIRECTOR->currentGameScene()->getGameObject(
                                               m_glViewport->selectedGameObjectUID()));
}

//-----------------------------------------------------------------------------

void EditorWindow::onSelectedGameObjectTransformChanged(QGLGameEditorViewport::ManipulatorTypes selected_manipulator) {
    if (selected_manipulator == QGLGameEditorViewport::SCALE) {
        if (m_glViewport->autoRecalcAABBOnResize()) {
            CGameObject* selected_gameobj = (CGameObject*)getSelectedGameObject();
            RE_ASSERT(selected_gameobj);
            selected_gameobj->removeFromDynamicsWorld();
            selected_gameobj->calcSize();
            if(selected_gameobj->bulletProperties.physicsEnabled()) {
                selected_gameobj->deployIntoDynamicsWorld();
            }
        }
    }
    m_propertyBrowser->update();
}

//-----------------------------------------------------------------------------

void EditorWindow::onRecentSceneSelected() {
    QAction* action = static_cast<QAction*>(sender());
    QString action_text = action->text();
    loadScene(action_text);
}

//-----------------------------------------------------------------------------

void EditorWindow::loadScene(QString filename) {
    CDIRECTOR->currentGameScene()->clear();
    QSerializer::deserialize(CDIRECTOR->currentGameScene(), filename);
    repopulatePropertyBrowser();
    m_currentSceneFilename = filename.toStdString();
    addSceneFilenameToRecentSceneList(QString::fromStdString(m_currentSceneFilename));
}

//-----------------------------------------------------------------------------

void EditorWindow::updateMemoryLeaksDetectionStatus() {
#if VLD_ENABLED
    ( m_memoryLeaksDetection ? VLDEnable() : VLDDisable() );
#endif
}

//-----------------------------------------------------------------------------

void EditorWindow::afterViewportInitializeGLCall(QGLGameViewport* game_viewport) {
    restoreLayout();
    updateGUIState();
}

//-----------------------------------------------------------------------------

EditorWindow::~EditorWindow() {
    SAFE_RELEASE(ui);
}
