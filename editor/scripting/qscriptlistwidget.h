#ifndef RIOENGINE_EDITOR_SCRIPTING_QSCRIPTLISTWIDGET_H_
#define RIOENGINE_EDITOR_SCRIPTING_QSCRIPTLISTWIDGET_H_

#include <QDialog>

namespace Ui {
class QScriptListWidget;
}

class QCodeEditor;

class QScriptListWidget : public QDialog
{
    Q_OBJECT

public:
    explicit QScriptListWidget(QWidget *parent = 0);
    ~QScriptListWidget();

private:
    Ui::QScriptListWidget *ui;
    QList<QCodeEditor*> m_editors;

    void saveAll();
    void compileAll();

private slots:
    void on_btnRefresh_clicked();
    void on_btnCompile_clicked();

    void on_scriptList_doubleClicked(const QModelIndex &index);
    void on_editor_destroyed(QObject*);
};

#endif // RIOENGINE_EDITOR_SCRIPTING_QSCRIPTLISTWIDGET_H_
