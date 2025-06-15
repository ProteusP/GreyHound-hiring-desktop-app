#include "vacancydetailswindow.h"
#include "ui_vacancydetailswindow.h"
#include <QDebug>
#include <QMessageBox>

vacancyDetailsWindow::vacancyDetailsWindow(
    QNetworkAccessManager *manager_, const QString &title_,
    const QString &company_, const QString &salary_, const QString &place_,
    const int &workSchedule_, const int &remoteness_, const int &vacancy_id_,
    QWidget *parent)
    : QDialog(parent), ui(new Ui::vacancyDetailsWindow),
      networkManager(manager_), vacancy_id(vacancy_id_) {
    ui->setupUi(this);

    ui->title->setText(title_);
    ui->company->setText(company_);
    ui->salary->setText(salary_);
    ui->place->setText(place_);

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

    this->setStyleSheet("QLabel { color: black; }");
}

vacancyDetailsWindow::~vacancyDetailsWindow() { delete ui; }

void vacancyDetailsWindow::on_exitButton_clicked() { this->close(); }

void vacancyDetailsWindow::on_pushButton_clicked() {
    QUrl url("http://localhost:80/api/v1/Notifications/candRespond");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    payload["vacancy_id"] = vacancy_id;

    QNetworkReply *reply =
        networkManager->post(request, QJsonDocument(payload).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();

        QMessageBox msgBox;
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #fefefe; font-size: 14px; }"
            "QLabel { color: #2c3e50; }"
            "QPushButton { background-color: #3498db; color: white; "
            "border-radius: 5px; padding: 6px 12px; }"
            "QPushButton:hover { background-color: #2980b9; }");

        // Читаем тело ответа всегда
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject obj = jsonDoc.object();

        if (reply->error() == QNetworkReply::NoError) {
            // Всё прошло успешно
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle("Успех");
            msgBox.setText("Отклик успешно отправлен!");
        } else {
            // Ошибка — но проверим, может сервер дал пояснение
            QString serverError = obj["error"].toString();
            qDebug() << "Ошибка с сервера:" << serverError;

            if (serverError == "Response already exists") {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle("Уже откликались");
                msgBox.setText("Вы уже откликались на эту вакансию ранее.");
            } else if (!serverError.isEmpty()) {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle("Ошибка");
                msgBox.setText("Ошибка: " + serverError);
            } else {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle("Ошибка");
                msgBox.setText(
                    "Не удалось отправить отклик. Повторите попытку позже.");
            }
        }

        msgBox.exec();
    });
}
