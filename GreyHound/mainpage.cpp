#include "mainpage.h"

MainPage::MainPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainPage), flow_layout(nullptr) {
    ui->setupUi(this);
}

void MainPage::setStatusOfCandidate(bool status_) {
    isCandidate = status_;
}

FlowLayout *MainPage::getFlowLayout() {
    return flow_layout;
}

void MainPage::show() {
    flow_layout = new FlowLayout();
    QSqlQuery query;
    if (!isCandidate) {
        if (!query.exec("SELECT * FROM `candidates`")) {
            qDebug() << "Ошибка при выполнении запроса:"
                     << query.lastError().text();
            delete flow_layout;
            return;
        }
        while (query.next()) {
            int search_status = query.value(13).toInt();
            if (search_status != 3) {
                QString name = query.value(1).toString();
                QString surname = query.value(2).toString();
                QString phone_number = query.value(5).toString();
                flow_layout->addWidget(
                    new VacancyCard(name, surname, phone_number)
                );
            }
        }
    } else {
        if (!query.exec("SELECT * FROM `vacancies`")) {
            qDebug() << "Ошибка при выполнении запроса:"
                     << query.lastError().text();
            delete flow_layout;
            return;
        }
        while (query.next()) {
            int search_status = query.value(3).toInt();
            if (search_status == 1) {
                QSqlQuery query_for_company_name;
                query_for_company_name.prepare(
                    "SELECT employers.company_name "
                    "FROM vacancies "
                    "JOIN employers ON vacancies.employer_id = employers.id "
                    "WHERE vacancies.id = :vacancyId"
                );
                query_for_company_name.bindValue(
                    ":vacancyId", query.value(0).toInt()
                );
                if (query_for_company_name.exec()) {
                    if (query_for_company_name.next()) {
                        QString name = query.value(1).toString();
                        QString company_name =
                            query_for_company_name.value(0).toString();
                        QString salary = query.value(2).toString();
                        flow_layout->addWidget(
                            new VacancyCard(name, company_name, salary)
                        );
                    } else {
                        qDebug() << "Вакансия с такой компанией не найдена!";
                    }
                } else {
                    qDebug()
                        << "Ошибка выполнения запроса на название компании:"
                        << query.lastError().text();
                }
            }
        }
    }
    // fix it later (gle6a6y)
    QLayout *oldLayout = ui->groupBoxVacancies_3->layout();
    if (oldLayout) {
        delete oldLayout;
    }
    ui->groupBoxVacancies_3->setLayout(flow_layout);
}

void MainPage::hide() {
    QLayoutItem *item;
    while ((item = flow_layout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();
        if (widget != nullptr) {
            widget->setParent(nullptr);
            delete widget;
        }
        delete item;
    }
}

MainPage::~MainPage() {
    delete ui;
}

void MainPage::on_profilePB_3_clicked() {
    emit onProfilePressed();
}
