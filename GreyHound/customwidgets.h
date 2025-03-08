#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
class VacancyCard : public QFrame{
    Q_OBJECT
public:
    explicit VacancyCard(const QString &titleLabel_, const QString &companyLabel_, const QString &descLabel_, QWidget *parent = nullptr);
};

#endif  // CUSTOMWIDGETS_H
