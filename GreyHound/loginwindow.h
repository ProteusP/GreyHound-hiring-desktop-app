#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QMainWindow *mainWindow_,QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_exitPB_clicked();

    void on_loginPB_clicked();

    void on_registerPB_clicked();

private:
    Ui::LoginWindow *ui;
    QMainWindow *mainWindow;
};

#endif // LOGINWINDOW_H
