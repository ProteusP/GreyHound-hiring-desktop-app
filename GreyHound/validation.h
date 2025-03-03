#ifndef VALIDATION_H
#define VALIDATION_H

#include <QMainWindow>
#include <QRegularExpression>

bool validateEmail(const QString &email);

bool checkIfFieldsAreEmpty(
    QMainWindow *curWindow,
    const std::vector<QString> &fieldInputVec,
    QString &&message
);

#endif  // VALIDATION_H
