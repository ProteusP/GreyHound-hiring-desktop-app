#include "vacancycard.h"

VacancyCard::VacancyCard(const QString &title, const QString &company, const QString &description, QWidget *parent)
    : QFrame(parent) {
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setStyleSheet("VacancyCard {"
        "background: white;"
        "border-radius: 8px;"
        "padding: 10px;"
        "border: 1px solid #ddd;"
        "}");

    titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #555; font-weight: bold; font-size: 14px;");

    companyLabel = new QLabel(company);
    companyLabel->setStyleSheet("color: #555; font-size: 12px;");

    descLabel = new QLabel(description);
    descLabel->setStyleSheet("font-size: 12px;");
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(R"(
    font-size: 12px;
    color: #333;
    margin-top: 5px;
    margin-bottom: 5px;
)");
    descLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    detailsButton = new QPushButton("Подробнее");
    detailsButton->setStyleSheet("QPushButton {"
        "background: #4285f4;"
        "color: white;"
        "border: none;"
        "padding: 5px 10px;"
        "border-radius: 4px;"
        "font-size: 12px;"
        "}");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(companyLabel);
    layout->addWidget(descLabel);
    layout->addStretch();
    layout->addWidget(detailsButton, 0, Qt::AlignRight);

    connect(detailsButton, &QPushButton::clicked, [this]() {
        emit detailsRequested(this->vacancyId);
    });
}

void VacancyCard::setFixedSize(int width, int height) {
    QFrame::setFixedSize(width, height);
}


void VacancyCard::setVacancyId(int id) {
    vacancyId = id;
}
