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
#include <QNetworkAccessManager>

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

    QString getEmail() { return email; }

    void setEmail(QString email_) { email = email_; }

  private slots:
    void onMainPage(bool isCandidate);
    void onRegisterStatusPage();
    void onBackToLoginPage();
    void onStatusCandidatePage();
    void onStatusEmployerPage();
    void onBackToRegisterStatusPage();
    void onProfilePage();
    void loadProfileData();
    void registerSuccess();
    void onProfileCandidateHomeClicked();
    void onProfileEmployerHomeClicked();
    void onLogoutClicked();

  private:
    QNetworkAccessManager *networkManager;
    Ui::MainWindow *ui;
    QString email;
    LoginWidget *loginPage;
    RegisterStatus *registerStatusPage;
    RegisterPageForCandidate *registerCandidatePage;
    RegisterPageForEmployer *registerEmployerPage;
    MainPage *mainPage;
    ProfilePageForCandidate *profileCandidatePage;
    ProfilePageForEmployer *profileEmployerPage;
    friend class MainWindowTest;
};
#endif // MAINWINDOW_H
