#ifndef QSCRIPTLISTWIDGET_H
#define QSCRIPTLISTWIDGET_H

#include <QDialog>

namespace Ui {
class QScriptListWidget;
}

class QScriptListWidget : public QDialog
{
    Q_OBJECT

public:
    explicit QScriptListWidget(QWidget *parent = 0);
    ~QScriptListWidget();

private:
    Ui::QScriptListWidget *ui;

private slots:
    void on_btnRefresh_clicked();
    void on_btnCompile_clicked();

    void on_scriptList_doubleClicked(const QModelIndex &index);
};

#endif // QSCRIPTLISTWIDGET_H
