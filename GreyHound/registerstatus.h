#ifndef REGISTERSTATUS_H
#define REGISTERSTATUS_H

#include <QMainWindow>

namespace Ui {
class RegisterStatus;
}

class RegisterStatus : public QMainWindow {
  Q_OBJECT

public:
  explicit RegisterStatus(QMainWindow *mainWindow_, QWidget *parent = nullptr);
  ~RegisterStatus();

private slots:
  void on_statusCandidatePB_clicked();

  void on_statusEmployerPB_clicked();

private:
  Ui::RegisterStatus *ui;
  QMainWindow *mainWindow;
};

#endif // REGISTERSTATUS_H
