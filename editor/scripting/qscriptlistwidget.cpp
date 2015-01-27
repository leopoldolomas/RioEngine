#include "qscriptlistwidget.h"
#include "editor/scripting/qscriptmodel.h"
#include "ui_qscriptlistwidget.h"
#include "editor/scripting/qcodeeditor.h"
#include "misc/directoryhelper.hpp"

QScriptListWidget::QScriptListWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QScriptListWidget) {
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

void QScriptListWidget::on_btnRefresh_clicked() {
    ui->scriptList->clear();

    std::string scripts_path = DirectoryHelper::getScriptsPath();
    auto file_list = DirectoryHelper::getFileList(scripts_path, "rioscript");
    for (auto it = file_list.begin(); it != file_list.end(); ++it) {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(QString::fromStdString(*it));
        ui->scriptList->insertItem(0, newItem);
    }
}

void QScriptListWidget::on_btnCompile_clicked() {
}

void QScriptListWidget::on_scriptList_doubleClicked(const QModelIndex &index) {
    auto item = ui->scriptList->item(index.row());
    QString script_filename = item->text();
    auto editor_widget = new QCodeEditor();
    editor_widget->resize(640, 480);
    editor_widget->loadFile(script_filename);
    editor_widget->show();
}

QScriptListWidget::~QScriptListWidget() {
    delete ui;
}
