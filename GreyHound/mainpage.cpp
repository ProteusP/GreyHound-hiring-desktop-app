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

void MainPage::createEmplFilters() {
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);

    QWidget *container = new QWidget(this);
    container->setMaximumWidth(320);
    QVBoxLayout *mainVBox = new QVBoxLayout(container);
    ui->verticalLayout_3->addWidget(container);

    QLabel *label = new QLabel("Фильтры", this);
    label->setStyleSheet("font-weight: bold; font-size: 18px; color: black; margin-bottom: 8px;");
    mainVBox->addWidget(label);

            // Названия полей
    QMap<QString, QString> textFields = {
        { "place_of_study", "Место обучения" },
        { "faculty_of_educ", "Факультет" },
        { "place", "Местоположение" }
    };

    QMap<QString, QString> comboFields = {
        { "graduation_year", "Год выпуска" },
        { "search_status_id", "Статус поиска" },
        { "educ_status_id", "Статус обучения" },
        { "experience_status_id", "Опыт" },
        { "work_schedule_status_id", "График" }
    };

    filterInputs.clear();  // Очистим на случай повторного вызова

            // Текстовые поля
    for (const auto &key : textFields.keys()) {
        QHBoxLayout *h = new QHBoxLayout();
        QLabel *lbl = new QLabel(textFields[key], this);
        lbl->setStyleSheet("color: black; font-size: 13px;");
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QLineEdit *line = new QLineEdit(this);
        line->setFixedWidth(160);
        line->setAlignment(Qt::AlignLeft);
        line->setStyleSheet(
            "QLineEdit {"
            " background-color: white;"
            " color: black;"
            " border: 1px solid #ccc;"
            " border-radius: 6px;"
            " padding: 4px 8px;"
            " font-size: 13px;"
            " }"
            );
        h->addWidget(lbl, 1, Qt::AlignLeft);
        h->addWidget(line, 0, Qt::AlignRight);
        mainVBox->addLayout(h);
        filterInputs[key] = line;
    }

            // Комбобоксы
    for (const auto &key : comboFields.keys()) {
        QHBoxLayout *h = new QHBoxLayout();
        QLabel *lbl = new QLabel(comboFields[key], this);
        lbl->setStyleSheet("color: black; font-size: 13px;");
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QComboBox *box = new QComboBox(this);
        box->setFixedWidth(160);
        box->setStyleSheet(
            "QComboBox {"
            " background-color: white;"
            " color: black;"
            " border: 1px solid #ccc;"
            " border-radius: 6px;"
            " padding: 4px 8px;"
            " font-size: 13px;"
            " }"
            "QComboBox::drop-down { border: none; }"
            "QComboBox QAbstractItemView {"
            " background-color: white;"
            " color: black;"
            " selection-background-color: #e0e0e0;"
            " }"
            );
        box->addItem("Без фильтров", QVariant());

        if (key == "graduation_year") {
            for (int year = 1975; year <= 2032; ++year)
                box->addItem(QString::number(year), year);
        } else if (key == "educ_status_id") {
            box->addItem("Среднее", 1);
            box->addItem("Среднее специальное", 2);
            box->addItem("Бакалавриат", 3);
            box->addItem("Магистратура", 4);
            box->addItem("Аспирантура", 5);
            box->addItem("Не имеет значения", 6);
        } else if (key == "experience_status_id") {
            box->addItem("Без опыта", 1);
            box->addItem("1–3 лет", 2);
            box->addItem("3–5 лет", 3);
            box->addItem("5+ лет", 4);
        } else if (key == "search_status_id") {
            box->addItem("Активно ищу", 1);
            box->addItem("Рассматриваю предложения", 2);
            box->addItem("Не ищу", 3);
        } else if (key == "work_schedule_status_id") {
            box->addItem("Полный рабочий день", 1);
            box->addItem("Частичная занятость", 2);
            box->addItem("Гибкий график", 3);
            box->addItem("Другое", 4);
        }

        h->addWidget(lbl, 1, Qt::AlignLeft);
        h->addWidget(box, 0, Qt::AlignRight);
        mainVBox->addLayout(h);
        filterInputs[key] = box;
    }

    mainVBox->addStretch();

            // Кнопка "Применить"
    QPushButton *applyButton = new QPushButton("Применить", this);
    applyButton->setFixedWidth(160);
    applyButton->setStyleSheet(
        "QPushButton {"
        " background-color: #3498db;"
        " color: white;"
        " border: none;"
        " padding: 6px;"
        " font-size: 14px;"
        " border-radius: 4px;"
        " }"
        "QPushButton:hover {"
        " background-color: #2980b9;"
        " }"
        );
    mainVBox->addWidget(applyButton, 0, Qt::AlignHCenter);

    connect(applyButton, &QPushButton::clicked, this, [=]() {
        QMap<QString, QVariant> filters;

        for (const auto &key : filterInputs.keys()) {
            if (auto line = qobject_cast<QLineEdit *>(filterInputs[key])) {
                QString val = line->text().trimmed();
                filters[key] = val;
            } else if (auto box = qobject_cast<QComboBox *>(filterInputs[key])) {
                QVariant val = box->currentData();
                filters[key] = val.isValid() ? val : "Без фильтров";
            }
        }

        qDebug() << "Формируем фильтры для запроса:";
        for (auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
            qDebug() << it.key() << "=>" << it.value().toString();
        }

        QWidget *filteredPage = createCandidatesPageWithFilters(filters);

        if (pageCache.contains(currentPage)) {
            QWidget *oldPage = pageCache[currentPage];
            ui->vacanciesPage->removeWidget(oldPage);
            oldPage->deleteLater();
        }

        ui->vacanciesPage->addWidget(filteredPage);
        pageCache[currentPage] = filteredPage;
        ui->vacanciesPage->setCurrentWidget(filteredPage);
    });
}


void MainPage::createCandFilters() {
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    QWidget *container = new QWidget(this);
    container->setMaximumWidth(340);
    QVBoxLayout *mainVBox = new QVBoxLayout(container);
    ui->verticalLayout_3->addWidget(container);

    QLabel *label = new QLabel("Фильтры", this);
    label->setStyleSheet("font-weight: bold; font-size: 18px; color: black; margin-bottom: 8px;");
    mainVBox->addWidget(label);

            // Карта текстовых полей
    QMap<QString, QString> textFields = {
        { "salary", "Зарплата (от:)" },
        { "place", "Местоположение" },
        { "educ_place", "Место обучения" }
    };

            // Карта комбобоксов
    QMap<QString, QString> comboFields = {
        { "experience_status_id", "Опыт" },
        { "work_schedule_status_id", "График" },
        { "educ_status_id", "Статус обучения" },
        { "remoteness_status_id", "Удалёнка" }
    };

    candFilterInputs.clear();

            // Текстовые поля
    for (const auto &key : textFields.keys()) {
        QHBoxLayout *h = new QHBoxLayout();
        QLabel *lbl = new QLabel(textFields[key], this);
        lbl->setStyleSheet("color: black; font-size: 13px;");
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QLineEdit *line = new QLineEdit(this);
        line->setFixedWidth(160);
        line->setAlignment(Qt::AlignLeft);
        line->setStyleSheet(
            "QLineEdit {"
            " background-color: white;"
            " color: black;"
            " border: 1px solid #ccc;"
            " border-radius: 6px;"
            " padding: 4px 8px;"
            " font-size: 13px;"
            " }"
            );
        h->addWidget(lbl, 1, Qt::AlignLeft);
        h->addWidget(line, 0, Qt::AlignRight);
        mainVBox->addLayout(h);
        candFilterInputs[key] = line;
    }

            // Комбобоксы
    for (const auto &key : comboFields.keys()) {
        QHBoxLayout *h = new QHBoxLayout();
        QLabel *lbl = new QLabel(comboFields[key], this);
        lbl->setStyleSheet("color: black; font-size: 13px;");
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QComboBox *box = new QComboBox(this);
        box->setFixedWidth(160);
        box->setStyleSheet(
            "QComboBox {"
            " background-color: white;"
            " color: black;"
            " border: 1px solid #ccc;"
            " border-radius: 6px;"
            " padding: 4px 8px;"
            " font-size: 13px;"
            " }"
            "QComboBox::drop-down { border: none; }"
            "QComboBox QAbstractItemView {"
            " background-color: white;"
            " color: black;"
            " selection-background-color: #e0e0e0;"
            " }"
            );
        box->addItem("Без фильтров", QVariant());

        if (key == "experience_status_id") {
            box->addItem("Без опыта", 1);
            box->addItem("1–3 лет", 2);
            box->addItem("3–5 лет", 3);
            box->addItem("5+ лет", 4);
        } else if (key == "work_schedule_status_id") {
            box->addItem("Полный рабочий день", 1);
            box->addItem("Частичная занятость", 2);
            box->addItem("Гибкий график", 3);
            box->addItem("Другое", 4);
        } else if (key == "educ_status_id") {
            box->addItem("Среднее", 1);
            box->addItem("Среднее специальное", 2);
            box->addItem("Бакалавриат", 3);
            box->addItem("Магистратура", 4);
            box->addItem("Аспирантура", 5);
            box->addItem("Не имеет значения", 6);
        } else if (key == "remoteness_status_id") {
            box->addItem("Да", 1);
            box->addItem("Нет", 0);
        }

        h->addWidget(lbl, 1, Qt::AlignLeft);
        h->addWidget(box, 0, Qt::AlignRight);
        mainVBox->addLayout(h);
        candFilterInputs[key] = box;
    }

    mainVBox->addStretch();

    QPushButton *applyButton = new QPushButton("Применить", this);
    applyButton->setFixedWidth(160);
    applyButton->setStyleSheet(
        "QPushButton {"
        " background-color: #3498db;"
        " color: white;"
        " border: none;"
        " padding: 6px;"
        " font-size: 14px;"
        " border-radius: 4px;"
        " }"
        "QPushButton:hover { background-color: #2980b9; }"
        );
    mainVBox->addWidget(applyButton, 0, Qt::AlignHCenter);

    connect(applyButton, &QPushButton::clicked, this, [=]() {
        QMap<QString, QVariant> filters;

        for (const auto &key : candFilterInputs.keys()) {
            if (auto line = qobject_cast<QLineEdit *>(candFilterInputs[key])) {
                QString val = line->text().trimmed();
                filters[key] = val;
            } else if (auto box = qobject_cast<QComboBox *>(candFilterInputs[key])) {
                QVariant val = box->currentData();
                filters[key] = val.isValid() ? val : "Без фильтров";
            }
        }

        qDebug() << "Фильтры кандидата:";
        for (auto it = filters.begin(); it != filters.end(); ++it) {
            qDebug() << it.key() << "=>" << it.value().toString();
        }

        QWidget *filteredPage = createVacanciesPageWithFilters(filters);

        if (pageCache.contains(currentPage)) {
            QWidget *oldPage = pageCache[currentPage];
            ui->vacanciesPage->removeWidget(oldPage);
            oldPage->deleteLater();
        }

        ui->vacanciesPage->addWidget(filteredPage);
        pageCache[currentPage] = filteredPage;
        ui->vacanciesPage->setCurrentWidget(filteredPage);
    });
}

// ggwp@mail.ru
// Ggwp1122!
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

QWidget *MainPage::createCandidatesPageWithFilters(const QMap<QString, QVariant> &filters) {
    QWidget *page = new QWidget;
    QWidget *candidateContainer = new QWidget(page);
    QVBoxLayout *candidateLayout = new QVBoxLayout(candidateContainer);

            // 1. Показываем индикатор загрузки
    QLabel *loadingLabel = new QLabel("Загрузка...", candidateContainer);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet("font-size: 16px; color: gray; padding: 20px;");
    candidateLayout->addWidget(loadingLabel);

    candidateContainer->setLayout(candidateLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(candidateContainer);
    page->setLayout(pageLayout);

            // 2. Формируем URL
    QUrl url("http://localhost:80/api/v1/resources/candidateCards");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(currentPage));
    query.addQueryItem("pageSize", "20");

    for (auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
        const QString &key = it.key();
        const QVariant &val = it.value();
        if (!val.toString().isEmpty() && val.toString() != "Без фильтров") {
            query.addQueryItem(key, val.toString());
        }
    }

    url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить данные.");
            reply->deleteLater();
            return;
        }

                // Удаляем индикатор загрузки
        QLayout *oldLayout = candidateContainer->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

                // Создаём layout и добавляем карточки
        FlowLayout *flowLayout = new FlowLayout(candidateContainer);
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

        candidateContainer->setLayout(flowLayout);

        reply->deleteLater();
    });

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


QWidget *MainPage::createVacanciesPageWithFilters(const QMap<QString, QVariant> &filters) {
    QWidget *page = new QWidget;
    QWidget *vacancyContainer = new QWidget(page);
    QVBoxLayout *vacancyLayout = new QVBoxLayout(vacancyContainer);

    QLabel *loadingLabel = new QLabel("Загрузка...", vacancyContainer);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet("font-size: 16px; color: gray; padding: 20px;");
    vacancyLayout->addWidget(loadingLabel);
    vacancyContainer->setLayout(vacancyLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(vacancyContainer);
    page->setLayout(pageLayout);

    QUrl url("http://localhost:80/api/v1/resources/vacanciesCards");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(currentPage));
    query.addQueryItem("pageSize", "20");

    for (auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
        const QString &key = it.key();
        const QVariant &val = it.value();
        if (!val.toString().isEmpty() && val.toString() != "Без фильтров") {
            query.addQueryItem(key, val.toString());
        }
    }

    url.setQuery(query);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить данные.");
            reply->deleteLater();
            return;
        }

        QLayout *oldLayout = vacancyContainer->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

        FlowLayout *flowLayout = new FlowLayout(vacancyContainer);
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

        vacancyContainer->setLayout(flowLayout);
        reply->deleteLater();
    });

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
