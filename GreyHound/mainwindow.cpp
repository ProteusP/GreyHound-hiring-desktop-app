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
#include "flowlayout.h"
#include "customwidgets.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    FlowLayout *flowLayout = new FlowLayout;
    flowLayout->addWidget(new VacancyCard("senior developer python", "ноунейм1", "топ вакансия мильон в сутки"));
    flowLayout->addWidget(new VacancyCard("senior developer c++", "ноунейм2", "топ вакансия мильон в сутки"));
    flowLayout->addWidget(new VacancyCard("senior developer go", "ноунейм3", "топ вакансия мильон в сутки"));
    flowLayout->addWidget(new VacancyCard("senior developer assembler", "ноунейм4", "топ вакансия мильон в сутки"));
    flowLayout->addWidget(new VacancyCard("senior developer pascal", "ноунейм5", "топ вакансия мильон в сутки"));
    ui->groupBoxVacancies->setLayout(flowLayout);
    setWindowTitle("GreyHound");
    resize(800, 600);
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
