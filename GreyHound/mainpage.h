#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "customwidgets.h"
#include "flowlayout.h"
#include "ui_mainpage.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>

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

private:
    Ui::MainPage *ui;
    bool isCandidate;

private slots:
    void on_profilePB_3_clicked();

signals:
    void onProfilePressed();
};

#endif  // MAINPAGE_H
