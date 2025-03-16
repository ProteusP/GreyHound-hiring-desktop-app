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
#include <QComboBox>
#include "resume.h"

namespace Ui {
class ProfilePageForCandidate;
}

class ProfilePageForCandidate : public QWidget {
  Q_OBJECT

public:
  explicit ProfilePageForCandidate(QWidget *parent = nullptr);
  ~ProfilePageForCandidate();
  void updateUserData(const QString& name, const QString& email, const QString& surname,const QString& phoneNum, const QString& place);
  void saveChangesToDB(const QString& newPhone, const QString& newPlace);
  void loadResumeData();
  void saveResumeData();
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
  QComboBox *statusCombo;
  QLineEdit *placeEdit;
    //fix it after adding an output for resumes
  //Resume currentResume;
  QLineEdit *universityEdit;
  QLineEdit *facultyEdit;
  QComboBox *experinceCombo;

};

#endif // PROFILEPAGEFORCANDIDATE_H
