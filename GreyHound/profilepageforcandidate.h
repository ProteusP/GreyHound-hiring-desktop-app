#ifndef PROFILEPAGEFORCANDIDATE_H
#define PROFILEPAGEFORCANDIDATE_H

#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "resume.h"

namespace Ui {
class ProfilePageForCandidate;
}

class ProfilePageForCandidate : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePageForCandidate(
        QNetworkAccessManager *manager,
        QWidget *parent = nullptr
    );
    ~ProfilePageForCandidate();
    void setCandidateData(
        const QString &name,
        const QString &email,
        const QString &surname,
        const QString &phoneNum,
        const QString &place,
        const qint16 &search_status_id,
        const QString &faculty_of_educ,
        const QString &place_of_study,
        const qint16 &experience_status_id
    );
    void saveChangesToDB(
        const QString &newPhone,
        const QString &newPlace,
        const QString &newSearchStatus
    );
    void loadResumeData();
    void saveResumeData();
signals:
    void homeButtonClicked();
    void logoutButtonClicked();

private slots:
    void onSaveClicked();

    void on_resumeButton_clicked();

private:
    void SetupUI();
    QNetworkAccessManager *networkManager;
    Ui::ProfilePageForCandidate *ui;
};

#endif  // PROFILEPAGEFORCANDIDATE_H
