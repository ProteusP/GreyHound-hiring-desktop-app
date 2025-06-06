#include "registerstatus.h"
#include <QGraphicsDropShadowEffect>
#include "ui_registerstatus.h"

RegisterStatus::RegisterStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterStatus) {
    ui->setupUi(this);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 50));
    ui->widget->setGraphicsEffect(shadow);
}

RegisterStatus::~RegisterStatus() {
    delete ui;
}

void RegisterStatus::on_backToLoginPB_clicked() {
    emit backToLoginPressed();
}

void RegisterStatus::on_statusCandidatePB_clicked() {
    emit statusCandidatePressed();
}

void RegisterStatus::on_statusEmployerPB_clicked() {
    emit statusEmployerPressed();
}
