#include "passwordwarningdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PasswordWarningDialog::PasswordWarningDialog(QWidget *parent)
    : QDialog(parent) {
  setWindowTitle("Ошибка пароля");
  setFixedSize(300, 200);
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *label = new QLabel("Пароль должен содержать:\n"
                             "• Минимум 8 символов\n"
                             "• Хотя бы одну заглавную букву\n"
                             "• Хотя бы одну строчную букву\n"
                             "• Хотя бы одну цифру\n"
                             "• Хотя бы один спец. символ",
                             this);
  label->setStyleSheet("color: red; font-size: 12px;");

  QPushButton *okButton = new QPushButton("ОК", this);
  connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

  layout->addWidget(label);
  layout->addWidget(okButton);
}
