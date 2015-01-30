#include <QtScript/QScriptEngine>
#include "macro.h"
#include "misc/directoryhelper.hpp"
#include "editor/scripting/qcodeeditor.h"
#include "qscriptlistwidget.h"
#include "ui_qscriptlistwidget.h"

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
    saveAll();
    compileAll();
}

void QScriptListWidget::on_scriptList_doubleClicked(const QModelIndex &index) {
    auto item = ui->scriptList->item(index.row());
    QString script_filename = item->text();

    bool found = false;
    for(auto it = m_editors.begin(); it != m_editors.end(); ++it) {
        QCodeEditor* editor = *it;
        if(script_filename.compare(editor->filename()) == 0) {
            found = true;
        }
    }

    if(!found) {
        auto editor_widget = new QCodeEditor();
        editor_widget->setAttribute(Qt::WA_DeleteOnClose);
        editor_widget->resize(640, 480);
        editor_widget->loadFile(script_filename);
        editor_widget->show();
        m_editors.push_back(editor_widget);

        connect(editor_widget, SIGNAL(destroyed(QObject*)), this, SLOT(on_editor_destroyed(QObject*)));
    }
}

void QScriptListWidget::saveAll() {
    for(auto it = m_editors.begin(); it != m_editors.end(); ++it) {
        QCodeEditor* editor = *it;
        editor->saveChanges();
    }
}

void QScriptListWidget::compileAll() {
    ui->txtErrors->clear();
    ui->txtErrors->appendPlainText("Performing syntax check...\n");
    for(int i = 0; i < ui->scriptList->count(); i++) {
        QString filename = ui->scriptList->item(i)->text();
        QString program = QCodeEditor::getScriptContent(filename);

        auto result = QScriptEngine::checkSyntax(program);
        if(result.state() == QScriptSyntaxCheckResult::Error) {
            QString message = QString::number(result.errorLineNumber()) + " : " + result.errorMessage();
            ui->txtErrors->appendPlainText("File: " + filename);
            ui->txtErrors->appendPlainText(message);
            ui->txtErrors->appendPlainText("----------------------------");
        }
    }
    ui->txtErrors->appendPlainText("\nSyntax check complete!");
}

void QScriptListWidget::on_editor_destroyed(QObject *object) {
    auto editor = static_cast<QCodeEditor*>(object);
    m_editors.removeOne(editor);
}

QScriptListWidget::~QScriptListWidget() {
    delete ui;
}
