#include "candidatecard.h"
#include "candidatedetailswindow.h"

candidateCard::candidateCard(
    QNetworkAccessManager *manager,
    const QString &placeOfStudy_,
    const QString &facultyOfEducation_,
    const QString &experience_,
    const int &id_,
    QWidget *parent
)
    : QFrame(parent), user_id(id_), networkManager(manager) {
    setFixedSize(170, 150);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setStyleSheet(
        "candidateCard {"
        "background: white;"
        "border-radius: 8px;"
        "padding: 10px;"
        "border: 1px solid #ddd;"
        "}"
    );

    placeOfStudy = new QLabel(placeOfStudy_);
    placeOfStudy->setStyleSheet(
        "color: #555; font-weight: bold; font-size: 14px;"
    );

    facultyOfEducation = new QLabel(facultyOfEducation_);
    facultyOfEducation->setStyleSheet("color: #555; font-size: 12px;");

    experience = new QLabel(experience_);
    experience->setStyleSheet("color: #555; font-size: 12px;");
    detailsButton = new QPushButton("Подробнее");
    detailsButton->setStyleSheet(
        "QPushButton {"
        "background: #4285f4;"
        "color: white;"
        "border: none;"
        "padding: 5px 10px;"
        "border-radius: 4px;"
        "font-size: 12px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(placeOfStudy);
    layout->addWidget(facultyOfEducation);
    layout->addWidget(experience);
    layout->addStretch();
    layout->addWidget(detailsButton, 0, Qt::AlignRight);

    connect(
        detailsButton, &QPushButton::clicked, this,
        &candidateCard::on_detailsButton_clicked
    );
}

void candidateCard::setFixedSize(int width, int height) {
    QFrame::setFixedSize(width, height);
}

void candidateCard::on_detailsButton_clicked() {
    auto *detailsWindow = new candidateDetailsWindow(networkManager, user_id);
    detailsWindow->setWindowTitle("Информация о кандидате");
    detailsWindow->exec();
}
