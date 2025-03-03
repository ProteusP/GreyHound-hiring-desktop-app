#ifndef REGISTERSTATUS_H
#define REGISTERSTATUS_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class RegisterStatus;
}

class RegisterStatus : public QMainWindow {
  Q_OBJECT

public:
  explicit RegisterStatus(MainWindow *mainWindow_, QWidget *parent = nullptr);
  ~RegisterStatus();

private slots:
  void on_statusCandidatePB_clicked();

  void on_statusEmployerPB_clicked();

  void on_backToLoginPB_clicked();

  private:
  Ui::RegisterStatus *ui;
  MainWindow *mainWindow;
  QWidget *parentLogin;
};

#endif // REGISTERSTATUS_H
