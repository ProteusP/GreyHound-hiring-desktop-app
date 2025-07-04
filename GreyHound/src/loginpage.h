#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT

  public:
    explicit LoginWidget(QNetworkAccessManager *manager,
                         QWidget *parent = nullptr);
    ~LoginWidget();

  private slots:
    void on_loginPB_clicked();

    void on_registerPB_clicked();

  private:
    QNetworkAccessManager *networkManager;
    Ui::LoginWidget *ui;

  signals:
    void loginSuccessful(bool isemployee = false);
    void registerPressed();
};

#endif // LOGINPAGE_H
