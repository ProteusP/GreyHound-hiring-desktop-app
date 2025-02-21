#include "profilewindow.h"
#include "ui_profilewindow.h"
//TODO: Дима и Миша
// Нужно глянуть в дб и решить что будет в профиле, как добавлять анкету
// Добавить функцию загрузки резюме в дб
// добавить функцию создания объекта анкеты, который будет использоваться
// для отображения анкет в мейн окне (надо подумать над деталями реализации, мб откажемся от класса)
ProfileWindow::ProfileWindow(QMainWindow *mainWindow_,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProfileWindow),
    mainWindow(mainWindow_)
{
    ui->setupUi(this);
    this->setWindowTitle("Profile window");
    this->resize(800,600);
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::on_homePB_clicked()
{
    this->hide();
    this->mainWindow->show();
}

