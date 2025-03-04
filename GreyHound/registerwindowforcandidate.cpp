#include "registerwindowforcandidate.h"
#include "hashing.h"
#include "ui_registerwindowforcandidate.h"
#include "validation.h"

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

    std::vector<QString> fieldInputVec = {name, surname, email, password};
    if (checkIfFieldsAreEmpty(
            this, fieldInputVec, "Никакое поле не должно быть пустым"
        )) {
        return;
    }

    QString hashedPassword = hashPassword(password);

    if (validateEmail(email)) {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO candidates (name, surname, email, password) VALUES "
            "(:name, :surname, :email, :password)"
        );
        query.bindValue(":name", name);
        query.bindValue(":surname", surname);
        query.bindValue(":email", email);
        query.bindValue(":password", hashedPassword);
        query.exec();

        this->close();
        mainWindow->show();
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
    }
}

void RegisterWindowForCandidate::on_backToStatusPB_clicked() {
    this->hide();
    parentStatus->show();
}
