#ifndef PROFILEPAGEFOREMPLOYER_H
#define PROFILEPAGEFOREMPLOYER_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class ProfilePageForEmployer;
}

class ProfilePageForEmployer : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePageForEmployer(
        QNetworkAccessManager *manager,
        QWidget *parent = nullptr
    );
    ~ProfilePageForEmployer();

    void updateEmployerData(
        const QString &companyName,
        const QString &email,
        const QString &about
    );

    void addCandidateWidget(
        QVBoxLayout *layout,
        const QString &name,
        const QString &position,
        const QString &date,
        int vacancyId,
        int candidateId
    );

    void acceptCandidate(int candidateId, int vacancyId);
    void rejectCandidate(int candidateId, int vacancyId);

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
    void loadExperienceData(QComboBox *comboBox);
    void loadWorkScheduleData(QComboBox *comboBox);
    void loadEducStatusData(QComboBox *comboBox);
    void loadRemotenessData(QComboBox *comboBox);
    void loadResponses();
    void deleteResponse(int vacancyId, int candidateId);
    int currentEmployerId;
    QNetworkAccessManager *networkManager;
    Ui::ProfilePageForEmployer *ui;
    QVBoxLayout *candidatesLayout;

    QPushButton *
    createActionButton(const QString &iconText, const QString &tooltip);
};
#endif  // PROFILEPAGEFOREMPLOYER_H
