#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "registerwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

LoginWindow::LoginWindow(QMainWindow *mainWindow_,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow),
    mainWindow(mainWindow_)
{
    ui->setupUi(this);
    this->setWindowTitle("Login window");
    this->resize(800,600);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("your_host_name");
    db.setDatabaseName("your_db");
    db.setUserName("your_username");
    db.setPassword("your_password");

    if (!db.open()) {
        qDebug() << "Error: Unable to connect to the database.";
        qDebug() << db.lastError().text();
    } else {
        qDebug() << "Connected to the database successfully!";
    }

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

        this->close();
        mainWindow->show();
    }
}


void LoginWindow::on_registerPB_clicked()
{
    RegisterWindow *registerWindow = new RegisterWindow();
    this->hide();
    registerWindow->show();
}

