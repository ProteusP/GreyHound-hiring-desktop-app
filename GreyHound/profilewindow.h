#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit ProfileWindow(MainWindow *mainWindow_, QWidget *parent = nullptr);
  ~ProfileWindow();

private slots:
  void on_homePB_clicked();

private:
  Ui::ProfileWindow *ui;
  MainWindow *mainWindow;
};

#endif // PROFILEWINDOW_H
