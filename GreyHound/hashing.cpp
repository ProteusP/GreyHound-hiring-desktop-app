#include "hashing.h"

QString hashPassword(const QString &password) {
  QByteArray hashed =
      QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
  return hashed.toHex();
}
