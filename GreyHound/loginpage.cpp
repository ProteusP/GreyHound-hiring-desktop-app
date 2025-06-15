#include "loginpage.h"
#include "hashing.h"
#include "ui_loginpage.h"
#include <QGraphicsDropShadowEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>

LoginWidget::LoginWidget(QNetworkAccessManager *manager, QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWidget), networkManager(manager) {
    ui->setupUi(this);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));
    ui->widgetLogin->setGraphicsEffect(shadow);
}

LoginWidget::~LoginWidget() { delete ui; }

void LoginWidget::on_loginPB_clicked() {
    QString email = ui->mailLine->text();
    QString hashingPassword = hashPassword(ui->passwordLine->text());
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:80/api/v1/Auth/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["email"] = email;
    json["password"] = hashingPassword;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseInfo = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseInfo);
            QJsonObject obj = doc.object();
            emit loginSuccessful(obj["status_user"] == "candidate");
        } else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 400) {
                QMessageBox::warning(this, "Ошибка",
                                     "Никакое поле не должно быть пустым.");
            } else if (statusCode == 401) {
                QMessageBox::warning(this, "Упс...",
                                     "Неверный логин или пароль.");
            } else if (statusCode == 500) {
                QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            } else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное.");
            }
        }
        reply->deleteLater();
    });
    return;
}

void LoginWidget::on_registerPB_clicked() { emit registerPressed(); }
