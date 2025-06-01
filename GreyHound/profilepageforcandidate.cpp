#include "profilepageforcandidate.h"
#include <QGroupBox>
#include <QMessageBox>
#include "ui_profilepageforcandidate.h"

ProfilePageForCandidate::ProfilePageForCandidate(QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfilePageForCandidate) {
    ui->setupUi(this);
    SetupUI();
}

ProfilePageForCandidate::~ProfilePageForCandidate() = default;

void ProfilePageForCandidate::SetupUI() {
    ui->statusCombo->addItem("Активно ищу работу", "active");
    ui->statusCombo->addItem("Рассматриваю предложения", "searching");
    ui->statusCombo->addItem("Не ищу работу", "inactive");
    ui->experinceCombo->addItem("Нет", "intern");
    ui->experinceCombo->addItem("1 - 3 года", "junior");
    ui->experinceCombo->addItem("3 - 5 лет", "middle");
    ui->experinceCombo->addItem("5+ лет", "senior");
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

void ProfilePageForCandidate::updateUserData(
    const QString &name,
    const QString &email,
    const QString &surname,
    const QString &phoneNum,
    const QString &place
) {
    ui->nameLabel->setText(name);
    ui->surnameLabel->setText(surname);
    ui->emailLabel->setText(email);
    ui->phoneEdit->setText(phoneNum);
    ui->placeEdit->setText(place);
}

void ProfilePageForCandidate::onSaveClicked() {
    QString newPhone = ui->phoneEdit->text();
    QString newPlace = ui->placeEdit->text();
    QString newSearchStatus = ui->statusCombo->currentData().toString();
    saveChangesToDB(newPhone, newPlace, newSearchStatus);
    saveResumeData();
}

void ProfilePageForCandidate::saveChangesToDB(
    const QString &newPhone,
    const QString &newPlace,
    const QString &newSearchStatus
) {
    QSqlQuery query;
    query.prepare(
        "UPDATE candidates "
        "JOIN search_statuses ON search_statuses.name = :new_search_status "
        "SET "
        "candidates.phone_num = :phone, "
        "candidates.place = :new_place, "
        "candidates.search_status_id = search_statuses.id "
        "WHERE candidates.email = :email"
    );
    QString email = ui->emailLabel->text();
    query.bindValue(":email", email);
    query.bindValue(":phone", newPhone);
    query.bindValue(":new_place", newPlace);
    query.bindValue(":new_search_status", newSearchStatus);

    if (!query.exec()) {
        QMessageBox::critical(
            this, "Ошибка",
            "Не удалось обновить данные: " + query.lastError().text()
        );
    } else {
        QMessageBox::information(this, "Успех", "Данные сохранены!");
    }
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
