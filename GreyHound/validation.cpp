#include "validation.h"
#include <QMessageBox>

bool isEmailValid(const QString &email) {
    static QRegularExpression regex(
        R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})"
    );

    QRegularExpressionMatch match = regex.match(email);

    return match.hasMatch();
}

bool isPasswordValid(const QString &password) {
    static QRegularExpression regex(
        "^(?=.*[A-Z])(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?])"
        "(?=.*[a-z])(?=.*\\d)[A-Za-z\\d!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>"
        "\\/?]{8,}$"
    );

    return regex.match(password).hasMatch();
}

bool checkIfFieldsAreEmpty(
    QMainWindow *curWindow,
    const std::vector<QString> &fieldInputVec,
    QString &&message
) {
    for (auto &fieldInput : fieldInputVec) {
        if (fieldInput.isEmpty()) {
            QMessageBox::warning(curWindow, "Ошибка", message);
            return true;
        }
    }
    return false;
}
