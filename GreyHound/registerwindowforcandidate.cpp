#include "registerwindowforcandidate.h"
#include "ui_registerwindowforcandidate.h"

RegisterWindowForCandidate::RegisterWindowForCandidate(
    MainWindow *mainWindow_,
    QWidget *parent
)
    : QMainWindow(parent),
      ui(new Ui::RegisterWindowForCandidate),
      mainWindow(mainWindow_),
      parentStatus(parent) {
    ui->setupUi(this);
    this->setWindowTitle("Register window for candidate");
    this->resize(800, 600);
}

RegisterWindowForCandidate::~RegisterWindowForCandidate() {
    delete ui;
}

void RegisterWindowForCandidate::on_PBregistrationCandidate_clicked() {
    QString name = ui->registrationLineNameCandidate->text();
    QString surname = ui->registrationLineSurnameCandidate->text();
    QString email = ui->registrationLineEmailCandidate->text();
    QString password = ui->registrationLinePasswordCandidate->text();

    QSqlQuery query;
    query.prepare(
        "INSERT INTO candidates (name, surname, email, password) VALUES "
        "(:name, :surname, :email, :password)"
    );
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.exec();

    QMessageBox::information(this, "", "Вы зарегистрировались!");
    this->close();
    mainWindow->show();
}

void RegisterWindowForCandidate::on_backToStatusPB_clicked() {
    this->hide();
    parentStatus->show();
}
