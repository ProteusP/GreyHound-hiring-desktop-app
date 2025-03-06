#include "profilewindowforemployer.h"
#include "ui_profilewindowforemployer.h"

ProfileWindowForEmployer::ProfileWindowForEmployer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ProfileWindowForEmployer) {
    ui->setupUi(this);
}

ProfileWindowForEmployer::~ProfileWindowForEmployer() {
    delete ui;
}
