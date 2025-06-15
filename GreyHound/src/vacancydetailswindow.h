#ifndef VACANCYDETAILSWINDOW_H
#define VACANCYDETAILSWINDOW_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ui {
class vacancyDetailsWindow;
}

class vacancyDetailsWindow : public QDialog {
    Q_OBJECT

  public:
    explicit vacancyDetailsWindow(
        QNetworkAccessManager *manager_, const QString &title_,
        const QString &company_, const QString &salary_, const QString &place_,
        const int &workSchedule_, const int &remoteness_,
        const int &vacancy_id_, QWidget *parent = nullptr);
    ~vacancyDetailsWindow();

  private slots:
    void on_exitButton_clicked();
    void on_pushButton_clicked();

  private:
    QNetworkAccessManager *networkManager;
    const int vacancy_id;
    Ui::vacancyDetailsWindow *ui;
};

#endif // VACANCYDETAILSWINDOW_H
