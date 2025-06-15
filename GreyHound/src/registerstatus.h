#ifndef REGISTERSTATUS_H
#define REGISTERSTATUS_H

#include <QWidget>

namespace Ui {
class RegisterStatus;
}

class RegisterStatus : public QWidget {
    Q_OBJECT

  public:
    explicit RegisterStatus(QWidget *parent = nullptr);
    ~RegisterStatus();

  private slots:
    void on_backToLoginPB_clicked();

    void on_statusCandidatePB_clicked();

    void on_statusEmployerPB_clicked();

  private:
    Ui::RegisterStatus *ui;

  signals:
    void backToLoginPressed();
    void statusCandidatePressed();
    void statusEmployerPressed();
};

#endif // REGISTERSTATUS_H
