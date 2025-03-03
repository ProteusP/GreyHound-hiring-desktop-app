#include "validation.h"
#include <QMessageBox>

bool validateEmail(const QString &email) {
    static QRegularExpression regex(
        R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})"
    );

    QRegularExpressionMatch match = regex.match(email);

    return match.hasMatch();
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
