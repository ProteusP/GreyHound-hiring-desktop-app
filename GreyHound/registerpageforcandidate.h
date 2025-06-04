#ifndef REGISTERWINDOWFORCANDIDATE_H
#define REGISTERWINDOWFORCANDIDATE_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWidget>
#include <QNetworkAccessManager>

namespace Ui {
class RegisterPageForCandidate;
}

class RegisterPageForCandidate : public QWidget {
    Q_OBJECT

public:
    explicit RegisterPageForCandidate(QNetworkAccessManager* manager, QWidget *parent = nullptr);
    ~RegisterPageForCandidate();

private slots:
    void on_PBregistrationCandidate_clicked();

    void on_backToStatusPB_clicked();

private:
    Ui::RegisterPageForCandidate *ui;
    QNetworkAccessManager* networkManager;

signals:
    void registerSuccessful();
    void backToRegisterStatusPagePressed();
};

#endif  // REGISTERWINDOWFORCANDIDATE_H
