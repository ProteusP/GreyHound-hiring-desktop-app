#ifndef CANDIDATEDETAILSWINDOW_H
#define CANDIDATEDETAILSWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>

namespace Ui {
class candidateDetailsWindow;
}

class candidateDetailsWindow : public QDialog {
    Q_OBJECT

public:
    explicit candidateDetailsWindow(
        QNetworkAccessManager *manager,
        const int &user_id_,
        QWidget *parent = nullptr
    );
    ~candidateDetailsWindow();

private slots:
    void on_exitButton_clicked();

    void on_resumeButton_clicked();

private:
    Ui::candidateDetailsWindow *ui;
    QNetworkAccessManager *networkManager;
    const int user_id;
};

#endif  // CANDIDATEDETAILSWINDOW_H
