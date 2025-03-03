#ifndef REGISTERWINDOWFOREMPLOYER_H
#define REGISTERWINDOWFOREMPLOYER_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "mainwindow.h"

namespace Ui {
class RegisterWindowForEmployer;
}

class RegisterWindowForEmployer : public QMainWindow {
    Q_OBJECT

public:
    explicit RegisterWindowForEmployer(
        MainWindow *mainWindow_,
        QWidget *parent = nullptr
    );
    ~RegisterWindowForEmployer();

private slots:
    void on_registrationPB_employer_clicked();

    void on_backToStatusPB_clicked();

private:
    Ui::RegisterWindowForEmployer *ui;
    MainWindow *mainWindow;
    QWidget *parentStatus;
};

#endif  // REGISTERWINDOWFOREMPLOYER_H
