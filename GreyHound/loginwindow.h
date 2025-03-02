#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit LoginWindow(MainWindow *mainWindow_, QWidget *parent = nullptr);
  ~LoginWindow();

private slots:
  void on_exitPB_clicked();

  void on_loginPB_clicked();

  void on_registerPB_clicked();

private:
  Ui::LoginWindow *ui;
  MainWindow *mainWindow;
  QSqlDatabase db;
};

#endif // LOGINWINDOW_H
