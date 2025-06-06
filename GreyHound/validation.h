#ifndef VALIDATION_H
#define VALIDATION_H

#include <QRegularExpression>
#include <QWidget>

bool isEmailValid(const QString &email);

bool isPasswordValid(const QString &password);

#endif  // VALIDATION_H
