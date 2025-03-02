#include "employeeprofilewindow.h"
#include "ui_employeeprofilewindow.h"

employeeprofilewindow::employeeprofilewindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::employeeprofilewindow)
{
    ui->setupUi(this);
}

employeeprofilewindow::~employeeprofilewindow()
{
    delete ui;
}
