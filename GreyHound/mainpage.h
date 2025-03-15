#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget {
  Q_OBJECT

public:
  explicit MainPage(QWidget *parent = nullptr);
  ~MainPage();

private:
  Ui::MainPage *ui;

private slots:
  void on_profilePB_3_clicked();

signals:
  void onProfilePressed();
};

#endif // MAINPAGE_H
