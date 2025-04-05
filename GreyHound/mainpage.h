#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDebug>
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
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setStatusOfCandidate(bool status_);

    void show();

    void hide();

    void setEmail(QString email_);

    int getCurrentCandidateId();

    FlowLayout *getFlowLayout();

private:
    Ui::MainPage *ui;
    bool isCandidate;
    QString email;
    FlowLayout *flow_layout;
    void addFormField(QLayout *layout, const QString &fieldName,
                      const QString &fieldValue,
                      const QString &fieldStyle = "",
                      const QString &valueStyle = "");

private slots:
    void on_profilePB_3_clicked();
    void showVacancyDetails(int vacancyId);
    void respondToVacancy(int vacancyId);

signals:
    void onProfilePressed();
};

#endif  // MAINPAGE_H
