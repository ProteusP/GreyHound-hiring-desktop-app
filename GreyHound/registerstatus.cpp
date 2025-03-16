#include "registerstatus.h"
#include "ui_registerstatus.h"

RegisterStatus::RegisterStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterStatus) {
    ui->setupUi(this);
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
