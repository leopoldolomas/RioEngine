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

#include <QSplitter>
#include "cengine/cobject.h"
#include "editor/qpropertybrowser/qobjectinspector.h"
#include "editor/qpropertyeditor/qpropertyeditorwidget.h"
#include "editor/qpropertybrowser/qobjectmodel.h"
#include "ui_qpropertybrowser.h"
#include "qpropertybrowser.h"

//-----------------------------------------------------------------------------

QPropertyBrowser::QPropertyBrowser(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QPropertyBrowser) {
    m_ui->setupUi(this);
    m_listView = new QObjectInspector(this);
    m_propertyEditor = new QPropertyEditorWidget(this);
    m_model = new QObjectModel(this);
    m_propertyEditor->setAlternatingRowColors(true);

    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(m_listView);
    splitter->addWidget(m_propertyEditor);
    splitter->setStretchFactor(1, 1);

    m_listView->setModel(m_model);
    connect(m_listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onSelectedObjectInInspector(QModelIndex)));

    m_ui->verticalLayout->addWidget(splitter);
}

//-----------------------------------------------------------------------------

QPropertyEditorWidget* QPropertyBrowser::getQPropertyEditorWidget() const {
    return m_propertyEditor;
}

//-----------------------------------------------------------------------------

bool QPropertyBrowser::containsChild(QObject* child) {
    RE_ASSERT(child);
    int index = m_model->children()->indexOf(child);
    return (index != -1);
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::addChild(QObject* child, bool select_immediately) {
    if (containsChild(child)) {
        if (select_immediately) {
            setSelectedChild(child);
        }
        return;
    }

    m_model->addChild(child);

    if (select_immediately) {
        setSelectedChild(child);
    }
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::setSelectedChild(QObject *child) {
    if (!containsChild(child)) {
        return;
    }

    selectChildInInspector(child->objectName().toStdString());
    m_propertyEditor->setObject(child);
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::newObjectSelected(const QModelIndex &index) {
    setSelectedChild(m_model->children()->at(index.row()));
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::clearSelection() {
    m_propertyEditor->setObject(NULL);
    m_listView->unselectAll();
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::selectChildInInspector(std::string id) {
    m_listView->unselectAll();
    QObjectModel* model = (QObjectModel*)m_listView->model();
    int index = -1;
    for (int i = 0; i < model->children()->count(); i++) {
        if (model->children()->at(i)->objectName().toStdString() == id) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        QModelIndex Index = model->index(index, 0);
        if ( Index.isValid() ) {
            m_listView->selectionModel()->select( Index, QItemSelectionModel::Select );
        }
    }
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::removeChildInInspector(std::string id) {
    m_listView->unselectAll();
    QObjectModel* model = (QObjectModel*)m_listView->model();
    for (int i = 0; i < model->children()->count(); i++) {
        if (model->children()->at(i)->objectName().toStdString() == id) {
            model->removeChild(QString::fromStdString(id));
            return;
        }
    }
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::clearModel() {
    m_model->clear();
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::update() {
    m_propertyEditor->setFocus();  // bug fix
    emit m_model->dataChanged(QModelIndex(), QModelIndex());
    m_propertyEditor->updateObject();
}

//-----------------------------------------------------------------------------

void QPropertyBrowser::onSelectedObjectInInspector(QModelIndex index) {
    QVariant value = m_model->data(index, 0);
    emit selectedObjectOnInspector(value.toString());
}

//-----------------------------------------------------------------------------

QPropertyBrowser::~QPropertyBrowser() {
    SAFE_RELEASE(m_model);
    delete m_ui;
}
