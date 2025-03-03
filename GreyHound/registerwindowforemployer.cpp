#include "registerwindowforemployer.h"
#include "ui_registerwindowforemployer.h"
#include "validation.h"

RegisterWindowForEmployer::RegisterWindowForEmployer(
    MainWindow *mainWindow_,
    QWidget *parent
)
    : QMainWindow(parent),
      ui(new Ui::RegisterWindowForEmployer),
      mainWindow(mainWindow_),
      parentStatus(parent) {
    ui->setupUi(this);
    this->setWindowTitle("Register window for employer");
    this->resize(800, 600);
}

RegisterWindowForEmployer::~RegisterWindowForEmployer() {
    delete ui;
}

void RegisterWindowForEmployer::on_registrationPB_employer_clicked() {
    QString company_name = ui->registrationLineEditCompanyNameEmployer->text();
    QString email = ui->registrationLineEditEmailEmployer->text();
    QString password = ui->registrationLineEditPasswordEmployer->text();

    std::vector<QString> fieldInputVec = {company_name, email, password};
    if (checkIfFieldsAreEmpty(
            this, fieldInputVec, "Никакое поле не должно быть пустым"
        )) {
        return;
    }

    if (validateEmail(email)) {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO employers (email, company_name, password) VALUES "
            "(:email, :company_name, :password)"
        );
        query.bindValue(":email", email);
        query.bindValue(":company_name", company_name);
        query.bindValue(":password", password);
        query.exec();

        QMessageBox::information(this, "", "Вы зарегистрировались!");
        this->close();
        mainWindow->show();
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
    }
}

void RegisterWindowForEmployer::on_backToStatusPB_clicked() {
    this->hide();
    parentStatus->show();
}
