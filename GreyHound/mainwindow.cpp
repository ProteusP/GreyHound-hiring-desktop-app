#include "mainwindow.h"
#include <QString>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

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

    loginPage = new LoginWidget(this);
    registerStatusPage = new RegisterStatus(this);
    registerCandidatePage = new RegisterPageForCandidate(this);
    registerEmployerPage = new RegisterPageForEmployer(this);
    mainPage = new MainPage(this);
    profileCandidatePage = new ProfilePageForCandidate(this);
    profileEmployerPage = new ProfilePageForEmployer(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(registerStatusPage);
    ui->stackedWidget->addWidget(registerCandidatePage);
    ui->stackedWidget->addWidget(registerEmployerPage);
    ui->stackedWidget->addWidget(mainPage);
    ui->stackedWidget->addWidget(profileCandidatePage);
    ui->stackedWidget->addWidget(profileEmployerPage);

    ui->stackedWidget->setCurrentWidget(loginPage);

    connect(
        loginPage, &LoginWidget::loginSuccessful, this,
        [this](const QString &email, bool status) {
            setEmail(email);
            setStatus(status);
            onMainPage();
        }
    );
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
        this, [this](const QString &email, bool status) {
            setEmail(email);
            setStatus(status);
            onMainPage();
        }
    );
    connect(
        registerEmployerPage, &RegisterPageForEmployer::registerSuccessful,
        this, [this](const QString &email, bool status) {
            setEmail(email);
            setStatus(status);
            onMainPage();
        }
    );
    connect(
        mainPage, &MainPage::onProfilePressed, this, &MainWindow::onProfilePage
    );
    connect(
        profileCandidatePage, &ProfilePageForCandidate::homeButtonClicked, this,
        &MainWindow::onMainPage
    );
    connect(
        profileEmployerPage, &ProfilePageForEmployer::homeButtonClicked, this,
        &MainWindow::onMainPage
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

void MainWindow::onMainPage() {
    mainPage->setStatusOfCandidate(isemployee);
    if (mainPage->getFlowLayout() != nullptr) {
        mainPage->hide();
    }
    mainPage->setEmail(email);
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
    this->isemployee = true;
    ui->stackedWidget->setCurrentWidget(registerCandidatePage);
}

void MainWindow::onStatusEmployerPage() {
    this->isemployee = false;
    ui->stackedWidget->setCurrentWidget(registerEmployerPage);
}

void MainWindow::onBackToRegisterStatusPage() {
    ui->stackedWidget->setCurrentWidget(registerStatusPage);
}

void MainWindow::loadProfileData() {
    QString email = getEmail();
    QSqlQuery query;
    if (isemployee) {
        qDebug() << " im in candidat loading...";
        query.prepare(
            "SELECT name,surname,phone_num, place FROM candidates WHERE email "
            "= "
            ":email"
        );

        query.bindValue(":email", email);
        if (!query.exec() || !query.next()) {
            qDebug() << "Ошибка загрузки данных: " << query.lastError().text();
            return;
        }

        QString name = query.value(0).toString();
        QString surname = query.value(1).toString();
        QString phoneNum = query.value(2).toString();
        QString place = query.value(3).toString();
        qDebug() << name << " " << surname << " " << phoneNum << " " << place
                 << " " << email;
        profileCandidatePage->updateUserData(
            name, email, surname, phoneNum, place
        );
        profileCandidatePage->loadResumeData();

    } else {
        qDebug() << "im in employer loading...";
        query.prepare(
            "SELECT id, company_name, about FROM employers WHERE email ="
            ":email"
        );
        query.bindValue(":email", email);
        if (!query.exec() || !query.next()) {
            qDebug() << "Ошибка загрузки данных: " << query.lastError().text();
            return;
        }

        int ID = query.value(0).toInt();
        QString companyName = query.value(1).toString();
        QString about = query.value(2).toString();

        qDebug() << ID << " " << companyName << " " << about;
        profileEmployerPage->updateEmployerData(companyName, email, about, ID);
    }
}

void MainWindow::onProfilePage() {
    loadProfileData();
    if (isemployee) {
        ui->stackedWidget->setCurrentWidget(profileCandidatePage);
    } else {
        ui->stackedWidget->setCurrentWidget(profileEmployerPage);
    }
}
