#include "registerwindow.h"
#include "ui_registerwindow.h"
//TODO: Миша и/или глеб
//Нужно задезайнить окно регистрации и
//оформить весь функционал
RegisterWindow::RegisterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    this->resize(800,600);
    this->setWindowTitle("Register window");
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}
