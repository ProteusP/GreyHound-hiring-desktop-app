#ifndef REGISTERWINDOWFOREMPLOYER_H
#define REGISTERWINDOWFOREMPLOYER_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class RegisterWindowForEmployer;
}

class RegisterWindowForEmployer : public QMainWindow {
  Q_OBJECT

public:
  explicit RegisterWindowForEmployer(QMainWindow *mainWindow_,
                                     QWidget *parent = nullptr);
  ~RegisterWindowForEmployer();

private slots:
  void on_registrationPB_employer_clicked();

private:
  Ui::RegisterWindowForEmployer *ui;
  QMainWindow *mainWindow;
};

#endif // REGISTERWINDOWFOREMPLOYER_H
