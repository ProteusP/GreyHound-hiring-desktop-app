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

void MainPage::showVacancyDetails(int vacancyId) {
    QDialog *detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle("Детали вакансии");
    detailsDialog->resize(500, 600);

    detailsDialog->setStyleSheet(R"(
        QDialog {
            background-color: #2D2D2D;
            color: #E0E0E0;
        }
        QFrame[frameShape="4"] {
            background-color: #424242;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(detailsDialog);
    QScrollArea *scrollArea = new QScrollArea(detailsDialog);
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);
    scrollContent->setStyleSheet("background-color: #2D2D2D;");

    QSqlQuery query;
    query.prepare(
        "SELECT v.*, e.company_name, exp.name as experience, "
        "ws.name as work_schedule, ed.name as education "
        "FROM vacancies v "
        "JOIN employers e ON v.employer_id = e.id "
        "LEFT JOIN experience exp ON v.experience_status_id = exp.id "
        "LEFT JOIN work_schedule ws ON v.work_schedule_status_id = ws.id "
        "LEFT JOIN educ_statuses ed ON v.educ_status_id = ed.id "
        "WHERE v.id = :vacancyId"
    );
    query.bindValue(":vacancyId", vacancyId);

    if (query.exec() && query.next()) {
        // Стили
        QString sectionStyle =
            "background-color: #383838;"
            "border-radius: 6px;"
            "padding: 12px;"
            "margin-bottom: 10px;";

        QString titleStyle =
            "font-size: 18px;"
            "font-weight: bold;"
            "color: #BB86FC;"
            "margin-bottom: 8px;";

        QString fieldStyle =
            "font-size: 14px;"
            "color: #BB86FC;"
            "margin: 4px 0;";

        QString valueStyle =
            "font-size: 14px;"
            "color: #E0E0E0;"
            "margin: 4px 0 12px 0;";

        QFrame *headerFrame = new QFrame();
        headerFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);

        QLabel *titleLabel = new QLabel(query.value("name").toString());
        titleLabel->setStyleSheet(titleStyle);

        QLabel *companyLabel =
            new QLabel(query.value("company_name").toString());
        companyLabel->setStyleSheet("font-size: 15px; color: #E0E0E0;");

        headerLayout->addWidget(titleLabel);
        headerLayout->addWidget(companyLabel);
        contentLayout->addWidget(headerFrame);

        QFrame *infoFrame = new QFrame();
        infoFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);

        QString salary =
            query.value("salary").isNull()
                ? "не указана"
                : QString("%1 руб.").arg(query.value("salary").toString());

        addFormField(infoLayout, "Зарплата:", salary, fieldStyle, valueStyle);
        addFormField(
            infoLayout,
            "График работы:", query.value("work_schedule").toString(),
            fieldStyle, valueStyle
        );
        addFormField(
            infoLayout, "Опыт:", query.value("experience").toString(),
            fieldStyle, valueStyle
        );

        contentLayout->addWidget(infoFrame);

        // 3. Блок требований
        QFrame *requirementsFrame = new QFrame();
        requirementsFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *reqLayout = new QVBoxLayout(requirementsFrame);

        addFormField(
            reqLayout, "Образование:", query.value("education").toString(),
            fieldStyle, valueStyle
        );
        addFormField(
            reqLayout, "Местоположение:", query.value("place").toString(),
            fieldStyle, valueStyle
        );

        contentLayout->addWidget(requirementsFrame);

        // 4. Блок описания
        QFrame *descFrame = new QFrame();
        descFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *descLayout = new QVBoxLayout(descFrame);

        QLabel *descTitle = new QLabel("Описание вакансии");
        descTitle->setStyleSheet(
            "font-size: 15px; color: #BB86FC; margin-bottom: 8px;"
        );

        QLabel *descContent = new QLabel(query.value("about").toString());
        descContent->setStyleSheet("font-size: 14px; color: #E0E0E0;");
        descContent->setWordWrap(true);

        descLayout->addWidget(descTitle);
        descLayout->addWidget(descContent);
        contentLayout->addWidget(descFrame);

        // Кнопка отклика
        QPushButton *respondButton = new QPushButton("Откликнуться");
        respondButton->setStyleSheet(R"(
            QPushButton {
                background-color: #556B2F;
                color: white;
                border: none;
                padding: 12px 24px;
                font-size: 15px;
                border-radius: 6px;
                min-width: 150px;
                margin: 15px 0;
            }
            QPushButton:hover {
                background-color: #A370D8;
            }
        )");
        connect(respondButton, &QPushButton::clicked, [this, vacancyId]() {
            emit respondToVacancy(vacancyId);
        });

        contentLayout->addWidget(respondButton, 0, Qt::AlignHCenter);
    } else {
        // Обработка ошибки
    }

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
    detailsDialog->exec();
}

void MainPage::respondToVacancy(int vacancyId) {
    // 1. Проверяем авторизацию пользователя
    if (!isCandidate) {
        QMessageBox::warning(
            this, "Ошибка", "Только кандидаты могут откликаться на вакансии"
        );
        return;
    }

    int candidateId = getCurrentCandidateId();  // Нужно реализовать этот метод
    if (candidateId == -1) {
        QMessageBox::warning(
            this, "Ошибка", "Не удалось определить ваш профиль"
        );
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare(
        "SELECT COUNT(*) FROM responces "
        "WHERE candidate_id = ? AND vacancy_id = ?"
    );
    checkQuery.addBindValue(candidateId);
    checkQuery.addBindValue(vacancyId);

    if (!checkQuery.exec()) {
        QMessageBox::critical(
            this, "Ошибка",
            "Ошибка проверки откликов: " + checkQuery.lastError().text()
        );
        return;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::information(
            this, "Информация", "Вы уже откликались на эту вакансию"
        );
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO responces "
        "(candidate_id, vacancy_id, status, created_at) "
        "VALUES (?, ?, 'pending', CURRENT_TIMESTAMP)"
    );
    insertQuery.addBindValue(candidateId);
    insertQuery.addBindValue(vacancyId);

    if (!insertQuery.exec()) {
        QMessageBox::critical(
            this, "Ошибка",
            "Не удалось сохранить отклик: " + insertQuery.lastError().text()
        );
        return;
    }

    QMessageBox::information(
        this, "Успех",
        "Ваш отклик успешно отправлен!\n"
        "Работодатель получит уведомление."
    );
}

int MainPage::getCurrentCandidateId() {
    QSqlQuery query;
    query.prepare("SELECT id FROM candidates WHERE email = ?");
    query.addBindValue(email);  // currentUserId нужно получить из сессии

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

void MainPage::addFormField(
    QLayout *layout,
    const QString &fieldName,
    const QString &fieldValue,
    const QString &fieldStyle,
    const QString &valueStyle
) {
    QLabel *fieldLabel = new QLabel(fieldName);
    fieldLabel->setStyleSheet(fieldStyle);

    QLabel *valueLabel =
        new QLabel(fieldValue.isEmpty() ? "не указано" : fieldValue);
    valueLabel->setStyleSheet(valueStyle);
    valueLabel->setWordWrap(true);

    layout->addWidget(fieldLabel);
    layout->addWidget(valueLabel);
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
        QWidget *newPage = new QWidget();
        ui->vacanciesPage->setCurrentWidget(newPage);
    }
}

QWidget *MainPage::createCandidatesPage(int numberPage) {
    QWidget *page = new QWidget;
    QWidget *vacancyContainer = new QWidget(page);
    FlowLayout *flowLayout = new FlowLayout(vacancyContainer);

    // for (int i = 0; i < 20; ++i) {
    //     QString univer = "НИУ ВШЭ СПб, ПМИ " + QString::number(currentPage);
    //     QString exp = "20 лет";
    //     flowLayout->addWidget(new candidateCard(univer, exp));
    // }
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

void MainPage::setEmail(QString email_) {
    email = email_;
}

void MainPage::on_profilePB_3_clicked() {
    emit onProfilePressed();
}

void MainPage::on_pushButtonNext_clicked() {
    ui->pageNumber->setText(QString::number(++currentPage));
    if (!pageCache.contains(currentPage)) {
        QWidget *newPage = createCandidatesPage(currentPage);
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
