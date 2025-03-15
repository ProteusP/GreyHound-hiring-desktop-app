#ifndef PROFILEPAGEFORCANDIDATE_H
#define PROFILEPAGEFORCANDIDATE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QString>
#include <QFormLayout>
#include <QPushButton>


namespace Ui {
class ProfilePageForCandidate;
}

class ProfilePageForCandidate : public QWidget {
  Q_OBJECT

public:
  explicit ProfilePageForCandidate(QWidget *parent = nullptr);
  ~ProfilePageForCandidate();
  void updateUserData(const QString& name, const QString& email, const QString& surname);

private:
  void SetupUI();
  QLabel *nameLabel;
  QLabel *emailLabel;
  QLabel *surnameLabel;


};

#endif // PROFILEPAGEFORCANDIDATE_H
