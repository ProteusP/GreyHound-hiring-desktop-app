#ifndef PROFILEPAGEFOREMPLOYER_H
#define PROFILEPAGEFOREMPLOYER_H

#include <QWidget>

namespace Ui {
class ProfilePageForEmployer;
}

class ProfilePageForEmployer : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePageForEmployer(QWidget *parent = nullptr);
    ~ProfilePageForEmployer();

private:
    Ui::ProfilePageForEmployer *ui;
};

#endif  // PROFILEPAGEFOREMPLOYER_H
