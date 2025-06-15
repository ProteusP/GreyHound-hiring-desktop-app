#ifndef VACANCYCARD_H
#define VACANCYCARD_H

#include <QFrame>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QVBoxLayout>

class VacancyCard : public QFrame {
    Q_OBJECT
  public:
    explicit VacancyCard(QNetworkAccessManager *manager_, const QString &title_,
                         const QString &company_, const QString &salary_,
                         const QString &place_, const int &workSchedule_,
                         const int &remoteness_, const int &vacancy_id_,
                         QWidget *parent = nullptr);

    void setFixedSize(int width, int height);
    void setVacancyId(int id); // Переопределяем для внутренней настройки

  private slots:
    void on_detailsButton_clicked();

  private:
    QNetworkAccessManager *networkManager;
    QLabel *title;
    QLabel *company;
    QLabel *salary;
    QLabel *place;
    const QString titleString;
    const QString companyString;
    const QString salaryString;
    const QString placeString;
    const int workSchedule;
    const int remoteness;
    const int vacancy_id;
    QPushButton *detailsButton;
};

#endif // VACANCYCARD_H
