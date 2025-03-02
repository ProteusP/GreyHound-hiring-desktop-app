#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit ProfileWindow(QMainWindow *mainWindow_, QWidget *parent = nullptr);
  ~ProfileWindow();

private slots:
  void on_homePB_clicked();

private:
  Ui::ProfileWindow *ui;
  QMainWindow *mainWindow;
};

#endif // PROFILEWINDOW_H
