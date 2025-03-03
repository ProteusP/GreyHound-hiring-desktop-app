#include "profilewindow.h"
#include "mainwindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(MainWindow *mainWindow_, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ProfileWindow), mainWindow(mainWindow_) {
    ui->setupUi(this);
    this->setWindowTitle("Profile window");
    this->resize(800, 600);
}

ProfileWindow::~ProfileWindow() {
    delete ui;
}

void ProfileWindow::on_homePB_clicked() {
    this->hide();
    this->mainWindow->show();
}
