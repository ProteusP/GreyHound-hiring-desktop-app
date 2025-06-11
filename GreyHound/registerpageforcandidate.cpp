#include "registerpageforcandidate.h"
#include <QGraphicsDropShadowEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "hashing.h"
#include "passwordwarningdialog.h"
#include "ui_registerpageforcandidate.h"
#include "validation.h"

RegisterPageForCandidate::RegisterPageForCandidate(
    QNetworkAccessManager *manager,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::RegisterPageForCandidate),
      networkManager(manager) {
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
    QString hashedPassword = hashPassword(password);

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
    if (!isEmailValid(email)) {
        QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:80/api/v1/Auth/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["status"] = "candidate";
    json["name"] = name;
    json["surname"] = surname;
    json["email"] = email;
    json["password"] = hashedPassword;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit registerSuccessful();
        } else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 409) {
                QMessageBox::warning(
                    this, "Ошибка", "Этот email уже зарегистрирован."
                );
            } else if (statusCode == 400) {
                QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            } else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное...");
            }
        }
        reply->deleteLater();
    });
    return;
}

void RegisterPageForCandidate::on_backToStatusPB_clicked() {
    emit backToRegisterStatusPagePressed();
}
