#ifndef HASHING_H
#define HASHING_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

QString hashPassword(const QString &password);

#endif // HASHING_H
