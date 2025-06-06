#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(
        QNetworkAccessManager *manager,
        QWidget *parent = nullptr
    );
    ~LoginWidget();

private slots:
    void on_loginPB_clicked();

    void on_registerPB_clicked();

private:
    QNetworkAccessManager *networkManager;
    Ui::LoginWidget *ui;

signals:
    void loginSuccessful();
    void registerPressed();
};

#endif  // LOGINPAGE_H
