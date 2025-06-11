#include "candidatedetailswindow.h"
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QUrl>
#include "ui_candidatedetailswindow.h"

candidateDetailsWindow::candidateDetailsWindow(
    QNetworkAccessManager *manager,
    const int &user_id_,
    QWidget *parent
)
    : QDialog(parent),
      ui(new Ui::candidateDetailsWindow),
      networkManager(manager),
      user_id(user_id_) {
    ui->setupUi(this);
}

candidateDetailsWindow::~candidateDetailsWindow() {
    delete ui;
}

void candidateDetailsWindow::on_exitButton_clicked() {
    this->close();
}

void candidateDetailsWindow::on_resumeButton_clicked() {
    QString url =
        QString("http://127.0.0.1:80/api/v1/resources/resume/%1").arg(user_id);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray pdfData = reply->readAll();
            QFile file("resume.pdf");
            if (file.open(QIODevice::WriteOnly)) {
                QTemporaryFile tempFile;
                tempFile.setAutoRemove(false);
                tempFile.open();
                tempFile.write(pdfData);
                tempFile.close();
                QDesktopServices::openUrl(QUrl::fromLocalFile(tempFile.fileName(
                )));
            }
        } else {
            QMessageBox::warning(this, "Упс...", "Ничего не найдено.");
        }
        reply->deleteLater();
    });
}
