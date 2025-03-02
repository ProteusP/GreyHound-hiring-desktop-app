#include "registerstatus.h"
#include "mainwindow.h"
#include "registerwindowforcandidate.h"
#include "registerwindowforemployer.h"
#include "ui_registerstatus.h"

RegisterStatus::RegisterStatus(QMainWindow *mainWindow_, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RegisterStatus), mainWindow(mainWindow_) {
  ui->setupUi(this);
  this->resize(500, 300);
  this->setWindowTitle("Register status window");
}

RegisterStatus::~RegisterStatus() { delete ui; }

void RegisterStatus::on_statusCandidatePB_clicked() {
  RegisterWindowForCandidate *registerWindowForCandidate =
      new RegisterWindowForCandidate(mainWindow);
  this->hide();
  registerWindowForCandidate->show();
}

void RegisterStatus::on_statusEmployerPB_clicked() {
  RegisterWindowForEmployer *registerWindowForEmployer =
      new RegisterWindowForEmployer(mainWindow);
  this->close();
  registerWindowForEmployer->show();
}
