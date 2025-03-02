#ifndef EMPLOYEEPROFILEWINDOW_H
#define EMPLOYEEPROFILEWINDOW_H

#include <QMainWindow>

namespace Ui {
class employeeprofilewindow;
}

class employeeprofilewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit employeeprofilewindow(QWidget *parent = nullptr);
    ~employeeprofilewindow();

private:
    Ui::employeeprofilewindow *ui;
};

#endif // EMPLOYEEPROFILEWINDOW_H
