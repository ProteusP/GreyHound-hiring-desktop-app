#include "profilepageforcandidate.h"
#include <QGroupBox>
#include <QMessageBox>
#include "ui_profilepageforcandidate.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

ProfilePageForCandidate::ProfilePageForCandidate(
    QNetworkAccessManager *manager,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::ProfilePageForCandidate),
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
    connect(
        ui->homepagePB, &QPushButton::clicked, this,
        &ProfilePageForCandidate::homeButtonClicked
    );
    connect(
        ui->saveButton, &QPushButton::clicked, this,
        &ProfilePageForCandidate::onSaveClicked
    );
    connect(
        ui->logoutPB, &QPushButton::clicked, this,
        &ProfilePageForCandidate::logoutButtonClicked
    );
}

void ProfilePageForCandidate::setCandidateData(
    const QString &name,
    const QString &email,
    const QString &surname,
    const QString &phoneNum,
    const QString &place,
    const qint16 &search_status_id,
    const QString &faculty_of_educ,
    const QString &place_of_study,
    const qint16 &experience_status_id
) {
    ui->nameEdit->setText(name);
    ui->surnameEdit->setText(surname);
    ui->emailEdit->setText(email);
    ui->phoneEdit->setText(phoneNum);
    ui->placeEdit->setText(place);
    // qDebug() << search_status_id << "<- серч статус, эксп статус -> " << experience_status_id << '\n';
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
    QNetworkReply *reply = networkManager->sendCustomRequest(request, "PATCH", data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Успех", "Данные сохранены!");
        }
        else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 400) {
                QMessageBox::warning(
                    this, "Ошибка", "По пути на сервер данные потерялись( просим прощения"
                    );
            } else if (statusCode == 500) {
                QMessageBox::warning(
                    this, "Упс...", "Ошибка сервера."
                    );
            } else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное.");
            }
        }
        reply->deleteLater();
    });
    // saveResumeData();
}


void ProfilePageForCandidate::loadResumeData() {
    QSqlQuery query;
    query.prepare(
        "SELECT place_of_study, faculty_of_educ FROM candidates WHERE email "
        "=:email"
    );
    QString email = ui->emailLabel->text();
    query.bindValue(":email", email);
    if (!query.exec() || !query.next()) {
        qDebug() << "Не удалось получить данные для резюме";
        return;
    }

    QString university = query.value(0).toString();
    QString faculty = query.value(1).toString();

    ui->universityEdit->setText(university);
    ui->facultyEdit->setText(faculty);
    // TODO add experience and smth else...
}

void ProfilePageForCandidate::saveResumeData() {
    QSqlQuery query;
    // TODO add experience and smth else...
    query.prepare(
        "UPDATE candidates SET  place_of_study =:university, faculty_of_educ "
        "=:faculty WHERE email =:email"
    );

    QString email = ui->emailLabel->text();
    QString university = ui->universityEdit->text();
    QString faculty = ui->facultyEdit->text();

    query.bindValue(":email", email);
    query.bindValue(":university", university);
    query.bindValue(":faculty", faculty);

    if (!query.exec()) {
        QMessageBox::critical(
            this, "Ошибка",
            "Ошибка сохранения резюме: " + query.lastError().text()
        );
    }
}
