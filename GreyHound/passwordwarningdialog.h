#ifndef PASSWORDWARNINGDIALOG_H
#define PASSWORDWARNINGDIALOG_H

#include <QDialog>

class PasswordWarningDialog : public QDialog {
    Q_OBJECT

public:
    explicit PasswordWarningDialog(QWidget *parent = nullptr);
};

#endif  // PASSWORDWARNINGDIALOG_H
