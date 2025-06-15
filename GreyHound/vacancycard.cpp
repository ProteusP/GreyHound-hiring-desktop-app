#include "vacancycard.h"
#include "vacancydetailswindow.h"

VacancyCard::VacancyCard(QNetworkAccessManager *manager_, const QString &title_,
                         const QString &company_, const QString &salary_,
                         const QString &place_, const int &workSchedule_,
                         const int &remoteness_, const int &vacancy_id_,
                         QWidget *parent)
    : QFrame(parent), vacancy_id(vacancy_id_), networkManager(manager_),
      titleString(title_), companyString(company_), salaryString(salary_),
      placeString(place_), workSchedule(workSchedule_),
      remoteness(remoteness_) {
    setFixedSize(140, 140);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setStyleSheet("VacancyCard {"
                        "background: white;"
                        "border-radius: 8px;"
                        "padding: 10px;"
                        "border: 1px solid #ddd;"
                        "}");

    title = new QLabel(title_);
    title->setStyleSheet("color: #555; font-weight: bold; font-size: 14px;");

    company = new QLabel(company_);
    company->setStyleSheet("color: #555; font-size: 12px;");

    salary = new QLabel(salary_);
    salary->setStyleSheet("font-size: 12px;");
    salary->setWordWrap(true);
    salary->setStyleSheet(R"(
            font-size: 12px;
            color: #333;
            margin-top: 5px;
            margin-bottom: 5px;
            )");
    salary->setAlignment(Qt::AlignLeft | Qt::AlignTop);

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
    layout->addWidget(title);
    layout->addWidget(company);
    layout->addWidget(salary);
    layout->addStretch();
    layout->addWidget(detailsButton, 0, Qt::AlignRight);

    connect(detailsButton, &QPushButton::clicked, this,
            &VacancyCard::on_detailsButton_clicked);
}

void VacancyCard::setFixedSize(int width, int height) {
    QFrame::setFixedSize(width, height);
}

void VacancyCard::on_detailsButton_clicked() {
    auto *detailsWindow = new vacancyDetailsWindow(
        networkManager, titleString, companyString, salaryString, placeString,
        workSchedule, remoteness, vacancy_id);
    detailsWindow->setWindowTitle("Информация о вакансии");
    detailsWindow->show();
}
