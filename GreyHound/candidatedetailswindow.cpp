#include "candidatedetailswindow.h"
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTemporaryFile>
#include <QUrl>
#include "ui_candidatedetailswindow.h"

candidateDetailsWindow::candidateDetailsWindow(
    QNetworkAccessManager *manager,
    const int &user_id_,
    QWidget *parent
)
    : QDialog(parent),
      ui(new Ui::candidateDetailsWindow),
      networkManager(manager),
      user_id(user_id_) {
    ui->setupUi(this);
    QNetworkRequest request;
    request.setUrl(
        QUrl(QString("http://localhost:80/api/v1/resources/candidateInfo/%1")
                 .arg(user_id))
    );
    request.setRawHeader("Accept", "application/json");
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseInfo = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseInfo);
            QJsonObject obj = doc.object();
            ui->name->setText(obj["name"].toString());
            ui->surname->setText(obj["surname"].toString());
            ui->city->setText(obj["place"].toString());
            ui->email->setText(obj["email"].toString());
            ui->phone->setText(obj["phone_num"].toString());
            ui->education->setText(obj["study_info"].toString());
            QString experienceString;
            switch (obj["experience_status_id"].toInt()
            ) {  // TODO: create file-getter
                case 1:
                    experienceString = "Без опыта";
                    break;
                case 2:
                    experienceString = "1-3 лет";
                    break;
                case 3:
                    experienceString = "3-5 лет";
                    break;
                case 4:
                    experienceString = "5+ лет";
                    break;
                default:
                    experienceString = "хз скок";
                    break;
            }
            ui->experience->setText(experienceString);
        } else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 404) {
                QMessageBox::warning(this, "Ошибка", "Пользователь не найден");
            } else if (statusCode == 500) {
                QMessageBox::warning(
                    this, "Ошибка", "Ошибка на нашей стороне..."
                );
            } else {
                QMessageBox::warning(this, "Ошибка", "Что-то непонятное");
            }
        }
        reply->deleteLater();
    });
}

candidateDetailsWindow::~candidateDetailsWindow() {
    delete ui;
}

void candidateDetailsWindow::on_exitButton_clicked() {
    this->close();
}

void candidateDetailsWindow::on_resumeButton_clicked() {
    QString url =
        QString("http://127.0.0.1:80/api/v1/resources/resume/%1").arg(user_id);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray pdfData = reply->readAll();
            QFile file("resume.pdf");
            if (file.open(QIODevice::WriteOnly)) {
                QTemporaryFile tempFile;
                tempFile.setAutoRemove(false);
                tempFile.open();
                tempFile.write(pdfData);
                tempFile.close();
                QDesktopServices::openUrl(QUrl::fromLocalFile(tempFile.fileName(
                )));
            }
        } else {
            QMessageBox::warning(this, "Упс...", "Ничего не найдено.");
        }
        reply->deleteLater();
    });
}
