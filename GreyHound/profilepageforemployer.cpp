#include "profilepageforemployer.h"
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDoubleSpinBox>

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
    vacanciesTable->setHorizontalHeaderLabels({"Название", "Зарплата", "Статус", "Действия"});
    vacanciesTable->horizontalHeader()->setStretchLastSection(true);

    QPushButton *addVacancyButton = new QPushButton("Добавить вакансию");
    connect(addVacancyButton, &QPushButton::clicked, this, &ProfilePageForEmployer::onAddVacancyClicked);

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
    connect(homepagePB, &QPushButton::clicked, this, &ProfilePageForEmployer::homeButtonClicked);

    QPushButton *saveButton = new QPushButton("Сохранить изменения");
    connect(saveButton, &QPushButton::clicked, this, &ProfilePageForEmployer::onSaveClicked);

    QPushButton *logoutPB = new QPushButton("Выйти");
    logoutPB->setStyleSheet("QPushButton { color: red; }");
    connect(logoutPB, &QPushButton::clicked, this, &ProfilePageForEmployer::logoutButtonClicked);

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
    query.prepare("UPDATE employers SET company_name = :name, about = :about WHERE email = :email");
    query.bindValue(":name", companyNameEdit->text());
    query.bindValue(":about", aboutEdit->toPlainText());
    query.bindValue(":email", emailLabel->text());

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Ошибка сохранения: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные компании обновлены!");
    }
}

void ProfilePageForEmployer::loadVacancies() {
    vacanciesTable->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT * FROM vacancies WHERE employer_id = (SELECT id FROM employers WHERE email = :email)");
    query.bindValue(":email", emailLabel->text());

    if (query.exec()) {
        while (query.next()) {
            int row = vacanciesTable->rowCount();
            vacanciesTable->insertRow(row);


            QTableWidgetItem *nameItem = new QTableWidgetItem(query.value("name").toString());
            QTableWidgetItem *salaryItem = new QTableWidgetItem(query.value("salary").toString());
            QTableWidgetItem *statusItem = new QTableWidgetItem(query.value("status").toString());


            QWidget *actionWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(actionWidget);

            QPushButton *editButton = new QPushButton("✎");
            editButton->setProperty("vacancyId", query.value("id").toInt());
            connect(editButton, &QPushButton::clicked, this, &ProfilePageForEmployer::onEditVacancyClicked);

            QPushButton *deleteButton = new QPushButton("🗑");
            deleteButton->setProperty("vacancyId", query.value("id").toInt());
            connect(deleteButton, &QPushButton::clicked, this, &ProfilePageForEmployer::onDeleteVacancyClicked);

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
    QLineEdit *statusEdit = new QLineEdit();
    QTextEdit *aboutEdit = new QTextEdit();


    form.addRow("Название:", nameEdit);
    form.addRow("Зарплата:", salaryEdit);
    form.addRow("Статус:", statusEdit);
    form.addRow("Описание:", aboutEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {

        QSqlQuery query;
        query.prepare("INSERT INTO vacancies (name, salary, status, about, employer_id) "
            "VALUES (:name, :salary, :status, :about, :employer_id)");
        query.bindValue(":name", nameEdit->text());
        query.bindValue(":salary", salaryEdit->value());
        query.bindValue(":status", statusEdit->text());
        query.bindValue(":about", aboutEdit->toPlainText());
        query.bindValue(":employer_id", currentEmployerId);

        if (query.exec()) {
            loadVacancies();
        } else {
            qDebug() << "Ошибка добавления вакансии:" << query.lastError();
        }
    }
}

void ProfilePageForEmployer::onEditVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
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


        form.addRow("Название:", nameEdit);
        form.addRow("Зарплата:", salaryEdit);


        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);

        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE vacancies SET name = :name, salary = :salary WHERE id = :id");
            updateQuery.bindValue(":name", nameEdit->text());
            updateQuery.bindValue(":salary", salaryEdit->value());
            updateQuery.bindValue(":id", vacancyId);

            if (updateQuery.exec()) {
                loadVacancies();
            } else {
                qDebug() << "Ошибка обновления вакансии:" << updateQuery.lastError();
            }
        }
    }
}

void ProfilePageForEmployer::onDeleteVacancyClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
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
            QMessageBox::critical(this, "Ошибка", "Ошибка удаления: " + query.lastError().text());
        }
    }
}
