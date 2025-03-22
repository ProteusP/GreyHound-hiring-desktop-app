#include "mainpage.h"

MainPage::MainPage(QWidget *parent) : QWidget(parent), ui(new Ui::MainPage) {
    ui->setupUi(this);
}

void MainPage::setStatusOfCandidate(bool status_) {
    isCandidate = status_;
}

void MainPage::show() {
    FlowLayout *flowLayout = new FlowLayout;
    if (!isCandidate) {
        QSqlQuery query;
        if (!query.exec("SELECT * FROM `candidates`")) {
            qDebug() << "Ошибка при выполнении запроса:" << query.lastError().text();
            return;
        }
        while (query.next()) {
            QVariant search_status = query.value(13);
            bool is_active_search = search_status.toBool();
            if (is_active_search) {
                QString name = query.value(1).toString();
                QString surname = query.value(2).toString();
                QString phone_number = query.value(5).toString();
                flowLayout->addWidget(new VacancyCard(
                    name, surname, phone_number));
            }
        }
    }
    else {
        // Print vacancies
    }
    // fix it later (gle6a6y)
    QLayout* oldLayout = ui->groupBoxVacancies_3->layout();
    if (oldLayout) {
        delete oldLayout;
    }
    ui->groupBoxVacancies_3->setLayout(flowLayout);
}

MainPage::~MainPage() {
    delete ui;
}

void MainPage::on_profilePB_3_clicked() {
    emit onProfilePressed();
}
