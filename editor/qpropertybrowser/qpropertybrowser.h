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

#ifndef RIOENGINE_EDITOR_QPROPERTYBROWSER_QPROPERTYBROWSER_H_
#define RIOENGINE_EDITOR_QPROPERTYBROWSER_QPROPERTYBROWSER_H_

#include <QWidget>
#include <QModelIndex>

class QListView;
class QPropertyEditorWidget;
class QObjectModel;
class QObjectInspector;

namespace Ui {
class QPropertyBrowser;
}

class QPropertyBrowser : public QWidget {
    Q_OBJECT

public:
    QPropertyBrowser(QWidget *parent = 0);

    QPropertyEditorWidget* getQPropertyEditorWidget() const;

    bool containsChild(QObject* child);
    void addChild(QObject* child, bool select_immediately = true);
    void removeChildInInspector(std::string child);
    void setSelectedChild(QObject* child);
    void clearSelection();
    void clearModel();
    void update();

    ~QPropertyBrowser();

private:
    void selectChildInInspector(std::string id);

    Ui::QPropertyBrowser*  m_ui;
    QObjectModel*          m_model;
    QObjectInspector*      m_listView;
    QPropertyEditorWidget* m_propertyEditor;

public slots:
    void newObjectSelected(const QModelIndex& index);

private slots:
    void onSelectedObjectInInspector(QModelIndex index);

Q_SIGNALS:
    void selectedObjectOnInspector(QString);
};

#endif // RIOENGINE_EDITOR_QPROPERTYBROWSER_QPROPERTYBROWSER_H_
