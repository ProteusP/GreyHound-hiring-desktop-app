#ifndef REGISTERWINDOWFORCANDIDATE_H
#define REGISTERWINDOWFORCANDIDATE_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "mainwindow.h"
#include "mainwindow.h"


namespace Ui {
class RegisterWindowForCandidate;
}

class RegisterWindowForCandidate : public QMainWindow {
    Q_OBJECT

public:
    explicit RegisterWindowForCandidate(MainWindow *mainWindow_,
                                        QWidget *parent = nullptr);
    ~RegisterWindowForCandidate();

private slots:
    void on_PBregistrationCandidate_clicked();

    void on_backToStatusPB_clicked();

private:
    Ui::RegisterWindowForCandidate *ui;
    MainWindow *mainWindow;
    QWidget *parentStatus;
};

#endif // REGISTERWINDOWFORCANDIDATE_H
