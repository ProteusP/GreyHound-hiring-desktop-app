#include "profilepageforemployer.h"
#include "ui_profilepageforemployer.h"

ProfilePageForEmployer::ProfilePageForEmployer(QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfilePageForEmployer) {
    ui->setupUi(this);
}

ProfilePageForEmployer::~ProfilePageForEmployer() {
    delete ui;
}
