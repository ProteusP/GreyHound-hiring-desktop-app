#ifndef REGISTERWINDOWFORCANDIDATE_H
#define REGISTERWINDOWFORCANDIDATE_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class RegisterWindowForCandidate;
}

class RegisterWindowForCandidate : public QMainWindow {
  Q_OBJECT

public:
  explicit RegisterWindowForCandidate(QMainWindow *mainWindow_,
                                      QWidget *parent = nullptr);
  ~RegisterWindowForCandidate();

private slots:
  void on_PBregistrationCandidate_clicked();

private:
  Ui::RegisterWindowForCandidate *ui;
  QMainWindow *mainWindow;
};

#endif // REGISTERWINDOWFORCANDIDATE_H
