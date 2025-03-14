#include "registerpageforemployer.h"
#include "hashing.h"
#include "passwordwarningdialog.h"
#include "ui_registerpageforemployer.h"
#include "validation.h"

RegisterPageForEmployer::RegisterPageForEmployer(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegisterPageForEmployer) {
  ui->setupUi(this);
}

RegisterPageForEmployer::~RegisterPageForEmployer() { delete ui; }

void RegisterPageForEmployer::on_registrationPB_employer_clicked() {
  QString company_name = ui->registrationLineEditCompanyNameEmployer->text();
  QString email = ui->registrationLineEditEmailEmployer->text();
  QString password = ui->registrationLineEditPasswordEmployer->text();

  std::vector<QString> fieldInputVec = {company_name, email, password};
  if (fieldInputVec[0].isEmpty() || fieldInputVec[0].isEmpty()) {
    QMessageBox::warning(this, "Ошибка", "Никакое поле не должно быть пустым");
    return;
  }

  if (!isPasswordValid(password)) {
    PasswordWarningDialog dialog(this);
    dialog.exec();
    return;
  }

  QSqlQuery checkQuery;
  checkQuery.prepare("SELECT COUNT(*) FROM candidates WHERE email = :email");
  checkQuery.bindValue(":email", email);

  if (!checkQuery.exec()) {
    QMessageBox::warning(this, "Ошибка",
                         "Ошибка при выполнении запроса к базе данных.");
    return;
  }

  if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
    QMessageBox::warning(this, "Ошибка", "Этот email уже зарегистрирован.");
    return;
  }

  QString hashedPassword = hashPassword(password);

  if (isEmailValid(email)) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO employers (email, company_name, password) VALUES "
        "(:email, :company_name, :password)");
    query.bindValue(":email", email);
    query.bindValue(":company_name", company_name);
    query.bindValue(":password", hashedPassword);
    query.exec();

    emit registerSuccessful();
  } else {
    QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
  }
}

void RegisterPageForEmployer::on_backToStatusPB_clicked() {
  emit backToRegisterStatusPagePressed();
}
