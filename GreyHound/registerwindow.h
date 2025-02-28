#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QMainWindow *mainWindow_, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_registerPB_clicked();

private:
    Ui::RegisterWindow *ui;
    QMainWindow *mainWindow;
};

#endif // REGISTERWINDOW_H
