#include "mainpage.h"
#include "vacancycard.h"

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

void MainPage::showVacancyDetails(int vacancyId) {
    QDialog *detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle("Детали вакансии");
    detailsDialog->resize(500, 600);

    // Основные стили для темной темы
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

            // Запрос данных
    QSqlQuery query;
    query.prepare("SELECT v.*, e.company_name, exp.name as experience, "
        "ws.name as work_schedule, ed.name as education "
        "FROM vacancies v "
        "JOIN employers e ON v.employer_id = e.id "
        "LEFT JOIN experience exp ON v.experience_status_id = exp.id "
        "LEFT JOIN work_schedule ws ON v.work_schedule_status_id = ws.id "
        "LEFT JOIN educ_statuses ed ON v.educ_status_id = ed.id "
        "WHERE v.id = :vacancyId");
    query.bindValue(":vacancyId", vacancyId);

    if (query.exec() && query.next()) {
        // Стили
        QString sectionStyle = "background-color: #383838;"
            "border-radius: 6px;"
            "padding: 12px;"
            "margin-bottom: 10px;";

        QString titleStyle = "font-size: 18px;"
            "font-weight: bold;"
            "color: #BB86FC;"
            "margin-bottom: 8px;";

        QString fieldStyle = "font-size: 14px;"
            "color: #BB86FC;"
            "margin: 4px 0;";

        QString valueStyle = "font-size: 14px;"
            "color: #E0E0E0;"
            "margin: 4px 0 12px 0;";

                // 1. Блок заголовка
        QFrame *headerFrame = new QFrame();
        headerFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);

        QLabel *titleLabel = new QLabel(query.value("name").toString());
        titleLabel->setStyleSheet(titleStyle);

        QLabel *companyLabel = new QLabel(query.value("company_name").toString());
        companyLabel->setStyleSheet("font-size: 15px; color: #E0E0E0;");

        headerLayout->addWidget(titleLabel);
        headerLayout->addWidget(companyLabel);
        contentLayout->addWidget(headerFrame);

                // 2. Блок основной информации
        QFrame *infoFrame = new QFrame();
        infoFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);

        QString salary = query.value("salary").isNull() ? "не указана"
                                                        : QString("%1 руб.").arg(query.value("salary").toString());

        addFormField(infoLayout, "Зарплата:", salary, fieldStyle, valueStyle);
        addFormField(infoLayout, "График работы:", query.value("work_schedule").toString(), fieldStyle, valueStyle);
        addFormField(infoLayout, "Опыт:", query.value("experience").toString(), fieldStyle, valueStyle);

        contentLayout->addWidget(infoFrame);

                // 3. Блок требований
        QFrame *requirementsFrame = new QFrame();
        requirementsFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *reqLayout = new QVBoxLayout(requirementsFrame);

        addFormField(reqLayout, "Образование:", query.value("education").toString(), fieldStyle, valueStyle);
        addFormField(reqLayout, "Местоположение:", query.value("place").toString(), fieldStyle, valueStyle);

        contentLayout->addWidget(requirementsFrame);

                // 4. Блок описания
        QFrame *descFrame = new QFrame();
        descFrame->setStyleSheet(sectionStyle);
        QVBoxLayout *descLayout = new QVBoxLayout(descFrame);

        QLabel *descTitle = new QLabel("Описание вакансии");
        descTitle->setStyleSheet("font-size: 15px; color: #BB86FC; margin-bottom: 8px;");

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
    if(!isCandidate) {
        QMessageBox::warning(this, "Ошибка", "Только кандидаты могут откликаться на вакансии");
        return;
    }

    int candidateId = getCurrentCandidateId(); // Нужно реализовать этот метод
    if(candidateId == -1) {
        QMessageBox::warning(this, "Ошибка", "Не удалось определить ваш профиль");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM responces "
        "WHERE candidate_id = ? AND vacancy_id = ?");
    checkQuery.addBindValue(candidateId);
    checkQuery.addBindValue(vacancyId);

    if(!checkQuery.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка проверки откликов: " +
                                  checkQuery.lastError().text());
        return;
    }

    if(checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::information(this, "Информация",
                                 "Вы уже откликались на эту вакансию");
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO responces "
        "(candidate_id, vacancy_id, status, created_at) "
        "VALUES (?, ?, 'pending', CURRENT_TIMESTAMP)");
    insertQuery.addBindValue(candidateId);
    insertQuery.addBindValue(vacancyId);

    if(!insertQuery.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось сохранить отклик: " +
                                  insertQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "Успех",
                             "Ваш отклик успешно отправлен!\n"
                             "Работодатель получит уведомление.");
}


int MainPage::getCurrentCandidateId()
{
    QSqlQuery query;
    query.prepare("SELECT id FROM candidates WHERE email = ?");
    query.addBindValue(email); // currentUserId нужно получить из сессии

    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}


void MainPage::addFormField(QLayout *layout, const QString &fieldName,
                            const QString &fieldValue,
                            const QString &fieldStyle,
                            const QString &valueStyle) {
    QLabel *fieldLabel = new QLabel(fieldName);
    fieldLabel->setStyleSheet(fieldStyle);

    QLabel *valueLabel = new QLabel(fieldValue.isEmpty() ? "не указано" : fieldValue);
    valueLabel->setStyleSheet(valueStyle);
    valueLabel->setWordWrap(true);

    layout->addWidget(fieldLabel);
    layout->addWidget(valueLabel);
}

void MainPage::show() {
    flow_layout = new FlowLayout();
    QSqlQuery query;

    const int CARD_WIDTH = 250;  // Фиксированная ширина
    const int CARD_HEIGHT = 180; // Фиксированная высота
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
                QString name = query.value("name").toString();
                QString surname = query.value("surname").toString();
                QString phone_number = query.value("phone_num").toString();
                flow_layout->addWidget(
                    new VacancyCard(name, surname, phone_number)
                );
            }
        }
    } else {
        QSqlQuery query("SELECT v.*, e.company_name FROM vacancies v "
            "JOIN employers e ON v.employer_id = e.id "
            "WHERE v.status = 1");
        while (query.next()) {
            QString title = query.value("name").toString();
            QString company = query.value("company_name").toString();
            QString salary = query.value("salary").toString();
            QString experience = query.value("experience_status_id").toString();
            QString description = QString("Зарплата: %1\nОпыт: %2")
                                      .arg(salary.isEmpty() ? "не указана" : salary)
                                      .arg(experience.isEmpty() ? "не указан" : experience);

            VacancyCard *card = new VacancyCard(title, company, description);
            card->setFixedSize(CARD_WIDTH, CARD_HEIGHT);
            card->setVacancyId(query.value("id").toInt());

            connect(card, &VacancyCard::detailsRequested, this, &MainPage::showVacancyDetails);
            flow_layout->addWidget(card);
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

void MainPage::setEmail(QString email_) {
    email = email_;
}

void MainPage::on_profilePB_3_clicked() {
    emit onProfilePressed();
}
