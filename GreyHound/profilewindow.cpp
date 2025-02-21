#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Profile window");
    this->resize(800,600);
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}
