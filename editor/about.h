#ifndef RIOENGINE_EDITOR_ABOUT_H_
#define RIOENGINE_EDITOR_ABOUT_H_

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;
};

#endif // RIOENGINE_EDITOR_ABOUT_H_
