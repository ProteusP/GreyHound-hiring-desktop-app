#include "registerwindowforemployer.h"
#include "hashing.h"
#include "passwordwarningdialog.h"
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

    if (!isPasswordValid(password)) {
        PasswordWarningDialog dialog(this);
        dialog.exec();
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM candidates WHERE email = :email");
    checkQuery.bindValue(":email", email);

    if (!checkQuery.exec()) {
        QMessageBox::warning(
            this, "Ошибка", "Ошибка при выполнении запроса к базе данных."
        );
        return;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Ошибка", "Этот email уже зарегистрирован.");
        return;
    }

    QString hashedPassword = hashPassword(password);

    if (isEmailValid(email)) {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO employers (email, company_name, password) VALUES "
            "(:email, :company_name, :password)"
        );
        query.bindValue(":email", email);
        query.bindValue(":company_name", company_name);
        query.bindValue(":password", hashedPassword);
        query.exec();

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
