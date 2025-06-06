#ifndef VACANCYCARD_H
#define VACANCYCARD_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class VacancyCard : public QFrame {
    Q_OBJECT
public:
    explicit VacancyCard(
        const QString &title,
        const QString &company,
        const QString &description,
        QWidget *parent = nullptr
    );

    void setFixedSize(int width, int height);
    void setVacancyId(int id);  // Переопределяем для внутренней настройки

signals:
    void detailsRequested(int vacancyId);

private:
    int vacancyId = -1;
    QLabel *titleLabel;
    QLabel *companyLabel;
    QLabel *descLabel;
    QPushButton *detailsButton;
};

#endif  // VACANCYCARD_H
