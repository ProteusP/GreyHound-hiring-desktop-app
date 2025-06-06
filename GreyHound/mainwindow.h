#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include "loginpage.h"
#include "mainpage.h"
#include "profilepageforcandidate.h"
#include "profilepageforemployer.h"
#include "registerpageforcandidate.h"
#include "registerpageforemployer.h"
#include "registerstatus.h"

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

    QString getEmail() {
        return email;
    }

    void setEmail(QString email_) {
        email = email_;
    }

    void setStatus(bool status_) {
        isemployee = status_;
    }

    bool get_status() const noexcept {
        return isemployee;
    }

private slots:
    void onMainPage();
    void onRegisterStatusPage();
    void onBackToLoginPage();
    void onStatusCandidatePage();
    void onStatusEmployerPage();
    void onBackToRegisterStatusPage();
    void onProfilePage();
    void loadProfileData();

private:
    bool isemployee;
    QNetworkAccessManager *networkManager;
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString email;
    LoginWidget *loginPage;
    RegisterStatus *registerStatusPage;
    RegisterPageForCandidate *registerCandidatePage;
    RegisterPageForEmployer *registerEmployerPage;
    MainPage *mainPage;
    ProfilePageForCandidate *profileCandidatePage;
    ProfilePageForEmployer *profileEmployerPage;
};
#endif  // MAINWINDOW_H
