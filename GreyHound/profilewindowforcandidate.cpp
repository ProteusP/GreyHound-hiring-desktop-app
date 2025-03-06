#include "profilewindowforcandidate.h"
#include "ui_profilewindowforcandidate.h"

ProfileWindowForCandidate::ProfileWindowForCandidate(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ProfileWindowForCandidate) {
    ui->setupUi(this);
}

ProfileWindowForCandidate::~ProfileWindowForCandidate() {
    delete ui;
}
