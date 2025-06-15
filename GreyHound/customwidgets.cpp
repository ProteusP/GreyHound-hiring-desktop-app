#include "customwidgets.h"

VacancyCard::VacancyCard(const QString &titleLabel_,
                         const QString &companyLabel_,
                         const QString &descLabel_, QWidget *parent)
    : QFrame(parent) {
    QLabel *titleLabel = new QLabel(titleLabel_, this);
    QLabel *companyLabel = new QLabel(companyLabel_, this);
    QLabel *descLabel = new QLabel(descLabel_, this);
    this->setFrameShape(QFrame::Box);
    this->setLineWidth(2);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(companyLabel);
    layout->addWidget(descLabel);
    setLayout(layout);
}
