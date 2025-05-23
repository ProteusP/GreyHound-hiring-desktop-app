#ifndef REGISTERPAGEFOREMPLOYER_H
#define REGISTERPAGEFOREMPLOYER_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWidget>

namespace Ui {
class RegisterPageForEmployer;
}

class RegisterPageForEmployer : public QWidget {
    Q_OBJECT

public:
    explicit RegisterPageForEmployer(QWidget *parent = nullptr);
    ~RegisterPageForEmployer();

private slots:
    void on_registrationPB_employer_clicked();

    void on_backToStatusPB_clicked();

private:
    Ui::RegisterPageForEmployer *ui;

signals:
    void registerSuccessful(const QString &email, bool status);
    void backToRegisterStatusPagePressed();
};

#endif  // REGISTERPAGEFOREMPLOYER_H
