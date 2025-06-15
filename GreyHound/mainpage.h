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

    void deleteData() {
        currentPage = 0;
        pageCache.clear();
    }
    void createEmplFilters();
    void createCandFilters();
    QMap<QString, QWidget*> filterInputs;
    QMap<QString, QWidget*> candFilterInputs;

    QWidget *createCandidatesPage(int numberPage);
    QWidget *createCandidatesPageWithFilters(const QMap<QString, QVariant> &filters);
    QWidget *createVacanciesPageWithFilters(const QMap<QString, QVariant> &filters);
    QWidget *createVacanciesPage(int numberPage);

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
    // void showVacancyDetails(int vacancy_id);
    // void respondToVacancy(int vacancy_id);
    void on_pushButtonNext_clicked();
    void on_pushButtonPrev_clicked();

signals:
    void onProfilePressed();
};

#endif  // MAINPAGE_H
