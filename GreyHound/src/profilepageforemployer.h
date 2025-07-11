#ifndef PROFILEPAGEFOREMPLOYER_H
#define PROFILEPAGEFOREMPLOYER_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QScrollArea>
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
    explicit ProfilePageForEmployer(QNetworkAccessManager *manager,
                                    QWidget *parent = nullptr);
    ~ProfilePageForEmployer();

    void setEmployerData(const QString &companyName, const QString &email,
                         const QString &about);

    void addCandidateWidget(QVBoxLayout *layout, const QString &name,
                            const QString &position, const QString &date,
                            int vacancyId, int candidateId);

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
    void sendAcceptanceEmail(int candidateId, int vacancyId);
    void deleteResponse(int vacancyId, int candidateId);
    QString getExpThroughId(const int &id);
    QString getWorkScheduleThroughId(const int &id);
    QString getEducationStatusThroughId(const int &id);
    int currentEmployerId;
    QNetworkAccessManager *networkManager;
    Ui::ProfilePageForEmployer *ui;
    QScrollArea *responsesScrollArea = nullptr;
    QWidget *responsesContainer = nullptr;
    QVBoxLayout *responsesLayout = nullptr;

    QPushButton *createActionButton(const QString &iconText,
                                    const QString &tooltip);
};
#endif // PROFILEPAGEFOREMPLOYER_H
