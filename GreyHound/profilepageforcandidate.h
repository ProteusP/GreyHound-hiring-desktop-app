#ifndef PROFILEPAGEFORCANDIDATE_H
#define PROFILEPAGEFORCANDIDATE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QString>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
namespace Ui {
class ProfilePageForCandidate;
}

class ProfilePageForCandidate : public QWidget {
  Q_OBJECT

public:
  explicit ProfilePageForCandidate(QWidget *parent = nullptr);
  ~ProfilePageForCandidate();
  void updateUserData(const QString& name, const QString& email, const QString& surname);
  void saveChangesToDB(const QString& newPhone);
  signals:
  void homeButtonClicked();
    void logoutButtonClicked();


private slots:
      void onSaveClicked();

private:
  void SetupUI();
  QLabel *nameLabel;
  QLabel *emailLabel;
  QLabel *surnameLabel;
  QLineEdit *phoneEdit;

};

#endif // PROFILEPAGEFORCANDIDATE_H
