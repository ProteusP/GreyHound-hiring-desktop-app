#include "profilepageforcandidate.h"
#include "ui_profilepageforcandidate.h"
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

ProfilePageForCandidate::ProfilePageForCandidate(QNetworkAccessManager *manager,
                                                 QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfilePageForCandidate),
      networkManager(manager) {
    ui->setupUi(this);
    SetupUI();
}

ProfilePageForCandidate::~ProfilePageForCandidate() = default;

void ProfilePageForCandidate::SetupUI() {
    ui->statusCombo->addItem("Активно ищу работу", "1");
    ui->statusCombo->addItem("Рассматриваю предложения", "2");
    ui->statusCombo->addItem("Не ищу работу", "3");
    ui->experinceCombo->addItem("Нет", "1");
    ui->experinceCombo->addItem("1 - 3 года", "2");
    ui->experinceCombo->addItem("3 - 5 лет", "3");
    ui->experinceCombo->addItem("5+ лет", "4");
    connect(ui->homepagePB, &QPushButton::clicked, this,
            &ProfilePageForCandidate::homeButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this,
            &ProfilePageForCandidate::onSaveClicked);
    connect(ui->logoutPB, &QPushButton::clicked, this,
            &ProfilePageForCandidate::logoutButtonClicked);
}

void ProfilePageForCandidate::setCandidateData(
    const QString &name, const QString &email, const QString &surname,
    const QString &phoneNum, const QString &place,
    const qint16 &search_status_id, const QString &faculty_of_educ,
    const QString &place_of_study, const qint16 &experience_status_id) {
    ui->nameEdit->setText(name);
    ui->surnameEdit->setText(surname);
    ui->emailEdit->setText(email);
    ui->phoneEdit->setText(phoneNum);
    ui->placeEdit->setText(place);
    ui->statusCombo->setCurrentIndex(search_status_id - 1);
    ui->universityEdit->setText(place_of_study);
    ui->facultyEdit->setText(faculty_of_educ);
    ui->experinceCombo->setCurrentIndex(experience_status_id - 1);
}

void ProfilePageForCandidate::onSaveClicked() {
    QString newPhone = ui->phoneEdit->text();
    QString newPlace = ui->placeEdit->text();
    qint16 newSearchStatus = ui->statusCombo->currentData().toInt();
    QString newUniversity = ui->universityEdit->text();
    QString newFaculty = ui->facultyEdit->text();
    qint16 newExperience = ui->experinceCombo->currentData().toInt();
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:80/api/v1/profile/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["phone_num"] = newPhone;
    json["place"] = newPlace;
    json["search_status_id"] = newSearchStatus;
    json["place_of_study"] = newUniversity;
    json["faculty_of_educ"] = newFaculty;
    json["experience_status_id"] = newExperience;
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply *reply =
        networkManager->sendCustomRequest(request, "PATCH", data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Успех", "Данные сохранены!");
        } else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 400) {
                QMessageBox::warning(
                    this, "Ошибка",
                    "По пути на сервер данные потерялись( просим прощения");
            } else if (statusCode == 500) {
                QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            } else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное.");
            }
        }
        reply->deleteLater();
    });
}

void ProfilePageForCandidate::on_resumeButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл");
    if (!filePath.isEmpty()) {
        QFile *file = new QFile(filePath);
        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Ошибка открытия файла:" << file->errorString();
            delete file;
            return;
        }
        QHttpMultiPart *multiPart =
            new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QString("form-data; name=\"file\"; filename=\"%1\"")
                               .arg(QFileInfo(filePath).fileName()));
        filePart.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(filePart);
        QNetworkRequest request;
        QUrl url("http://localhost:80/api/v1/resources/resume");
        request.setUrl(url);
        QNetworkAccessManager manager;
        QNetworkReply *reply = networkManager->post(request, multiPart);
        multiPart->setParent(reply);
        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QMessageBox::information(this, "Успех", "Данные сохранены!");
            } else {
                QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            }
            reply->deleteLater();
        });
    }
}
