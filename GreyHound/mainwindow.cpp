#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("GreyHound");
    resize(800,600);



}

MainWindow::~MainWindow()
{
    delete ui;
}
