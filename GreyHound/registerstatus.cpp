#include "registerstatus.h"
#include "mainwindow.h"
#include "registerwindowforcandidate.h"
#include "registerwindowforemployer.h"
#include "ui_registerstatus.h"

RegisterStatus::RegisterStatus(MainWindow *mainWindow_, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RegisterStatus), mainWindow(mainWindow_), parentLogin(parent){
  ui->setupUi(this);
  this->resize(500, 300);
  this->setWindowTitle("Register status window");
}

RegisterStatus::~RegisterStatus() { delete ui; }

void RegisterStatus::on_statusCandidatePB_clicked() {
  RegisterWindowForCandidate *registerWindowForCandidate =
      new RegisterWindowForCandidate(mainWindow, this);

  mainWindow->set_status_to_candidat();

  this->close();
  registerWindowForCandidate->show();
}

void RegisterStatus::on_statusEmployerPB_clicked() {
  RegisterWindowForEmployer *registerWindowForEmployer =
      new RegisterWindowForEmployer(mainWindow, this);

  mainWindow->set_status_to_employee();

  this->close();
  registerWindowForEmployer->show();
}

void RegisterStatus::on_backToLoginPB_clicked()
{
    this->hide();
    parentLogin->show();
}

