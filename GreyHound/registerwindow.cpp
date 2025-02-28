#include "registerwindow.h"
#include "ui_registerwindow.h"

// TODO: Миша и/или глеб
// Нужно задезайнить окно регистрации и
// оформить весь функционал
RegisterWindow::RegisterWindow(QMainWindow *mainWindow_, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RegisterWindow), mainWindow(mainWindow_) {
  ui->setupUi(this);
  this->resize(800, 600);
  this->setWindowTitle("Register window");
}

RegisterWindow::~RegisterWindow() { delete ui; }

void RegisterWindow::on_registerPB_clicked() {
  QString name = ui->lineName->text();
  QString surname = ui->surnameLine->text();
  QString email = ui->lineEmail->text();
  QString password = ui->linePassword->text();

  QSqlQuery query;
  query.prepare("INSERT INTO users (name, surname, email, password) VALUES "
                "(:name, :surname, :email, :password)");
  query.bindValue(":name", name);
  query.bindValue(":surname", surname);
  query.bindValue(":email", email);
  query.bindValue(":password", password);
  query.exec();

  QMessageBox::information(this, "", "Вы зарегистрировались!");
  this->close();
  mainWindow->show();
}
