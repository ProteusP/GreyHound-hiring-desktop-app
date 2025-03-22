#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDebug>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "customwidgets.h"
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

    FlowLayout *getFlowLayout();

private:
    Ui::MainPage *ui;
    bool isCandidate;
    FlowLayout *flow_layout;

private slots:
    void on_profilePB_3_clicked();

signals:
    void onProfilePressed();
};

#endif  // MAINPAGE_H
