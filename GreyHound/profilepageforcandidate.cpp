#include "profilepageforcandidate.h"
#include "ui_profilepageforcandidate.h"
#include <QMessageBox>

ProfilePageForCandidate::ProfilePageForCandidate(QWidget *parent)
    : QWidget(parent) {
    SetupUI();
}

ProfilePageForCandidate::~ProfilePageForCandidate() = default;

void ProfilePageForCandidate::SetupUI(){
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    QScrollArea *leftScrollArea = new QScrollArea();
    QWidget *notificationsWidget = new QWidget();
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);
    QLabel *notificationsLabel = new QLabel("Уведомления");

    notificationsLayout->addWidget(notificationsLabel);

    //TODO add notif-s from BD

    for (int i  = 1; i <= 20; i++){
        notificationsLayout->addWidget(new QLabel(QString("Уведомления %1").arg(i)));
    }

    leftScrollArea->setWidget(notificationsWidget);
    leftScrollArea->setWidgetResizable(true);

    mainLayout->addWidget(leftScrollArea, 1);

    QWidget *centralWidget = new QWidget();

    QFormLayout *formLayout = new QFormLayout(centralWidget);


    //TODO add fields from DB

    nameLabel = new QLabel("Загрузка...");
    surnameLabel = new QLabel("Загрузка...");
    emailLabel = new QLabel("Загрузка...");
    phoneEdit = new QLineEdit();

    formLayout->addRow("Имя", nameLabel);
    formLayout->addRow("Фамилия", surnameLabel);
    formLayout->addRow("Email", emailLabel);
    formLayout->addRow("Телефон",phoneEdit);




    formLayout->setContentsMargins(20,20,20,20);

    mainLayout->addWidget(centralWidget,3);

    QWidget *rightWidget = new QWidget();

    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);



    QPushButton *homepagePB = new QPushButton("На главную");
    connect(homepagePB, &QPushButton::clicked,this, &ProfilePageForCandidate::homeButtonClicked);

    QPushButton *saveButton = new QPushButton("Сохранить изменения");

    connect(saveButton, &QPushButton::clicked, this, &ProfilePageForCandidate::onSaveClicked);

    QPushButton *logoutPB = new QPushButton("Выйти");
    logoutPB->setStyleSheet(
        "QPushButton {"
        "   color: red;}"
        );
    connect(logoutPB, &QPushButton::clicked, this, &ProfilePageForCandidate::logoutButtonClicked);


    rightLayout->addWidget(homepagePB,0,Qt::AlignTop);
    rightLayout->addStretch();
    rightLayout->addWidget(saveButton,0,Qt::AlignBottom);
    rightLayout->addWidget(logoutPB,0,Qt::AlignBottom);
    rightWidget->setContentsMargins(10,10,10,10);


    mainLayout->addWidget(rightWidget,1);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);

    leftScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}

void ProfilePageForCandidate::updateUserData(const QString &name, const QString &email, const QString &surname){
    nameLabel->setText(name);
    surnameLabel->setText(surname);
    emailLabel->setText(email);
}

void ProfilePageForCandidate::onSaveClicked(){
    QString newPhone = phoneEdit->text();

    saveChangesToDB(newPhone);
}

void ProfilePageForCandidate::saveChangesToDB(const QString &newPhone){
    QSqlQuery query;
    query.prepare("UPDATE candidates SET phone_num = :phone WHERE email = :email");
    QString email = emailLabel->text();
    query.bindValue(":email", email);
    query.bindValue(":phone", newPhone);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось обновить данные: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные сохранены!");
    }
}
