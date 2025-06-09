#include "mainwindow.h"
#include <QNetworkCookieJar>
#include <QString>
#include <QDebug>
#include "./ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
    networkManager->setCookieJar(new QNetworkCookieJar(networkManager));

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->addWidget(ui->stackedWidget);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("92.63.178.117");
    db.setDatabaseName("default_db");
    db.setUserName("gen_user");
    db.setPassword("HskcQ!tRbm}f05");

    if (!db.open()) {
        qDebug() << "Error: Unable to connect to the database.";
        qDebug() << db.lastError().text();
    } else {
        qDebug() << "Connected to the database successfully!";
    }

    loginPage = new LoginWidget(networkManager, this);
    registerStatusPage = new RegisterStatus(this);
    registerCandidatePage = new RegisterPageForCandidate(networkManager, this);
    registerEmployerPage = new RegisterPageForEmployer(networkManager, this);
    mainPage = new MainPage(networkManager, this);
    profileCandidatePage = new ProfilePageForCandidate(networkManager, this);
    profileEmployerPage = new ProfilePageForEmployer(networkManager, this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(registerStatusPage);
    ui->stackedWidget->addWidget(registerCandidatePage);
    ui->stackedWidget->addWidget(registerEmployerPage);
    ui->stackedWidget->addWidget(mainPage);
    ui->stackedWidget->addWidget(profileCandidatePage);
    ui->stackedWidget->addWidget(profileEmployerPage);

    ui->stackedWidget->setCurrentWidget(loginPage);
    connect(loginPage, &LoginWidget::loginSuccessful, this, [this](bool isemployee) {
        onMainPage(isemployee);
    });
    connect(
        loginPage, &LoginWidget::registerPressed, this,
        &MainWindow::onRegisterStatusPage
    );
    connect(
        registerStatusPage, &RegisterStatus::backToLoginPressed, this,
        &MainWindow::onBackToLoginPage
    );
    connect(
        registerStatusPage, &RegisterStatus::statusCandidatePressed, this,
        &MainWindow::onStatusCandidatePage
    );
    connect(
        registerStatusPage, &RegisterStatus::statusEmployerPressed, this,
        &MainWindow::onStatusEmployerPage
    );
    connect(
        registerCandidatePage,
        &RegisterPageForCandidate::backToRegisterStatusPagePressed, this,
        &MainWindow::onBackToRegisterStatusPage
    );
    connect(
        registerEmployerPage,
        &RegisterPageForEmployer::backToRegisterStatusPagePressed, this,
        &MainWindow::onBackToRegisterStatusPage
    );
    connect(
        registerCandidatePage, &RegisterPageForCandidate::registerSuccessful,
        this, [this]() { onBackToLoginPage(); }
    );
    connect(
        registerEmployerPage, &RegisterPageForEmployer::registerSuccessful,
        this, [this]() { onBackToLoginPage(); }
    );
    connect(
        mainPage, &MainPage::onProfilePressed, this, &MainWindow::onProfilePage
    );
    connect(
        profileCandidatePage, &ProfilePageForCandidate::homeButtonClicked, this,
        [this]() { onMainPage(true); }
    );
    connect(
        profileEmployerPage, &ProfilePageForEmployer::homeButtonClicked, this,
        [this]() { onMainPage(false); }
    );
    connect(
        profileEmployerPage, &ProfilePageForEmployer::logoutButtonClicked, this,
        [this]() { onBackToLoginPage(); }
    );
    connect(
        profileCandidatePage, &ProfilePageForCandidate::logoutButtonClicked,
        this,
        [this]() {
            // TODO
            // clear text in login page after pageback
            onBackToLoginPage();
        }
    );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMainPage(bool isemployee) {
    mainPage->setStatusOfCandidate(isemployee);
    if (mainPage->getFlowLayout() != nullptr) {
        mainPage->hide();
    }
    mainPage->show();
    ui->stackedWidget->setCurrentWidget(mainPage);
}

void MainWindow::onRegisterStatusPage() {
    ui->stackedWidget->setCurrentWidget(registerStatusPage);
}

void MainWindow::onBackToLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::onStatusCandidatePage() {
    ui->stackedWidget->setCurrentWidget(registerCandidatePage);
}

void MainWindow::onStatusEmployerPage() {
    ui->stackedWidget->setCurrentWidget(registerEmployerPage);
}

void MainWindow::onBackToRegisterStatusPage() {
    ui->stackedWidget->setCurrentWidget(registerStatusPage);
}

void MainWindow::loadProfileData() {
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:80/api/v1/profile/"));
    request.setRawHeader("Accept", "application/json");
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray responseInfo = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseInfo);
        QJsonObject obj = doc.object();
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << obj["status"].toString() << '\n';
            if (obj["status"].toString() == "candidate") {
                profileCandidatePage->setCandidateData(
                 obj["name"].toString(), obj["email"].toString(), obj["surname"].toString(),
                    obj["phone_num"].toString(), obj["place"].toString(), obj["search_status_id"].toInt(),
                 obj["faculty_of_educ"].toString(), obj["place_of_study"].toString(), obj["experience_status_id"].toInt()
                );
                ui->stackedWidget->setCurrentWidget(profileCandidatePage);
            }
            else if (obj["status"].toString() == "empl") {
                profileEmployerPage->setEmployerData(obj["company_name"].toString(),
                                                        obj["email"].toString(), obj["about"].toString());
                ui->stackedWidget->setCurrentWidget(profileEmployerPage);
            }
        }
        else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 404) {
                QMessageBox::warning(this, "Упс...", "Ты не найден в нашей базе(");
            }
            else if (statusCode == 400) {
                QMessageBox::warning(this, "Упс...", "Статус не определён");
            }
            else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное");
            }
        }
        reply->deleteLater();
    });
    return;
}

void MainWindow::onProfilePage() {
    loadProfileData();
}
