#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "profilewindow.h"
#include <QMenu>
#include <QPushButton>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("GreyHound");
  resize(800, 600);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_profilePB_clicked() {
  ProfileWindow *profileWindow = new ProfileWindow(this);
  this->hide();
  profileWindow->show();
}
