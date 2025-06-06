#include "profilepageforemployer.h"
#include <qdatetime.h>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
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

void ProfilePageForEmployer::updateEmployerData(
    const QString &companyName,
    const QString &email,
    const QString &about
) {
    ui->companyNameEdit->setText(companyName);
    ui->emailLabel->setText(email);
    ui->aboutEdit->setPlainText(about);
    loadVacancies();
    loadResponses();
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
    QSqlQuery query;
    query.prepare(
        "UPDATE employers SET company_name = :name, about = :about WHERE email "
        "= :email"
    );
    query.bindValue(":name", ui->companyNameEdit->text());
    query.bindValue(":about", ui->aboutEdit->toPlainText());
    query.bindValue(":email", ui->emailLabel->text());

    if (!query.exec()) {
        QMessageBox::critical(
            this, "Ошибка", "Ошибка сохранения: " + query.lastError().text()
        );
    } else {
        QMessageBox::information(this, "Успех", "Данные компании обновлены!");
    }
}

void ProfilePageForEmployer::loadVacancies() {
    ui->vacanciesTable->setRowCount(0);

    QSqlQuery query;
    query.prepare(
        "SELECT v.*, exp.name AS experience, w_s.name AS schedule, educ_s.name "
        "AS educ FROM vacancies v"
        " JOIN employers e ON v.employer_id = e.id"
        " LEFT JOIN experience exp ON v.experience_status_id = exp.id"
        " LEFT JOIN work_schedule w_s ON v.work_schedule_status_id = w_s.id"
        " LEFT JOIN educ_statuses educ_s ON v.educ_status_id= educ_s.id"
        " WHERE e.email = :email;"
    );
    query.bindValue(":email", ui->emailLabel->text());
    int counter = 0;
    if (query.exec()) {
        while (query.next()) {
            qDebug() << counter << "\n";
            int row = ui->vacanciesTable->rowCount();
            ui->vacanciesTable->insertRow(row);

            QTableWidgetItem *nameItem =
                new QTableWidgetItem(query.value("name").toString());
            QTableWidgetItem *salaryItem =
                new QTableWidgetItem(query.value("salary").toString());
            QTableWidgetItem *placeItem =
                new QTableWidgetItem(query.value("place").toString());
            QTableWidgetItem *educPlaceItem =
                new QTableWidgetItem(query.value("educ_place").toString());
            QTableWidgetItem *experienceItem =
                new QTableWidgetItem(query.value("experience").toString());
            QTableWidgetItem *scheduleItem =
                new QTableWidgetItem(query.value("schedule").toString());
            QTableWidgetItem *educItem =
                new QTableWidgetItem(query.value("educ").toString());
            QTableWidgetItem *remotenessItem = new QTableWidgetItem(
                query.value("remoteness_status_id").toString()
            );

            QWidget *actionWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(actionWidget);

            QPushButton *editButton = new QPushButton("✎");
            editButton->setProperty("vacancyId", query.value("id").toInt());
            connect(
                editButton, &QPushButton::clicked, this,
                &ProfilePageForEmployer::onEditVacancyClicked
            );

            QPushButton *deleteButton = new QPushButton("🗑");
            deleteButton->setProperty("vacancyId", query.value("id").toInt());
            connect(
                deleteButton, &QPushButton::clicked, this,
                &ProfilePageForEmployer::onDeleteVacancyClicked
            );

            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->setContentsMargins(0, 0, 0, 0);

            ui->vacanciesTable->setItem(row, 0, nameItem);
            ui->vacanciesTable->setItem(row, 1, salaryItem);
            ui->vacanciesTable->setItem(row, 2, placeItem);
            ui->vacanciesTable->setItem(row, 3, educPlaceItem);
            ui->vacanciesTable->setItem(row, 4, experienceItem);
            ui->vacanciesTable->setItem(row, 5, scheduleItem);
            ui->vacanciesTable->setItem(row, 6, educItem);
            ui->vacanciesTable->setItem(row, 7, remotenessItem);
            ui->vacanciesTable->setCellWidget(row, 8, actionWidget);
        }
    }
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
        loadResponses();  // Обновляем список
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

    QSqlQuery queryExperience;
    queryExperience.prepare("SELECT * FROM experience");

    if (queryExperience.exec()) {
        while (queryExperience.next()) {
            int id = queryExperience.value("id").toInt();
            QString name = queryExperience.value("name").toString();
            desiredExperienceCombo->addItem(name, id);
        }
    } else {
        qDebug() << "Ошибка получения данных из таблицы experience:"
                 << queryExperience.lastError();
    }

    QSqlQuery querySchedule;
    querySchedule.prepare("SELECT * FROM work_schedule");

    if (querySchedule.exec()) {
        while (querySchedule.next()) {
            int id = querySchedule.value("id").toInt();
            QString name = querySchedule.value("name").toString();
            workScheduleCombo->addItem(name, id);
        }
    } else {
        qDebug() << "Ошибка получения данных из таблицы experience:"
                 << querySchedule.lastError();
    }

    QSqlQuery queryEduc;
    queryEduc.prepare("SELECT * FROM educ_statuses");

    if (queryEduc.exec()) {
        while (queryEduc.next()) {
            int id = queryEduc.value("id").toInt();
            QString name = queryEduc.value("name").toString();
            educationCombo->addItem(name, id);
        }
    } else {
        qDebug() << "Ошибка получения данных из таблицы experience:"
                 << queryEduc.lastError();
    }

    remotePossibleCombo->addItem("Да", 1);
    remotePossibleCombo->addItem("Нет", 0);

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
        QSqlQuery queryInsert;
        queryInsert.prepare(
            "INSERT INTO vacancies (name, salary, status, about, employer_id, "
            "place, "
            "educ_place, experience_status_id, work_schedule_status_id, "
            "educ_status_id, remoteness_status_id) "
            "VALUES (:name, :salary, :status, :about, :employer_id, :place, "
            ":educ_place, "
            ":experience_status_id, :work_schedule_status_id, :educ_status_id, "
            ":remoteness_status_id)"
        );
        queryInsert.bindValue(":name", nameEdit->text());
        queryInsert.bindValue(":salary", salaryEdit->text().toDouble());
        queryInsert.bindValue(":status", statusCombo->currentData().toInt());
        queryInsert.bindValue(":about", aboutEdit->toPlainText());
        queryInsert.bindValue(":employer_id", currentEmployerId);
        queryInsert.bindValue(":place", locationEdit->text());
        queryInsert.bindValue(":educ_place", educationInstitutionEdit->text());
        queryInsert.bindValue(
            ":experience_status_id",
            desiredExperienceCombo->currentData().toInt()
        );
        queryInsert.bindValue(
            ":work_schedule_status_id", workScheduleCombo->currentData().toInt()
        );
        queryInsert.bindValue(
            ":educ_status_id", educationCombo->currentData().toInt()
        );
        queryInsert.bindValue(
            ":remoteness_status_id", remotePossibleCombo->currentData().toInt()
        );

        if (!queryInsert.exec()) {
            qDebug() << "Ошибка добавления вакансии:"
                     << queryInsert.lastError();
            return;
        }

        loadVacancies();
    }
}

void ProfilePageForEmployer::onEditVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int vacancyId = button->property("vacancyId").toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM vacancies WHERE id = :id");
    query.bindValue(":id", vacancyId);

    if (query.exec() && query.next()) {
        QDialog dialog(this);
        QFormLayout form(&dialog);

        QLineEdit *nameEdit = new QLineEdit(query.value("name").toString());
        QDoubleSpinBox *salaryEdit = new QDoubleSpinBox();
        salaryEdit->setValue(query.value("salary").toDouble());
        QLineEdit *placeEdit = new QLineEdit(query.value("place").toString());
        QLineEdit *educPlaceEdit =
            new QLineEdit(query.value("educ_place").toString());
        QComboBox *experienceCombo = new QComboBox();
        QComboBox *workScheduleCombo = new QComboBox();
        QComboBox *educStatusCombo = new QComboBox();
        QComboBox *remotenessCombo = new QComboBox();
        QTextEdit *aboutEdit = new QTextEdit(query.value("about").toString());

        loadExperienceData(experienceCombo);
        loadWorkScheduleData(workScheduleCombo);
        loadEducStatusData(educStatusCombo);
        loadRemotenessData(remotenessCombo);

        experienceCombo->setCurrentIndex(experienceCombo->findData(
            query.value("experience_status_id").toInt()
        ));
        workScheduleCombo->setCurrentIndex(workScheduleCombo->findData(
            query.value("work_schedule_status_id").toInt()
        ));
        educStatusCombo->setCurrentIndex(
            educStatusCombo->findData(query.value("educ_status_id").toInt())
        );
        remotenessCombo->setCurrentIndex(remotenessCombo->findData(
            query.value("remoteness_status_id").toInt()
        ));

        form.addRow("Название:", nameEdit);
        form.addRow("Зарплата:", salaryEdit);
        form.addRow("Местоположение:", placeEdit);
        form.addRow("Место обучения:", educPlaceEdit);
        form.addRow("Желаемый опыт:", experienceCombo);
        form.addRow("Требуемый график:", workScheduleCombo);
        form.addRow("Образование:", educStatusCombo);
        form.addRow("Удаленность:", remotenessCombo);
        form.addRow("Описание вакансии:", aboutEdit);

        QDialogButtonBox buttonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal,
            &dialog
        );
        form.addRow(&buttonBox);

        connect(
            &buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept
        );
        connect(
            &buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject
        );

        if (dialog.exec() == QDialog::Accepted) {
            QSqlQuery updateQuery;
            updateQuery.prepare(
                "UPDATE vacancies SET name = :name, salary = :salary, place = "
                ":place, "
                "educ_place = :educ_place, experience_status_id = "
                ":experience_status_id, "
                "work_schedule_status_id = :work_schedule_status_id, "
                "educ_status_id = :educ_status_id, "
                "remoteness_status_id = :remoteness_status_id, about = :about "
                "WHERE id = :id"
            );

            updateQuery.bindValue(":name", nameEdit->text());
            updateQuery.bindValue(":salary", salaryEdit->value());
            updateQuery.bindValue(":place", placeEdit->text());
            updateQuery.bindValue(":educ_place", educPlaceEdit->text());
            updateQuery.bindValue(
                ":experience_status_id", experienceCombo->currentData().toInt()
            );
            updateQuery.bindValue(
                ":work_schedule_status_id",
                workScheduleCombo->currentData().toInt()
            );
            updateQuery.bindValue(
                ":educ_status_id", educStatusCombo->currentData().toInt()
            );
            updateQuery.bindValue(
                ":remoteness_status_id", remotenessCombo->currentData().toInt()
            );
            updateQuery.bindValue(":about", aboutEdit->toPlainText());
            updateQuery.bindValue(":id", vacancyId);

            if (updateQuery.exec()) {
                loadVacancies();
            } else {
                qDebug() << "Ошибка обновления вакансии:"
                         << updateQuery.lastError();
            }
        }
    }
}

void ProfilePageForEmployer::loadExperienceData(QComboBox *comboBox) {
    QSqlQuery query;
    query.prepare("SELECT * FROM experience");
    if (query.exec()) {
        while (query.next()) {
            comboBox->addItem(
                query.value("name").toString(), query.value("id")
            );
        }
    } else {
        qDebug() << "Ошибка загрузки данных опыта:" << query.lastError();
    }
}

void ProfilePageForEmployer::loadWorkScheduleData(QComboBox *comboBox) {
    QSqlQuery query;
    query.prepare("SELECT * FROM work_schedule");
    if (query.exec()) {
        while (query.next()) {
            comboBox->addItem(
                query.value("name").toString(), query.value("id")
            );
        }
    } else {
        qDebug() << "Ошибка загрузки данных расписания работы:"
                 << query.lastError();
    }
}

void ProfilePageForEmployer::loadEducStatusData(QComboBox *comboBox) {
    QSqlQuery query;
    query.prepare("SELECT * FROM educ_statuses");
    if (query.exec()) {
        while (query.next()) {
            comboBox->addItem(
                query.value("name").toString(), query.value("id")
            );
        }
    } else {
        qDebug() << "Ошибка загрузки данных статусов образования:"
                 << query.lastError();
    }
}

void ProfilePageForEmployer::loadRemotenessData(QComboBox *comboBox) {
    comboBox->addItem("Да", 1);
    comboBox->addItem("Нет", 0);
}

void ProfilePageForEmployer::onDeleteVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int vacancyId = button->property("vacancyId").toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Удаление", "Удалить вакансию?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM vacancies WHERE id = :id");
        query.bindValue(":id", vacancyId);

        if (query.exec()) {
            loadVacancies();
        } else {
            QMessageBox::critical(
                this, "Ошибка", "Ошибка удаления: " + query.lastError().text()
            );
        }
    }
}
