#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow *mainWindow = new MainWindow;
    LoginWindow *loginWindow = new LoginWindow(mainWindow);
    loginWindow->show();
    return a.exec();
}
