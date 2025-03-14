#include "profilepageforcandidate.h"
#include "ui_profilepageforcandidate.h"

ProfilePageForCandidate::ProfilePageForCandidate(QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfilePageForCandidate) {
  ui->setupUi(this);
}

ProfilePageForCandidate::~ProfilePageForCandidate() { delete ui; }
