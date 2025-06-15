#include "vacancydetailswindow.h"
#include <QDebug>
#include "ui_vacancydetailswindow.h"

vacancyDetailsWindow::vacancyDetailsWindow(
    QNetworkAccessManager *manager_,
    const QString &title_,
    const QString &company_,
    const QString &salary_,
    const QString &place_,
    const int &workSchedule_,
    const int &remoteness_,
    const int &vacancy_id_,
    QWidget *parent
)
    : QDialog(parent),
      ui(new Ui::vacancyDetailsWindow),
      networkManager(manager_),
      vacancy_id(vacancy_id_) {
    ui->setupUi(this);
    qDebug() << "tut1\n";
    ui->title->setText(title_);
    ui->company->setText(company_);
    ui->salary->setText(salary_);
    ui->place->setText(place_);
    qDebug() << "tut2\n";
    QString schedule;
    switch (workSchedule_) {
        case 1:
            schedule = "Полный рабочий день";
            break;
        case 2:
            schedule = "Частичная занятость";
            break;
        case 3:
            schedule = "Гибкий график";
            break;
        case 4:
            schedule = "Другое";
            break;
        default:
            schedule = "...";
            break;
    }
    ui->workSchedule->setText(schedule);
    QString rmtnss;
    switch (remoteness_) {
        case 0:
            rmtnss = "Нет";
            break;
        case 1:
            rmtnss = "Да";
            break;
        default:
            rmtnss = "...";
            break;
    }
    ui->remoteness->setText(rmtnss);
}

vacancyDetailsWindow::~vacancyDetailsWindow() {
    delete ui;
}

void vacancyDetailsWindow::on_exitButton_clicked() {
    this->close();
}
