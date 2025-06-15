#ifndef VALIDATION_H
#define VALIDATION_H

#include <QRegularExpression>

bool isEmailValid(const QString &email);

bool isPasswordValid(const QString &password);

#endif  // VALIDATION_H
