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
    resize(1920,1080);

    QPushButton *menu_button = new QPushButton(this);
    menu_button->setText("Menu or filter");
    //menu_button->setFont(QFont("Sanserif", 12));
    menu_button->setStyleSheet("background-color:grey");
    menu_button->setGeometry(0,0,100,50);

    //popup for menu button ^ (maybe it`s gonna be a filter idk)
    QMenu *menu = new QMenu();
    //menu->setFont(QFont("Sanserif", 10));
    menu->setStyleSheet("background-color:grey");
    menu->addAction("Category_1");
    menu->addAction("Category_2");
    menu->addAction("Category_3");

    menu_button->setMenu(menu);



    QPushButton *user_account_button = new QPushButton(this);
    //need to add some picture here...
    user_account_button->setText("My account");
    //user_account_button->setFont(QFont("Sanserif", 12));
    user_account_button->setStyleSheet("background-color:grey");
    user_account_button->setGeometry(1500,0,100,50);

}

MainWindow::~MainWindow()
{
    delete ui;
}
