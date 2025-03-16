#include "profilepageforcandidate.h"
#include <QGroupBox>
#include <QMessageBox>
#include "ui_profilepageforcandidate.h"

ProfilePageForCandidate::ProfilePageForCandidate(QWidget *parent)
    : QWidget(parent) {
    SetupUI();
}

ProfilePageForCandidate::~ProfilePageForCandidate() = default;

void ProfilePageForCandidate::SetupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    QScrollArea *leftScrollArea = new QScrollArea();
    QWidget *notificationsWidget = new QWidget();
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);
    QLabel *notificationsLabel = new QLabel("Уведомления");

    notificationsLayout->addWidget(notificationsLabel);

    // TODO add notif-s from BD

    for (int i = 1; i <= 20; i++) {
        notificationsLayout->addWidget(
            new QLabel(QString("Уведомления %1").arg(i))
        );
    }

    leftScrollArea->setWidget(notificationsWidget);
    leftScrollArea->setWidgetResizable(true);

    mainLayout->addWidget(leftScrollArea, 1);

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    QGroupBox *basicInfoGroup = new QGroupBox("Основная информация");
    QFormLayout *basicInfoLayout = new QFormLayout(basicInfoGroup);

    // TODO add fields from DB

    nameLabel = new QLabel("Загрузка...");
    surnameLabel = new QLabel("Загрузка...");
    emailLabel = new QLabel("Загрузка...");
    phoneEdit = new QLineEdit();
    placeEdit = new QLineEdit();
    statusCombo = new QComboBox();
    statusCombo->addItem("Активно ищу работу", "active");
    statusCombo->addItem("Рассматриваю предложения", "searching");
    statusCombo->addItem("Не ищу работу", "inactive");

    basicInfoLayout->addRow("Имя", nameLabel);
    basicInfoLayout->addRow("Фамилия", surnameLabel);
    basicInfoLayout->addRow("Email", emailLabel);
    basicInfoLayout->addRow("Телефон", phoneEdit);
    basicInfoLayout->addRow("Местоположение", placeEdit);
    basicInfoLayout->addRow("Статус", statusCombo);

    QGroupBox *resumeGroup = new QGroupBox("Резюме");
    QFormLayout *resumeLayout = new QFormLayout(resumeGroup);

    universityEdit = new QLineEdit();
    facultyEdit = new QLineEdit();
    experinceCombo = new QComboBox();
    experinceCombo->addItem("Нет", "intern");
    experinceCombo->addItem("1 - 3 года", "junior");
    experinceCombo->addItem("3 - 5 лет", "middle");
    experinceCombo->addItem("5+ лет", "senior");

    resumeLayout->addRow("Учебное заведение", universityEdit);
    resumeLayout->addRow("Факультет", facultyEdit);
    resumeLayout->addRow("Опыт работы", experinceCombo);

    resumeLayout->setContentsMargins(20, 20, 20, 20);
    basicInfoLayout->setContentsMargins(20, 20, 20, 20);

    centralLayout->addWidget(basicInfoGroup);
    centralLayout->addSpacing(15);
    centralLayout->addWidget(resumeGroup);
    centralLayout->addStretch();

    mainLayout->addWidget(centralWidget, 3);

    QWidget *rightWidget = new QWidget();

    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QPushButton *homepagePB = new QPushButton("На главную");
    connect(
        homepagePB, &QPushButton::clicked, this,
        &ProfilePageForCandidate::homeButtonClicked
    );

    QPushButton *saveButton = new QPushButton("Сохранить изменения");

    connect(
        saveButton, &QPushButton::clicked, this,
        &ProfilePageForCandidate::onSaveClicked
    );

    QPushButton *logoutPB = new QPushButton("Выйти");
    logoutPB->setStyleSheet(
        "QPushButton {"
        "   color: red;}"
    );
    connect(
        logoutPB, &QPushButton::clicked, this,
        &ProfilePageForCandidate::logoutButtonClicked
    );

    rightLayout->addWidget(homepagePB, 0, Qt::AlignTop);
    rightLayout->addStretch();
    rightLayout->addWidget(saveButton, 0, Qt::AlignBottom);
    rightLayout->addWidget(logoutPB, 0, Qt::AlignBottom);
    rightWidget->setContentsMargins(10, 10, 10, 10);

    mainLayout->addWidget(rightWidget, 1);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);

    leftScrollArea->setSizePolicy(
        QSizePolicy::Expanding, QSizePolicy::Expanding
    );
    centralWidget->setSizePolicy(
        QSizePolicy::Expanding, QSizePolicy::Expanding
    );
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ProfilePageForCandidate::updateUserData(
    const QString &name,
    const QString &email,
    const QString &surname,
    const QString &phoneNum,
    const QString &place
) {
    nameLabel->setText(name);
    surnameLabel->setText(surname);
    emailLabel->setText(email);
    phoneEdit->setText(phoneNum);
    placeEdit->setText(place);
}

void ProfilePageForCandidate::onSaveClicked() {
    QString newPhone = phoneEdit->text();
    QString newPlace = placeEdit->text();
    saveChangesToDB(newPhone, newPlace);
    saveResumeData();
}

void ProfilePageForCandidate::saveChangesToDB(
    const QString &newPhone,
    const QString &newPlace
) {
    QSqlQuery query;
    query.prepare(
        "UPDATE candidates SET phone_num = :phone, place = :new_place WHERE "
        "email = :email"
    );
    QString email = emailLabel->text();
    query.bindValue(":email", email);
    query.bindValue(":phone", newPhone);
    query.bindValue(":new_place", newPlace);

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
    QString email = emailLabel->text();
    query.bindValue(":email", email);
    if (!query.exec() || !query.next()) {
        qDebug() << "Не удалось получить данные для резюме";
        return;
    }

    auto university = query.value(0).toString();
    auto faculty = query.value(1).toString();

    universityEdit->setText(university);
    facultyEdit->setText(faculty);
    // TODO add experience and smth else...
}

void ProfilePageForCandidate::saveResumeData() {
    QSqlQuery query;
    // TODO add experience and smth else...
    query.prepare(
        "UPDATE candidates SET  place_of_study =:university, faculty_of_educ "
        "=:faculty WHERE email =:email"
    );

    QString email = emailLabel->text();
    QString university = universityEdit->text();
    QString faculty = facultyEdit->text();

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
