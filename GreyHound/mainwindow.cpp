#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

    QWidget *centralWidget =new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* centralLayout = new QHBoxLayout(centralWidget);
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

  connect(loginPage, &LoginWidget::loginSuccessful, this,
          &MainWindow::onMainPage);
  connect(loginPage, &LoginWidget::registerPressed, this,
          &MainWindow::onRegisterStatusPage);
  connect(registerStatusPage, &RegisterStatus::backToLoginPressed, this,
          &MainWindow::onBackToLoginPage);
  connect(registerStatusPage, &RegisterStatus::statusCandidatePressed, this,
          &MainWindow::onStatusCandidatePage);
  connect(registerStatusPage, &RegisterStatus::statusEmployerPressed, this,
          &MainWindow::onStatusEmployerPage);
  connect(registerCandidatePage,
          &RegisterPageForCandidate::backToRegisterStatusPagePressed, this,
          &MainWindow::onBackToRegisterStatusPage);
  connect(registerEmployerPage,
          &RegisterPageForEmployer::backToRegisterStatusPagePressed, this,
          &MainWindow::onBackToRegisterStatusPage);
  connect(registerCandidatePage, &RegisterPageForCandidate::registerSuccessful,
          this, &MainWindow::onMainPage);
  connect(registerEmployerPage, &RegisterPageForEmployer::registerSuccessful,
          this, &MainWindow::onMainPage);
  connect(mainPage, &MainPage::onProfilePressed, this,
          &MainWindow::onProfilePage);

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onMainPage() { ui->stackedWidget->setCurrentWidget(mainPage); }

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

void MainWindow::onProfilePage() {
  if (isemployee) {
    ui->stackedWidget->setCurrentWidget(profileEmployerPage);
  } else {
    ui->stackedWidget->setCurrentWidget(profileCandidatePage);
  }
}
