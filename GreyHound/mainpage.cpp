#include "mainpage.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "candidatecard.h"
#include "vacancycard.h"

MainPage::MainPage(QNetworkAccessManager *manager, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainPage),
      flow_layout(nullptr),
      networkManager(manager),
      currentPage(1) {
    ui->setupUi(this);
}

void MainPage::setStatusOfCandidate(bool status_) {
    isCandidate = status_;
}

FlowLayout *MainPage::getFlowLayout() {
    return flow_layout;
}

// void MainPage::respondToVacancy(int vacancyId) {
//     // 1. Проверяем авторизацию пользователя
//     if (!isCandidate) {
//         QMessageBox::warning(
//             this, "Ошибка", "Только кандидаты могут откликаться на вакансии"
//         );
//         return;
//     }

//     int candidateId = getCurrentCandidateId();  // Нужно реализовать этот
//     метод if (candidateId == -1) {
//         QMessageBox::warning(
//             this, "Ошибка", "Не удалось определить ваш профиль"
//         );
//         return;
//     }

//     QSqlQuery checkQuery;
//     checkQuery.prepare(
//         "SELECT COUNT(*) FROM responces "
//         "WHERE candidate_id = ? AND vacancy_id = ?"
//     );
//     checkQuery.addBindValue(candidateId);
//     checkQuery.addBindValue(vacancyId);

//     if (!checkQuery.exec()) {
//         QMessageBox::critical(
//             this, "Ошибка",
//             "Ошибка проверки откликов: " + checkQuery.lastError().text()
//         );
//         return;
//     }

//     if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
//         QMessageBox::information(
//             this, "Информация", "Вы уже откликались на эту вакансию"
//         );
//         return;
//     }

//     QSqlQuery insertQuery;
//     insertQuery.prepare(
//         "INSERT INTO responces "
//         "(candidate_id, vacancy_id, status, created_at) "
//         "VALUES (?, ?, 'pending', CURRENT_TIMESTAMP)"
//     );
//     insertQuery.addBindValue(candidateId);
//     insertQuery.addBindValue(vacancyId);

//     if (!insertQuery.exec()) {
//         QMessageBox::critical(
//             this, "Ошибка",
//             "Не удалось сохранить отклик: " + insertQuery.lastError().text()
//         );
//         return;
//     }

//     QMessageBox::information(
//         this, "Успех",
//         "Ваш отклик успешно отправлен!\n"
//         "Работодатель получит уведомление."
//     );
// }

void MainPage::show(bool isemployee) {
    if (!isemployee) {
        int page = currentPage;
        if (!pageCache.contains(page)) {
            QWidget *newPage = createCandidatesPage(page);
            ui->vacanciesPage->addWidget(newPage);
            pageCache[page] = newPage;
        }
        ui->vacanciesPage->setCurrentWidget(pageCache[page]);
    } else {
        int page = currentPage;
        if (!pageCache.contains(page)) {
            QWidget *newPage = createVacanciesPage(page);
            ui->vacanciesPage->addWidget(newPage);
            pageCache[page] = newPage;
        }
        ui->vacanciesPage->setCurrentWidget(pageCache[page]);
    }
}

QWidget *MainPage::createCandidatesPage(int numberPage) {
    QWidget *page = new QWidget;
    QWidget *candidateContainer = new QWidget(page);
    FlowLayout *flowLayout = new FlowLayout(candidateContainer);
    QNetworkRequest request;
    QUrl url("http://localhost:80/api/v1/resources/candidateCards");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(currentPage));
    query.addQueryItem("pageSize", "20");
    url.setQuery(query);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray candidates = jsonObj["candidates"].toArray();

        for (const QJsonValue &val : candidates) {
            QJsonObject obj = val.toObject();
            QString place = obj["place"].toString();
            QString faculty = obj["faculty"].toString();
            QString experience = obj["experience"].toString();
            int user_id = obj["user_id"].toInt();
            candidateCard *card = new candidateCard(
                networkManager, place, faculty, experience, user_id
            );
            flowLayout->addWidget(card);
        }
        reply->deleteLater();
    });

    candidateContainer->setLayout(flowLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(candidateContainer);
    page->setLayout(pageLayout);

    return page;
}

QWidget *MainPage::createVacanciesPage(int numberPage) {
    QWidget *page = new QWidget;
    QWidget *vacancyContainer = new QWidget(page);
    FlowLayout *flowLayout = new FlowLayout(vacancyContainer);
    QNetworkRequest request;
    QUrl url("http://localhost:80/api/v1/resources/vacanciesCards");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(currentPage));
    query.addQueryItem("pageSize", "20");
    url.setQuery(query);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(
                this, "Ошибка", "Не удалось загрузить вакансии."
            );
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray vacancies = jsonObj["vacancies"].toArray();

        for (const QJsonValue &val : vacancies) {
            QJsonObject obj = val.toObject();
            QString company = obj["company_name"].toString();
            QString title = obj["name"].toString();
            QString salary = obj["salary"].toString();
            QString location = obj["place"].toString();
            const int scheduleId = obj["work_schedule_status_id"].toInt();
            const int remoteId = obj["remoteness_status_id"].toInt();
            const int vacancyId = obj["id"].toInt();
            VacancyCard *card = new VacancyCard(
                networkManager, title, company, salary, location, scheduleId,
                remoteId, vacancyId, this
            );
            flowLayout->addWidget(card);
        }
        reply->deleteLater();
    });

    vacancyContainer->setLayout(flowLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(vacancyContainer);
    page->setLayout(pageLayout);

    return page;
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

void MainPage::on_pushButtonNext_clicked() {
    ui->pageNumber->setText(QString::number(++currentPage));
    if (!pageCache.contains(currentPage)) {
        QWidget *newPage;
        if (!isCandidate) {
            newPage = createCandidatesPage(currentPage);
        } else {
            newPage = createVacanciesPage(currentPage);
        }
        ui->vacanciesPage->addWidget(newPage);
        pageCache[currentPage] = newPage;
    }
    ui->vacanciesPage->setCurrentWidget(pageCache[currentPage]);
}

void MainPage::on_pushButtonPrev_clicked() {
    if (currentPage > 1) {
        ui->pageNumber->setText(QString::number(--currentPage));
        ui->vacanciesPage->setCurrentWidget(pageCache[currentPage]);
    }
}
