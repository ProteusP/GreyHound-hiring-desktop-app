#include "loginwindow.h"
#include "mainwindow.h"
#include "registerstatus.h"
#include "ui_loginwindow.h"
LoginWindow::LoginWindow(MainWindow *mainWindow_, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow), mainWindow(mainWindow_) {
  ui->setupUi(this);
  this->setWindowTitle("Login window");
  this->resize(800, 600);

  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("92.63.178.117");
  db.setDatabaseName("default_db");
  db.setUserName("gen_user");
  db.setPassword("HskcQ!tRbm}f05");

  if (!db.open()) {
    qDebug() << "Error: Unable to connect to the database.";
    qDebug() << db.lastError().text();
  } else {
    qDebug() << "Connected to the database successfully!";
  }
}

LoginWindow::~LoginWindow() { delete ui; }

void LoginWindow::on_exitPB_clicked() {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Выход", "Вы точно хотите выйти?",
                                QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::No) {
    return;
  }

  QApplication::exit();
}

void LoginWindow::on_loginPB_clicked() {
  QString userMail = ui->mailLine->text();
  QString userPassword = ui->passwordLine->text();

  if (!(userMail == userPassword)) {
    QMessageBox::warning(this, "Упс...", "Пароль или почта неверен!");
  } else {

    this->close();
    mainWindow->show();
  }
}

void LoginWindow::on_registerPB_clicked() {
  RegisterStatus *registerStatus = new RegisterStatus(mainWindow);
  this->hide();
  registerStatus->show();
}
