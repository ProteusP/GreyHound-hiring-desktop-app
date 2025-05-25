#include "registerpageforcandidate.h"
#include "hashing.h"
#include "passwordwarningdialog.h"
#include "ui_registerpageforcandidate.h"
#include "validation.h"
#include <QGraphicsDropShadowEffect>
RegisterPageForCandidate::RegisterPageForCandidate(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterPageForCandidate) {
    ui->setupUi(this);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 50));
    ui->widget->setGraphicsEffect(shadow);
}

RegisterPageForCandidate::~RegisterPageForCandidate() {
    delete ui;
}

void RegisterPageForCandidate::on_PBregistrationCandidate_clicked() {
    QString name = ui->registrationLineNameCandidate->text();
    QString surname = ui->registrationLineSurnameCandidate->text();
    QString email = ui->registrationLineEmailCandidate->text();
    QString password = ui->registrationLinePasswordCandidate->text();

    std::vector<QString> fieldInputVec = {name, surname, email, password};
    if (fieldInputVec[0].isEmpty() || fieldInputVec[0].isEmpty()) {
        QMessageBox::warning(
            this, "Ошибка", "Никакое поле не должно быть пустым"
        );
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
            "INSERT INTO candidates (name, surname, email, password) VALUES "
            "(:name, :surname, :email, :password)"
        );
        query.bindValue(":name", name);
        query.bindValue(":surname", surname);
        query.bindValue(":email", email);
        query.bindValue(":password", hashedPassword);
        query.exec();

        emit registerSuccessful();
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
    }
}

void RegisterPageForCandidate::on_backToStatusPB_clicked() {
    emit backToRegisterStatusPagePressed();
}
