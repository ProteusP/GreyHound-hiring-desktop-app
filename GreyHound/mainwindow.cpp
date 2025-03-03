#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QSqlDatabase>
#include <QThread>
#include "./ui_mainwindow.h"
#include "profilewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("GreyHound");
    resize(800, 600);
    qDebug() << QSqlDatabase::drivers();
    qDebug() << "HERE";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_profilePB_clicked() {
    ProfileWindow *profileWindow = new ProfileWindow(this);
    this->hide();
    profileWindow->show();
}

void MainWindow::set_status_to_employee() {
    this->isemployee = true;
}

void MainWindow::set_status_to_candidat() {
    this->isemployee = false;
}
