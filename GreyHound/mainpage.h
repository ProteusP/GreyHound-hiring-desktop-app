#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDebug>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "flowlayout.h"
#include "ui_mainpage.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(
        QNetworkAccessManager *manager,
        QWidget *parent = nullptr
    );
    ~MainPage();

    void show(bool iseemployee);

    void setStatusOfCandidate(bool status_);

    void hide();

    void setEmail(QString email_);

    int getCurrentCandidateId();

    QWidget *createCandidatesPage(int numberPage);

    FlowLayout *getFlowLayout();

private:
    QNetworkAccessManager *networkManager;
    bool isCandidate;
    QString email;
    FlowLayout *flow_layout;
    void addFormField(
        QLayout *layout,
        const QString &fieldName,
        const QString &fieldValue,
        const QString &fieldStyle = "",
        const QString &valueStyle = ""
    );
    QMap<int, QWidget *> pageCache;
    int currentPage;
    Ui::MainPage *ui;

private slots:
    void on_profilePB_3_clicked();
    void showVacancyDetails(int vacancyId);
    void respondToVacancy(int vacancyId);

    void on_pushButtonNext_clicked();

    void on_pushButtonPrev_clicked();

signals:
    void onProfilePressed();
};

#endif  // MAINPAGE_H
