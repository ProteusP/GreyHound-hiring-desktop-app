#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginpage.h"
#include "mainpage.h"
#include "profilepageforcandidate.h"
#include "profilepageforemployer.h"
#include "registerpageforcandidate.h"
#include "registerpageforemployer.h"
#include "registerstatus.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onMainPage();
  void onRegisterStatusPage();
  void onBackToLoginPage();
  void onStatusCandidatePage();
  void onStatusEmployerPage();
  void onBackToRegisterStatusPage();
  void onProfilePage();

private:
  Ui::MainWindow *ui;
  QSqlDatabase db;
  bool isemployee;
  LoginWidget *loginPage;
  RegisterStatus *registerStatusPage;
  RegisterPageForCandidate *registerCandidatePage;
  RegisterPageForEmployer *registerEmployerPage;
  MainPage *mainPage;
  ProfilePageForCandidate *profileCandidatePage;
  ProfilePageForEmployer *profileEmployerPage;
};
#endif // MAINWINDOW_H
