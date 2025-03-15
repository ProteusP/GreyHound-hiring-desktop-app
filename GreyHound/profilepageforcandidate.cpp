#include "profilepageforcandidate.h"
#include "ui_profilepageforcandidate.h"

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
    formLayout->addRow("Имя", new QLabel("МОЁ ИМЯ"));
    formLayout->addRow("Email", new QLabel("МОЙ МЕЙЛ"));
    formLayout->addRow("Телефон",new QLabel("МОЙ НОМЕР"));

    formLayout->setContentsMargins(20,20,20,20);

    mainLayout->addWidget(centralWidget,3);

    QWidget *rightWidget = new QWidget();

    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QPushButton *homepagePB = new QPushButton("На главную");

    QPushButton *logoutPB = new QPushButton("Выйти");

    rightLayout->addWidget(homepagePB,0,Qt::AlignTop);
    rightLayout->addStretch();
    rightLayout->addWidget(logoutPB,0,Qt::AlignBottom);
    rightWidget->setContentsMargins(10,10,10,10);

    mainLayout->addWidget(rightWidget,1);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);

    leftScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}
