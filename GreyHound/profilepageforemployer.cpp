#include "profilepageforemployer.h"
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

ProfilePageForEmployer::ProfilePageForEmployer(QWidget *parent)
    : QWidget(parent) {
    SetupUI();
}

ProfilePageForEmployer::~ProfilePageForEmployer() = default;

void ProfilePageForEmployer::SetupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    QScrollArea *leftScrollArea = new QScrollArea();
    QWidget *notificationsWidget = new QWidget();
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);
    QLabel *notificationsLabel = new QLabel("Уведомления");
    notificationsLayout->addWidget(notificationsLabel);
    leftScrollArea->setWidget(notificationsWidget);
    leftScrollArea->setWidgetResizable(true);
    mainLayout->addWidget(leftScrollArea, 1);

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QGroupBox *companyInfoGroup = new QGroupBox("Информация о компании");
    QFormLayout *companyInfoLayout = new QFormLayout(companyInfoGroup);

    companyNameEdit = new QLineEdit();
    emailLabel = new QLabel("Загрузка...");
    aboutEdit = new QTextEdit();

    companyInfoLayout->addRow("Название компании", companyNameEdit);
    companyInfoLayout->addRow("Email", emailLabel);
    companyInfoLayout->addRow("О компании", aboutEdit);

    QGroupBox *vacanciesGroup = new QGroupBox("Вакансии");
    QVBoxLayout *vacanciesLayout = new QVBoxLayout(vacanciesGroup);

    vacanciesTable = new QTableWidget();
    vacanciesTable->setColumnCount(4);
    vacanciesTable->setHorizontalHeaderLabels(
        {"Название", "Зарплата", "Статус", "Действия"}
    );
    vacanciesTable->horizontalHeader()->setStretchLastSection(true);

    QPushButton *addVacancyButton = new QPushButton("Добавить вакансию");
    connect(
        addVacancyButton, &QPushButton::clicked, this,
        &ProfilePageForEmployer::onAddVacancyClicked
    );

    vacanciesLayout->addWidget(vacanciesTable);
    vacanciesLayout->addWidget(addVacancyButton);

    centralLayout->addWidget(companyInfoGroup);
    centralLayout->addSpacing(15);
    centralLayout->addWidget(vacanciesGroup);
    centralLayout->addStretch();

    mainLayout->addWidget(centralWidget, 3);

    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QPushButton *homepagePB = new QPushButton("На главную");
    connect(
        homepagePB, &QPushButton::clicked, this,
        &ProfilePageForEmployer::homeButtonClicked
    );

    QPushButton *saveButton = new QPushButton("Сохранить изменения");
    connect(
        saveButton, &QPushButton::clicked, this,
        &ProfilePageForEmployer::onSaveClicked
    );

    QPushButton *logoutPB = new QPushButton("Выйти");
    logoutPB->setStyleSheet("QPushButton { color: red; }");
    connect(
        logoutPB, &QPushButton::clicked, this,
        &ProfilePageForEmployer::logoutButtonClicked
    );

    rightLayout->addWidget(homepagePB, 0, Qt::AlignTop);
    rightLayout->addStretch();
    rightLayout->addWidget(saveButton, 0, Qt::AlignBottom);
    rightLayout->addWidget(logoutPB, 0, Qt::AlignBottom);

    mainLayout->addWidget(rightWidget, 1);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);
}

void ProfilePageForEmployer::updateEmployerData(
    const QString &companyName,
    const QString &email,
    const QString &about,
    const int ID
) {
    companyNameEdit->setText(companyName);
    emailLabel->setText(email);
    aboutEdit->setPlainText(about);
    currentEmployerId = ID;
    loadVacancies();
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
    query.bindValue(":name", companyNameEdit->text());
    query.bindValue(":about", aboutEdit->toPlainText());
    query.bindValue(":email", emailLabel->text());

    if (!query.exec()) {
        QMessageBox::critical(
            this, "Ошибка", "Ошибка сохранения: " + query.lastError().text()
        );
    } else {
        QMessageBox::information(this, "Успех", "Данные компании обновлены!");
    }
}

void ProfilePageForEmployer::loadVacancies() {
    vacanciesTable->setRowCount(0);

    QSqlQuery query;
    query.prepare(
        "SELECT v.* FROM vacancies v JOIN employers e ON v.employer_id = e.id WHERE e.email = :email;"
    );
    query.bindValue(":email", emailLabel->text());

    if (query.exec()) {
        while (query.next()) {
            int row = vacanciesTable->rowCount();
            vacanciesTable->insertRow(row);

            QTableWidgetItem *nameItem =
                new QTableWidgetItem(query.value("name").toString());
            QTableWidgetItem *salaryItem =
                new QTableWidgetItem(query.value("salary").toString());
            QTableWidgetItem *statusItem =
                new QTableWidgetItem(query.value("status").toString());

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

            vacanciesTable->setItem(row, 0, nameItem);
            vacanciesTable->setItem(row, 1, salaryItem);
            vacanciesTable->setItem(row, 2, statusItem);
            vacanciesTable->setCellWidget(row, 3, actionWidget);
        }
    }
}

void ProfilePageForEmployer::onAddVacancyClicked() {
    QDialog dialog(this);
    QFormLayout form(&dialog);

    QLineEdit *nameEdit = new QLineEdit();
    QDoubleSpinBox *salaryEdit = new QDoubleSpinBox();
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
        qDebug() << "Ошибка получения данных из таблицы experience:" << queryExperience.lastError();
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
        qDebug() << "Ошибка получения данных из таблицы experience:" << querySchedule.lastError();
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
        qDebug() << "Ошибка получения данных из таблицы experience:" << queryEduc.lastError();
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

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery queryInsert;
        queryInsert.prepare(
            "INSERT INTO vacancies (name, salary, status, about, employer_id, place, "
            "educ_place, experience_status_id, work_schedule_status_id, educ_status_id, remoteness_status_id) "
            "VALUES (:name, :salary, :status, :about, :employer_id, :place, :educ_place, "
            ":experience_status_id, :work_schedule_status_id, :educ_status_id, :remoteness_status_id)"
            );
        queryInsert.bindValue(":name", nameEdit->text());
        queryInsert.bindValue(":salary", salaryEdit->value());
        queryInsert.bindValue(":status", statusCombo->currentData().toInt());
        queryInsert.bindValue(":about", aboutEdit->toPlainText());
        queryInsert.bindValue(":employer_id", currentEmployerId);
        queryInsert.bindValue(":place", locationEdit->text());
        queryInsert.bindValue(":educ_place", educationInstitutionEdit->text());
        queryInsert.bindValue(":experiences_status_id", desiredExperienceCombo->currentData().toInt());
        queryInsert.bindValue(":work_schedule_status_id", workScheduleCombo->currentData().toInt());
        queryInsert.bindValue(":educ_status_id", educationCombo->currentData().toInt());
        queryInsert.bindValue(":remoteness_status_id", remotePossibleCombo->currentData().toInt());

        if (!queryInsert.exec()) {
            qDebug() << "Ошибка добавления вакансии:" << queryInsert.lastError();
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
        QLineEdit *educPlaceEdit = new QLineEdit(query.value("educ_place").toString());
        QComboBox *experienceCombo = new QComboBox();
        QComboBox *workScheduleCombo = new QComboBox();
        QComboBox *educStatusCombo = new QComboBox();
        QComboBox *remotenessCombo = new QComboBox();
        QTextEdit *aboutEdit = new QTextEdit(query.value("about").toString());

        loadExperienceData(experienceCombo);
        loadWorkScheduleData(workScheduleCombo);
        loadEducStatusData(educStatusCombo);
        loadRemotenessData(remotenessCombo);

        experienceCombo->setCurrentIndex(experienceCombo->findData(query.value("experience_status_id").toInt()));
        workScheduleCombo->setCurrentIndex(workScheduleCombo->findData(query.value("work_schedule_status_id").toInt()));
        educStatusCombo->setCurrentIndex(educStatusCombo->findData(query.value("educ_status_id").toInt()));
        remotenessCombo->setCurrentIndex(remotenessCombo->findData(query.value("remoteness_status_id").toInt()));

        form.addRow("Название:", nameEdit);
        form.addRow("Зарплата:", salaryEdit);
        form.addRow("Местоположение:", placeEdit);
        form.addRow("Место обучения:", educPlaceEdit);
        form.addRow("Желаемый опыт:", experienceCombo);
        form.addRow("Требуемый график:", workScheduleCombo);
        form.addRow("Образование:", educStatusCombo);
        form.addRow("Удаленность:", remotenessCombo);
        form.addRow("Описание вакансии:", aboutEdit);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);

        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            QSqlQuery updateQuery;
            updateQuery.prepare(
                "UPDATE vacancies SET name = :name, salary = :salary, place = :place, "
                "educ_place = :educ_place, experience_status_id = :experience_status_id, "
                "work_schedule_status_id = :work_schedule_status_id, educ_status_id = :educ_status_id, "
                "remoteness_status_id = :remoteness_status_id, about = :about "
                "WHERE id = :id"
                );

            updateQuery.bindValue(":name", nameEdit->text());
            updateQuery.bindValue(":salary", salaryEdit->value());
            updateQuery.bindValue(":place", placeEdit->text());
            updateQuery.bindValue(":educ_place", educPlaceEdit->text());
            updateQuery.bindValue(":experience_status_id", experienceCombo->currentData().toInt());
            updateQuery.bindValue(":work_schedule_status_id", workScheduleCombo->currentData().toInt());
            updateQuery.bindValue(":educ_status_id", educStatusCombo->currentData().toInt());
            updateQuery.bindValue(":remoteness_status_id", remotenessCombo->currentData().toInt());
            updateQuery.bindValue(":about", aboutEdit->toPlainText());
            updateQuery.bindValue(":id", vacancyId);

            if (updateQuery.exec()) {
                loadVacancies();
            } else {
                qDebug() << "Ошибка обновления вакансии:" << updateQuery.lastError();
            }
        }
    }
}

void ProfilePageForEmployer::loadExperienceData(QComboBox *comboBox) {
    QSqlQuery query;
    query.prepare("SELECT * FROM experience");
    if (query.exec()) {
        while (query.next()) {
            comboBox->addItem(query.value("name").toString(), query.value("id"));
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
            comboBox->addItem(query.value("name").toString(), query.value("id"));
        }
    } else {
        qDebug() << "Ошибка загрузки данных расписания работы:" << query.lastError();
    }
}

void ProfilePageForEmployer::loadEducStatusData(QComboBox *comboBox) {
    QSqlQuery query;
    query.prepare("SELECT * FROM educ_statuses");
    if (query.exec()) {
        while (query.next()) {
            comboBox->addItem(query.value("name").toString(), query.value("id"));
        }
    } else {
        qDebug() << "Ошибка загрузки данных статусов образования:" << query.lastError();
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
