#ifndef PROFILEWINDOWFORCANDIDATE_H
#define PROFILEWINDOWFORCANDIDATE_H

#include <QMainWindow>


namespace Ui {
class ProfileWindowForCandidate;
}

class ProfileWindowForCandidate : public QMainWindow {
    Q_OBJECT

public:
    explicit ProfileWindowForCandidate(QWidget *parent = nullptr);
    ~ProfileWindowForCandidate();

private:
    Ui::ProfileWindowForCandidate *ui;
};

#endif  // PROFILEWINDOWFORCANDIDATE_H
