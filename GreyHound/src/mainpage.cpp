#include "mainpage.h"
#include "candidatecard.h"
#include "vacancycard.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MainPage::MainPage(QNetworkAccessManager *manager, QWidget *parent)
    : QWidget(parent), ui(new Ui::MainPage), flow_layout(nullptr),
      networkManager(manager), currentPage(1) {
    ui->setupUi(this);
}

void MainPage::createEmplFilters() {
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    filterInputs.clear();

    QWidget *card = new QWidget(this);
    card->setStyleSheet(
        "background-color: white;"
        "border-radius: 16px;"
        "padding: 16px;"
        "border: 1px solid #eee;"
        );
    card->setMaximumWidth(260);
    card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QVBoxLayout *mainVBox = new QVBoxLayout(card);
    mainVBox->setSpacing(10);
    mainVBox->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout_3->addWidget(card);

    QLabel *title = new QLabel("Фильтры", this);
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet(
        "font-weight: 600;"
        "font-size: 20px;"
        "margin-bottom: 10px;"
        "background: transparent;"
        "border: none;"
        );
    mainVBox->addWidget(title);

    QWidget *filterFields = new QWidget(this);
    QVBoxLayout *fieldsLayout = new QVBoxLayout(filterFields);
    fieldsLayout->setSpacing(10);
    fieldsLayout->setContentsMargins(0, 0, 0, 0);

    QMap<QString, QString> textFields = {
        {"place_of_study", "Место обучения"},
        {"faculty_of_educ", "Факультет"},
        {"place", "Местоположение"}
    };

    QMap<QString, QString> comboFields = {
        {"graduation_year", "Год выпуска"},
        {"search_status_id", "Статус поиска"},
        {"educ_status_id", "Статус обучения"},
        {"experience_status_id", "Опыт"},
        {"work_schedule_status_id", "График"}
    };

    QString lineEditStyle = R"(
        QLineEdit {
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 10px;
            padding: 6px 10px;
            font-size: 13px;
        })";

    QString comboBoxStyle = R"(
        QComboBox {
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 6px 12px;
            background-color: #f9f9f9;
            font-size: 14px;
            color: black;
        }
        QComboBox QAbstractItemView {
            background-color: white;
            color: black;
            selection-background-color: white;
            selection-color: black;
            border: 1px solid #ccc;
            font-size: 14px;
        }
        QComboBox::down-arrow {
            image: url(:/caret-down.png);
            width: 12px;
            height: 12px;
            background-color: white;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left: 1px solid #ccc;
            background-color: white;
        })";

    for (const auto &key : textFields.keys()) {
        QLineEdit *line = new QLineEdit(this);
        line->setPlaceholderText(textFields[key]);
        line->setStyleSheet(lineEditStyle);
        line->setMinimumHeight(32);
        fieldsLayout->addWidget(line);
        filterInputs[key] = line;
    }

    for (const auto &key : comboFields.keys()) {
        QComboBox *box = new QComboBox(this);
        box->setStyleSheet(comboBoxStyle);
        box->setMinimumHeight(32);
        box->addItem(comboFields[key], QVariant());

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

        fieldsLayout->addWidget(box);
        filterInputs[key] = box;
    }

    mainVBox->addWidget(filterFields);
    mainVBox->addStretch();

    QPushButton *applyButton = new QPushButton("Применить", this);
    applyButton->setMinimumHeight(36);
    applyButton->setStyleSheet(
        "QPushButton {"
        " background-color: #347EFF;"
        " color: white;"
        " font-size: 14px;"
        " font-weight: 500;"
        " border: none;"
        " border-radius: 10px;"
        " }"
        "QPushButton:hover { background-color: #2c6fe0; }");
    mainVBox->addWidget(applyButton);

    connect(applyButton, &QPushButton::clicked, this, [=]() {
        QMap<QString, QVariant> filters;

        for (const auto &key : filterInputs.keys()) {
            if (auto line = qobject_cast<QLineEdit *>(filterInputs[key])) {
                filters[key] = line->text().trimmed();
            } else if (auto box = qobject_cast<QComboBox *>(filterInputs[key])) {
                QVariant val = box->currentData();
                filters[key] = val.isValid() ? val : "Без фильтров";
            }
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
    candFilterInputs.clear();

    QWidget *card = new QWidget(this);
    card->setStyleSheet(
        "background-color: white;"
        "border-radius: 16px;"
        "padding: 16px;"
        "border: 1px solid #eee;"
        );
    card->setMaximumWidth(260);
    card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QVBoxLayout *mainVBox = new QVBoxLayout(card);
    mainVBox->setSpacing(10);
    mainVBox->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout_3->addWidget(card);

    QLabel *title = new QLabel("Фильтры", this);
    title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet(
        "font-weight: 600;"
        "font-size: 20px;"
        "margin-bottom: 10px;"
        "background: transparent;"
        "border: none;"
        );
    mainVBox->addWidget(title);

    QWidget *filterFields = new QWidget(this);
    QVBoxLayout *fieldsLayout = new QVBoxLayout(filterFields);
    fieldsLayout->setSpacing(10);
    fieldsLayout->setContentsMargins(0, 0, 0, 0);

    QMap<QString, QString> textFields = {
        {"educ_place", "Место обучения"},
        {"place", "Местоположение"},
        {"salary", "Зарплата (от)"}
    };

    QMap<QString, QString> comboFields = {
        {"educ_status_id", "Статус обучения"},
        {"experience_status_id", "Опыт"},
        {"remoteness_status_id", "Удалёнка"},
        {"work_schedule_status_id", "График"}
    };

    QString lineEditStyle = R"(
        QLineEdit {
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 10px;
            padding: 6px 10px;
            font-size: 13px;
        })";

    QString comboBoxStyle = R"(
        QComboBox {
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 6px 12px;
            background-color: #f9f9f9;
            font-size: 14px;
            color: black;
        }
        QComboBox QAbstractItemView {
            background-color: white;
            color: black;
            selection-background-color: white;
            selection-color: black;
            border: 1px solid #ccc;
            font-size: 14px;
        }
        QComboBox::down-arrow {
            image: url(:/caret-down.png);
            width: 12px;
            height: 12px;
            background-color: white;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left: 1px solid #ccc;
            background-color: white;
        })";

    for (const auto &key : textFields.keys()) {
        QLineEdit *line = new QLineEdit(this);
        line->setPlaceholderText(textFields[key]);
        line->setStyleSheet(lineEditStyle);
        line->setMinimumHeight(32);
        fieldsLayout->addWidget(line);
        candFilterInputs[key] = line;
    }

    for (const auto &key : comboFields.keys()) {
        QComboBox *box = new QComboBox(this);
        box->setStyleSheet(comboBoxStyle);
        box->setMinimumHeight(32);
        box->addItem(comboFields[key], QVariant());

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

        fieldsLayout->addWidget(box);
        candFilterInputs[key] = box;
    }

    mainVBox->addWidget(filterFields);
    mainVBox->addStretch();

    QPushButton *applyButton = new QPushButton("Применить", this);
    applyButton->setMinimumHeight(36);
    applyButton->setStyleSheet(
        "QPushButton {"
        " background-color: #347EFF;"
        " color: white;"
        " font-size: 14px;"
        " font-weight: 500;"
        " border: none;"
        " border-radius: 10px;"
        " }"
        "QPushButton:hover { background-color: #2c6fe0; }");
    mainVBox->addWidget(applyButton);

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
                networkManager, place, faculty, experience, user_id);
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

QWidget *MainPage::createCandidatesPageWithFilters(
    const QMap<QString, QVariant> &filters) {
    QWidget *page = new QWidget;
    QWidget *candidateContainer = new QWidget(page);
    QVBoxLayout *candidateLayout = new QVBoxLayout(candidateContainer);

    QLabel *loadingLabel = new QLabel("Загрузка...", candidateContainer);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet("font-size: 16px; color: gray; padding: 20px;");
    candidateLayout->addWidget(loadingLabel);

    candidateContainer->setLayout(candidateLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(candidateContainer);
    page->setLayout(pageLayout);

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

        QLayout *oldLayout = candidateContainer->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

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
                networkManager, place, faculty, experience, user_id);
            flowLayout->addWidget(card);
        }

        candidateContainer->setLayout(flowLayout);

        reply->deleteLater();
    });

    return page;
}

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
            QMessageBox::warning(this, "Ошибка",
                                 "Не удалось загрузить вакансии.");
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
            VacancyCard *card = new VacancyCard(networkManager, title, company,
                                                salary, location, scheduleId,
                                                remoteId, vacancyId, this);
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

QWidget *MainPage::createVacanciesPageWithFilters(
    const QMap<QString, QVariant> &filters) {
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
            VacancyCard *card = new VacancyCard(networkManager, title, company,
                                                salary, location, scheduleId,
                                                remoteId, vacancyId, this);
            flowLayout->addWidget(card);
        }

        vacancyContainer->setLayout(flowLayout);
        reply->deleteLater();
    });

    return page;
}

void MainPage::setStatusOfCandidate(bool status_) { isCandidate = status_; }

FlowLayout *MainPage::getFlowLayout() { return flow_layout; }

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

MainPage::~MainPage() { delete ui; }

void MainPage::on_profilePB_3_clicked() { emit onProfilePressed(); }

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
