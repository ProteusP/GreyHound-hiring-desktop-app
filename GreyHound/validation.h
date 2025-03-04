#ifndef VALIDATION_H
#define VALIDATION_H

#include <QMainWindow>
#include <QRegularExpression>

bool isEmailValid(const QString &email);

bool isPasswordValid(const QString &password);

bool checkIfFieldsAreEmpty(
    QMainWindow *curWindow,
    const std::vector<QString> &fieldInputVec,
    QString &&message
);

#endif  // VALIDATION_H
