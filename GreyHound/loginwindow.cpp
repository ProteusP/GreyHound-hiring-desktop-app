#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "registerwindow.h"
LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Login window");
    this->resize(800,600);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_exitPB_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выход","Вы точно хотите выйти?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No){
        return;
    }

    QApplication::exit();

}


void LoginWindow::on_loginPB_clicked()
{
    //TODO: Миша и/или Глеб
    //Нужна реализация норм проверки пароля для пользователя!

    QString userMail = ui->mailLine->text();
    QString userPassword = ui->passwordLine->text();

    if (!(userMail == userPassword)){
        QMessageBox::warning(this, "Упс...","Пароль или почта неверен!");
    }
    else{
        MainWindow *mainWindow = new MainWindow();
        this->hide();
        mainWindow->show();
    }
}


void LoginWindow::on_registerPB_clicked()
{
    RegisterWindow *registerWindow = new RegisterWindow();
    this->hide();
    registerWindow->show();
}

