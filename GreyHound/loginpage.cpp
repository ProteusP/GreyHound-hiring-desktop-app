#include "loginpage.h"
#include <QRegularExpression>
#include "hashing.h"
#include "ui_loginpage.h"
// #include "registerstatus.h"
#include "validation.h"
#include <QGraphicsDropShadowEffect>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWidget) {
    ui->setupUi(this);

    // Создаем эффект тени
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));

    // Применяем к виджету
    ui->widget->setGraphicsEffect(shadow);
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_loginPB_clicked() {
    QString userMail = ui->mailLine->text();
    QString userPassword = ui->passwordLine->text();
    bool userIsEmployee = false;
    QString userStatus = ui->statusComboBox->currentText();

    QVector<QString> fieldInputVec = {userMail, userPassword};
    if (fieldInputVec[0].isEmpty() || fieldInputVec[0].isEmpty()) {
        QMessageBox::warning(
            this, "Ошибка", "Никакое поле не должно быть пустым"
        );
        return;
    }
    if (isEmailValid(userMail)) {
        QSqlQuery query;
        if (userStatus == "Кандидат") {
            query.prepare(
                "SELECT password FROM candidates WHERE email = :email "
            );
            userIsEmployee = true;
        } else {
            query.prepare("SELECT password FROM employers WHERE email = :email"
            );
        }

        query.bindValue(":email", userMail);
        if (!query.exec()) {
            QMessageBox::warning(
                this, "Ошибка", "Ошибка при выполнении запроса к базе данных."
            );
            return;
        }

        if (query.next()) {
            QString dbPassword = query.value(0).toString();

            if (dbPassword == hashPassword(userPassword)) {
                emit loginSuccessful(userMail, userIsEmployee);
            } else {
                QMessageBox::warning(this, "Упс...", "Неверный пароль.");
            }
        } else {
            QMessageBox::warning(
                this, "Упс...", "Пользователь с такой почтой не найден."
            );
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите корректную почту");
    }
}

void LoginWidget::on_registerPB_clicked() {
    emit registerPressed();
}

void LoginWidget::on_exitPB_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Выход", "Вы точно хотите выйти?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::No) {
        return;
    }

    QApplication::exit();
}
