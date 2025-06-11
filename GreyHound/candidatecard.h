#ifndef CANDIDATECARD_H
#define CANDIDATECARD_H

#include <QFrame>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QVBoxLayout>

class candidateCard : public QFrame {
    Q_OBJECT
public:
    explicit candidateCard(
        QNetworkAccessManager *manager,
        const QString &placeOfStudy_,
        const QString &facultyOfEducation_,
        const QString &experience_,
        const int &id,
        QWidget *parent = nullptr
    );

    void setFixedSize(int width, int height);

signals:
    void detailsRequested(int vacancyId);

private slots:
    void on_detailsButton_clicked();

private:
    QNetworkAccessManager *networkManager;
    QLabel *placeOfStudy;
    QLabel *facultyOfEducation;
    QLabel *experience;
    int user_id;
    QPushButton *detailsButton;
};

#endif  // CANDIDATECARD_H
