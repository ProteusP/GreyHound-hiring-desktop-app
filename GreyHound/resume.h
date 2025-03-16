#ifndef RESUME_H
#define RESUME_H

#include <QString>

class Resume {
public:
    QString university;
    QString faculty;
    QString experienceYears;

    Resume() : experienceYears("Нет") {
    }
};

#endif  // RESUME_H
