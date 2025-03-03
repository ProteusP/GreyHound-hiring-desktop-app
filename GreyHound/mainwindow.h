#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_status_to_employee();
    void set_status_to_candidat();

private slots:
    void on_profilePB_clicked();

private:
    Ui::MainWindow *ui;
    bool isemployee;
};
#endif  // MAINWINDOW_H
