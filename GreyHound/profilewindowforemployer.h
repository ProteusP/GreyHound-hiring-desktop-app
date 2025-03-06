#ifndef PROFILEWINDOWFOREMPLOYER_H
#define PROFILEWINDOWFOREMPLOYER_H

#include <QMainWindow>

namespace Ui {
class ProfileWindowForEmployer;
}

class ProfileWindowForEmployer : public QMainWindow {
    Q_OBJECT

public:
    explicit ProfileWindowForEmployer(QWidget *parent = nullptr);
    ~ProfileWindowForEmployer();

private:
    Ui::ProfileWindowForEmployer *ui;
};

#endif  // PROFILEWINDOWFOREMPLOYER_H
