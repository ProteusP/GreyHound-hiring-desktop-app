#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QSqlDatabase>
#include <QThread>
#include "./ui_mainwindow.h"
//
#include "profilewindow.h"
//
#include "profilewindowforcandidate.h"
#include "profilewindowforemployer.h"

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

    if (isemployee){
        auto myProfile = new ProfileWindowForEmployer(this);
        this->hide();
        myProfile->show();
    }
    else{
        auto myProfile = new ProfileWindowForCandidate(this);
        this->hide();
        myProfile->show();
    }

}

void MainWindow::set_status_to_employee() {
    this->isemployee = true;
}

void MainWindow::set_status_to_candidat() {
    this->isemployee = false;
}
