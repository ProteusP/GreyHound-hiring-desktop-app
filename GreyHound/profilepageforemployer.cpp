#include "profilepageforemployer.h"
#include <qdatetime.h>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "ui_profilepageforemployer.h"

ProfilePageForEmployer::ProfilePageForEmployer(
    QNetworkAccessManager *manager,
    QWidget *parent
)
    : QWidget(parent),
      ui(new Ui::ProfilePageForEmployer),
      networkManager(manager) {
    ui->setupUi(this);
    SetupUI();
}

ProfilePageForEmployer::~ProfilePageForEmployer() = default;

void ProfilePageForEmployer::SetupUI() {
    ui->vacanciesTable->resizeColumnsToContents();
    connect(
        ui->addVacancyButton, &QPushButton::clicked, this,
        &ProfilePageForEmployer::onAddVacancyClicked
    );
    connect(
        ui->homepagePB, &QPushButton::clicked, this,
        &ProfilePageForEmployer::homeButtonClicked
    );
    connect(
        ui->saveButton, &QPushButton::clicked, this,
        &ProfilePageForEmployer::onSaveClicked
    );
    connect(
        ui->logoutPB, &QPushButton::clicked, this,
        &ProfilePageForEmployer::logoutButtonClicked
    );
}

void ProfilePageForEmployer::setEmployerData(
    const QString &companyName,
    const QString &email,
    const QString &about
) {
    ui->companyNameEdit->setText(companyName);
    ui->emailEdit->setText(email);
    ui->aboutEdit->setPlainText(about);
    loadVacancies();
    // loadResponses();
}

void ProfilePageForEmployer::loadResponses() {
    QLayoutItem *item;
    while ((item = candidatesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT r.vacancy_id, r.candidate_id, r.created_at, "
        "v.name as vacancy_name, "
        "c.name, c.surname "
        "FROM responces r "
        "LEFT JOIN vacancies v ON r.vacancy_id = v.id "
        "LEFT JOIN candidates c ON r.candidate_id = c.id "
        "WHERE v.employer_id = :employer_id AND r.status = 'pending'"
    );
    query.bindValue(":employer_id", currentEmployerId);

    if (query.exec()) {
        while (query.next()) {
            int vacancyId = query.value("vacancy_id").toInt();
            int candidateId = query.value("candidate_id").toInt();
            QString createdAt =
                query.value("created_at").toDateTime().toString("dd.MM.yyyy");
            QString vacancyName = query.value("vacancy_name").toString();
            QString firstName = query.value("name").toString();
            QString lastName = query.value("surname").toString();
            QString fullName = QString("%1 %2").arg(firstName, lastName);

            addCandidateWidget(
                candidatesLayout, fullName, vacancyName, createdAt, vacancyId,
                candidateId
            );
        }
    } else {
        qDebug() << "Ошибка загрузки откликов:" << query.lastError().text();
    }
}

void ProfilePageForEmployer::onSaveClicked() {
    saveCompanyInfo();
}

void ProfilePageForEmployer::saveCompanyInfo() {
    QString newName = ui->companyNameEdit->text();
    QString newAbout = ui->aboutEdit->toPlainText();
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:80/api/v1/profile/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["company_name"] = newName;
    json["about"] = newAbout;
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply *reply =
        networkManager->sendCustomRequest(request, "PATCH", data);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Успех", "Данные сохранены!");
        } else {
            int statusCode =
                reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                    .toInt();
            if (statusCode == 400) {
                QMessageBox::warning(
                    this, "Ошибка",
                    "По пути на сервер данные потерялись( просим прощения"
                );
            } else if (statusCode == 500) {
                QMessageBox::warning(this, "Упс...", "Ошибка сервера.");
            } else {
                QMessageBox::warning(this, "Упс...", "Что-то непонятное.");
            }
        }
        reply->deleteLater();
    });
    // saveResumeData();
}

void ProfilePageForEmployer::loadVacancies() {
    ui->vacanciesTable->setRowCount(0);

    QNetworkRequest request(
        QUrl("http://localhost:80/api/v1/resources/emplVacancies")
    );
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Ошибка загрузки вакансий:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject root = doc.object();
        QJsonArray vacancies = root["vacancies"].toArray();

        for (int i = 0; i < vacancies.size(); i++) {
            QJsonObject vac = vacancies[i].toObject();
            int row = ui->vacanciesTable->rowCount();
            ui->vacanciesTable->insertRow(row);

            auto addItem = [&](int column, const QString &value) {
                ui->vacanciesTable->setItem(
                    row, column, new QTableWidgetItem(value)
                );
            };

            addItem(0, vac["name"].toString());
            addItem(1, vac["salary"].toString());
            addItem(2, vac["place"].toString());
            addItem(3, vac["educ_place"].toString());
            addItem(4, getExpThroughId(vac["experience_status_id"].toInt()));
            addItem(
                5,
                getWorkScheduleThroughId(vac["work_schedule_status_id"].toInt())
            );
            addItem(
                6, getEducationStatusThroughId(vac["educ_status_id"].toInt())
            );
            addItem(7, vac["remoteness_status_id"].toString());

            QWidget *actionWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(actionWidget);

            QPushButton *editButton = new QPushButton("✎");
            editButton->setProperty("vacancyId", vac["id"].toInt());
            connect(
                editButton, &QPushButton::clicked, this,
                &ProfilePageForEmployer::onEditVacancyClicked
            );

            QPushButton *deleteButton = new QPushButton("🗑");
            deleteButton->setProperty("vacancyId", vac["id"].toInt());
            connect(
                deleteButton, &QPushButton::clicked, this,
                &ProfilePageForEmployer::onDeleteVacancyClicked
            );

            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->setContentsMargins(0, 0, 0, 0);

            ui->vacanciesTable->setCellWidget(row, 8, actionWidget);
        }

        reply->deleteLater();
    });
}

void ProfilePageForEmployer::addCandidateWidget(
    QVBoxLayout *layout,
    const QString &name,
    const QString &position,
    const QString &date,
    int vacancyId,
    int candidateId
) {
    QWidget *candidateWidget = new QWidget();
    candidateWidget->setStyleSheet(
        "QWidget {"
        "  background: white;"
        "  border-radius: 5px;"
        "  padding: 8px;"
        "  margin-bottom: 6px;"
        "}"
        "QLabel { font-size: 12px; }"
        "QLabel#name { font-weight: bold; color: #2C3E50; }"
        "QLabel#vacancyId { color: #7F8C8D; font-size: 11px; }"
    );

    QVBoxLayout *candidateLayout = new QVBoxLayout(candidateWidget);
    candidateLayout->setSpacing(4);
    candidateLayout->setContentsMargins(4, 4, 4, 4);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setObjectName("name");
    nameLabel->setWordWrap(true);

    QLabel *posLabel = new QLabel(position);
    posLabel->setWordWrap(true);

    QLabel *dateLabel = new QLabel("Отклик: " + date);
    dateLabel->setStyleSheet("color: #7F8C8D; font-size: 11px;");

    QLabel *vacancyLabel =
        new QLabel("ID вакансии: " + QString::number(vacancyId));
    vacancyLabel->setObjectName("vacancyId");

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(4);
    auto createButton = [](const QString &text, const QString &color,
                           const QString &hoverColor) {
        QPushButton *btn = new QPushButton(text);
        btn->setStyleSheet(QString("QPushButton {"
                                   "  background: %1;"
                                   "  color: white;"
                                   "  border: none;"
                                   "  border-radius: 3px;"
                                   "  padding: 2px 5px;"
                                   "  font-size: 10px;"
                                   "  min-width: 60px;"
                                   "  max-width: 60px;"
                                   "}"
                                   "QPushButton:hover { background: %2; }")
                               .arg(color, hoverColor));
        return btn;
    };

    QPushButton *viewBtn = createButton("Просмотр", "#3498DB", "#2980B9");
    QPushButton *acceptBtn = createButton("Принять", "#27AE60", "#219653");
    QPushButton *rejectBtn = createButton("Отклонить", "#E74C3C", "#C0392B");

    buttonsLayout->addWidget(viewBtn);
    buttonsLayout->addWidget(acceptBtn);
    buttonsLayout->addWidget(rejectBtn);
    buttonsLayout->addStretch();

    candidateLayout->addWidget(nameLabel);
    candidateLayout->addWidget(posLabel);
    candidateLayout->addWidget(vacancyLabel);
    candidateLayout->addWidget(dateLabel);
    candidateLayout->addLayout(buttonsLayout);

    layout->addWidget(candidateWidget);

    connect(viewBtn, &QPushButton::clicked, this, [this, candidateId]() {
        qDebug() << "Просмотр кандидата ID:" << candidateId;
    });

    connect(
        acceptBtn, &QPushButton::clicked, this,
        [this, candidateId, vacancyId]() {
            deleteResponse(vacancyId, candidateId);
        }
    );

    connect(
        rejectBtn, &QPushButton::clicked, this,
        [this, candidateId, vacancyId]() {
            deleteResponse(vacancyId, candidateId);
        }
    );
}

void ProfilePageForEmployer::deleteResponse(int vacancyId, int candidateId) {
    QSqlQuery query;
    query.prepare(
        "DELETE FROM responces WHERE vacancy_id = :vacancy_id AND candidate_id "
        "= :candidate_id"
    );
    query.bindValue(":vacancy_id", vacancyId);
    query.bindValue(":candidate_id", candidateId);

    if (query.exec()) {
        loadResponses();
    } else {
        qDebug() << "Ошибка удаления отклика:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить отклик");
    }
}

void ProfilePageForEmployer::onAddVacancyClicked() {
    QDialog dialog(this);

    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #f9f9f9;
            border-radius: 10px;
        }

        QLineEdit, QTextEdit {
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 5px 8px;
            font-size: 14px;
            color: black;
        }
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
        }

        QTextEdit {
            padding: 6px;
        }

        QDialogButtonBox QPushButton {
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 6px 16px;
            font-size: 14px;
        }

        QDialogButtonBox QPushButton:hover {
            background-color: #005ecb;
        }

        QDialogButtonBox QPushButton:disabled {
            background-color: #ccc;
            color: #666;
        }

        QLabel {
            font-size: 14px;
            color: black;
        }
    )");

    QFormLayout form(&dialog);

    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *salaryEdit = new QLineEdit();
    QComboBox *statusCombo = new QComboBox();
    QTextEdit *aboutEdit = new QTextEdit();
    QLineEdit *locationEdit = new QLineEdit();
    QLineEdit *educationInstitutionEdit = new QLineEdit();
    QComboBox *desiredExperienceCombo = new QComboBox();
    QComboBox *workScheduleCombo = new QComboBox();
    QComboBox *educationCombo = new QComboBox();
    QComboBox *remotePossibleCombo = new QComboBox();

    statusCombo->addItem("active", 1);
    statusCombo->addItem("inactive", 0);

    loadExperienceData(desiredExperienceCombo);
    loadWorkScheduleData(workScheduleCombo);
    loadEducStatusData(educationCombo);
    loadRemotenessData(remotePossibleCombo);

    form.addRow("Название:", nameEdit);
    form.addRow("Зарплата:", salaryEdit);
    form.addRow("Статус:", statusCombo);
    form.addRow("Описание:", aboutEdit);
    form.addRow("Местоположение:", locationEdit);
    form.addRow("Место обучения:", educationInstitutionEdit);
    form.addRow("Желаемый опыт:", desiredExperienceCombo);
    form.addRow("Требуемый график:", workScheduleCombo);
    form.addRow("Образование:", educationCombo);
    form.addRow("Удаленность:", remotePossibleCombo);

    QDialogButtonBox buttonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog
    );
    form.addRow(&buttonBox);
    buttonBox.button(QDialogButtonBox::Ok)->setText("Сохранить");
    buttonBox.button(QDialogButtonBox::Ok)->setIcon(QIcon());

    buttonBox.button(QDialogButtonBox::Cancel)->setText("Отмена");
    buttonBox.button(QDialogButtonBox::Cancel)->setIcon(QIcon());

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QNetworkRequest request(
            QUrl("http://localhost:80/api/v1/resources/createVacancy")
        );
        request.setHeader(
            QNetworkRequest::ContentTypeHeader, "application/json"
        );
        QJsonObject json;
        json["name"] = nameEdit->text();
        json["salary"] = salaryEdit->text().toDouble();
        json["status"] = statusCombo->currentData().toInt();
        json["about"] = aboutEdit->toPlainText();
        json["place"] = locationEdit->text();
        json["educ_place"] = educationInstitutionEdit->text();
        json["experience_status_id"] =
            desiredExperienceCombo->currentData().toInt();
        json["work_schedule_status_id"] =
            workScheduleCombo->currentData().toInt();
        json["educ_status_id"] = educationCombo->currentData().toInt();
        json["remoteness_status_id"] =
            remotePossibleCombo->currentData().toInt();
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        QNetworkReply *reply = networkManager->post(request, data);

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response_data = reply->readAll();
                QJsonDocument response_doc =
                    QJsonDocument::fromJson(response_data);
                QJsonObject obj = response_doc.object();
                qDebug() << "Вакансия ID:" << obj["vacancy_id"];
                loadVacancies();
            } else {
                QMessageBox::warning(
                    this, "Упс...", "Ошибка при отправке запроса."
                );
            }
            reply->deleteLater();
        });
    }
}

void ProfilePageForEmployer::onEditVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int vacancyId = button->property("vacancyId").toInt();

    QDialog dialog(this);

    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #f9f9f9;
            border-radius: 10px;
        }

        QLineEdit, QTextEdit {
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 5px 8px;
            font-size: 14px;
            color: black;
        }
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
        }

        QTextEdit {
            padding: 6px;
        }

        QDialogButtonBox QPushButton {
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 6px 16px;
            font-size: 14px;
        }

        QDialogButtonBox QPushButton:hover {
            background-color: #005ecb;
        }

        QDialogButtonBox QPushButton:disabled {
            background-color: #ccc;
            color: #666;
        }

        QLabel {
            font-size: 14px;
            color: black;
        }
    )");

    QFormLayout form(&dialog);

    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *salaryEdit = new QLineEdit();
    QComboBox *statusCombo = new QComboBox();
    QTextEdit *aboutEdit = new QTextEdit();
    QLineEdit *locationEdit = new QLineEdit();
    QLineEdit *educationInstitutionEdit = new QLineEdit();
    QComboBox *desiredExperienceCombo = new QComboBox();
    QComboBox *workScheduleCombo = new QComboBox();
    QComboBox *educationCombo = new QComboBox();
    QComboBox *remotePossibleCombo = new QComboBox();

    statusCombo->addItem("active", 1);
    statusCombo->addItem("inactive", 0);

    loadExperienceData(desiredExperienceCombo);
    loadWorkScheduleData(workScheduleCombo);
    loadEducStatusData(educationCombo);
    loadRemotenessData(remotePossibleCombo);

    form.addRow("Название:", nameEdit);
    form.addRow("Зарплата:", salaryEdit);
    form.addRow("Статус:", statusCombo);
    form.addRow("Описание:", aboutEdit);
    form.addRow("Местоположение:", locationEdit);
    form.addRow("Место обучения:", educationInstitutionEdit);
    form.addRow("Желаемый опыт:", desiredExperienceCombo);
    form.addRow("Требуемый график:", workScheduleCombo);
    form.addRow("Образование:", educationCombo);
    form.addRow("Удаленность:", remotePossibleCombo);

    QDialogButtonBox buttonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog
    );
    form.addRow(&buttonBox);
    buttonBox.button(QDialogButtonBox::Ok)->setText("Сохранить");
    buttonBox.button(QDialogButtonBox::Ok)->setIcon(QIcon());

    buttonBox.button(QDialogButtonBox::Cancel)->setText("Отмена");
    buttonBox.button(QDialogButtonBox::Cancel)->setIcon(QIcon());

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QNetworkRequest request(QUrl(
            QString("http://localhost:80/api/v1/resources/updateVacancy/%1")
                .arg(vacancyId)
        ));
        request.setHeader(
            QNetworkRequest::ContentTypeHeader, "application/json"
        );
        QJsonObject updated;
        updated["name"] = nameEdit->text();
        updated["salary"] = salaryEdit->text().toInt();
        updated["status"] = statusCombo->currentData().toInt();
        updated["about"] = aboutEdit->toPlainText();
        updated["place"] = locationEdit->text();
        updated["educ_place"] = educationInstitutionEdit->text();
        updated["experience_status_id"] =
            desiredExperienceCombo->currentData().toInt();
        updated["work_schedule_status_id"] =
            workScheduleCombo->currentData().toInt();
        updated["educ_status_id"] = educationCombo->currentData().toInt();
        updated["remoteness_status_id"] =
            remotePossibleCombo->currentData().toInt();
        QByteArray data = QJsonDocument(updated).toJson();
        QNetworkReply *reply =
            networkManager->sendCustomRequest(request, "PATCH", data);
        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                loadVacancies();
            } else {
                QMessageBox::warning(this, "Упс...", "Произошла ошибка");
            }
            reply->deleteLater();
        });
    }
}

void ProfilePageForEmployer::onDeleteVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int vacancyId = button->property("vacancyId").toInt();

    QMessageBox::StandardButton replyToDelete = QMessageBox::question(
        this, "Удаление", "Удалить вакансию?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (replyToDelete == QMessageBox::Yes) {
        QNetworkRequest request(QUrl(
            QString("http://localhost:80/api/v1/resources/deleteVacancy/%1")
                .arg(vacancyId)
        ));
        request.setHeader(
            QNetworkRequest::ContentTypeHeader, "application/json"
        );
        QNetworkReply *reply =
            networkManager->sendCustomRequest(request, "DELETE");

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(response);
                QJsonObject obj = doc.object();

                if (obj.contains("count")) {
                    loadVacancies();
                } else {
                    QMessageBox::warning(
                        this, "Ошибка", "Вакансия не найдена или уже удалена."
                    );
                }
            } else {
                QMessageBox::critical(
                    this, "Ошибка сети", "Что-то непонятное..."
                );
            }
            reply->deleteLater();
        });
    }
}

void ProfilePageForEmployer::loadExperienceData(
    QComboBox *desiredExperienceCombo
) {
    desiredExperienceCombo->addItem("Без опыта", 1);
    desiredExperienceCombo->addItem("1-3 лет", 2);
    desiredExperienceCombo->addItem("3-5 лет", 3);
    desiredExperienceCombo->addItem("5+ лет", 4);
}

void ProfilePageForEmployer::loadWorkScheduleData(QComboBox *workScheduleCombo
) {
    workScheduleCombo->addItem("Полный рабочий день", 1);
    workScheduleCombo->addItem("Частичная занятость", 2);
    workScheduleCombo->addItem("Гибкий график", 3);
    workScheduleCombo->addItem("Другое", 4);
}

void ProfilePageForEmployer::loadEducStatusData(QComboBox *educationCombo) {
    educationCombo->addItem("Среднее", 1);
    educationCombo->addItem("Среднее специальное", 2);
    educationCombo->addItem("Бакалавриат", 3);
    educationCombo->addItem("Магистратура", 4);
    educationCombo->addItem("Аспирантура", 5);
    educationCombo->addItem("Не имеет значения", 6);
}

void ProfilePageForEmployer::loadRemotenessData(QComboBox *remotePossibleCombo
) {
    remotePossibleCombo->addItem("Да", 1);
    remotePossibleCombo->addItem("Нет", 0);
}

QString ProfilePageForEmployer::getExpThroughId(const int &id) {
    switch (id) {
        case 1:
            return "Без опыта";
        case 2:
            return "1-3 лет";
        case 3:
            return "3-5 лет";
        case 4:
            return "5+ лет";
        default:
            return "...";
    }
}

QString ProfilePageForEmployer::getWorkScheduleThroughId(const int &id) {
    switch (id) {
        case 1:
            return "Полный рабочий день";
        case 2:
            return "Частичная занятость";
        case 3:
            return "Гибкий график";
        case 4:
            return "Другое";
        default:
            return "...";
    }
}

QString ProfilePageForEmployer::getEducationStatusThroughId(const int &id) {
    switch (id) {
        case 1:
            return "Среднее";
        case 2:
            return "Среднее специальное";
        case 3:
            return "Бакалавриат";
        case 4:
            return "Магистратура";
        case 5:
            return "Аспирантура";
        case 6:
            return "Не имеет значения";
        default:
            return "...";
    }
}
