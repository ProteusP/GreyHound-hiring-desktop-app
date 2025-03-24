#ifndef PROFILEPAGEFOREMPLOYER_H
#define PROFILEPAGEFOREMPLOYER_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>
#include <QComboBox>

class ProfilePageForEmployer : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePageForEmployer(QWidget *parent = nullptr);
    ~ProfilePageForEmployer();

    void updateEmployerData(
        const QString &companyName,
        const QString &email,
        const QString &about,
        const int ID
        );

signals:
    void homeButtonClicked();
    void logoutButtonClicked();

private slots:
    void onSaveClicked();
    void onAddVacancyClicked();
    void onEditVacancyClicked();
    void onDeleteVacancyClicked();

private:
    void SetupUI();
    void saveCompanyInfo();
    void loadVacancies();

            // Вспомогательные функции для загрузки данных
    void loadExperienceData(QComboBox *comboBox);
    void loadWorkScheduleData(QComboBox *comboBox);
    void loadEducStatusData(QComboBox *comboBox);
    void loadRemotenessData(QComboBox *comboBox);  // Уже добавлена, но нужно объявить

    QLineEdit *companyNameEdit;
    QLabel *emailLabel;
    QTextEdit *aboutEdit;
    QTableWidget *vacanciesTable;
    int currentEmployerId;

    QPushButton *createActionButton(const QString &iconText, const QString &tooltip);
};

#endif  // PROFILEPAGEFOREMPLOYER_H
