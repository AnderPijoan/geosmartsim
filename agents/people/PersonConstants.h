#ifndef PERSONCONSTANTS_H
#define PERSONCONSTANTS_H

#include <QObject>
#include <QDateTime>

class PersonConstants : public QObject
{
    Q_OBJECT
public:
    enum gender {male, female};
    enum age_type {young, adult, elder};

    static QDateTime createBornDate(PersonConstants::age_type type_age){
        QDateTime age;
        int current_year = QDateTime::currentDateTime().date().year();
        switch(type_age){
        case young: { // 0 - 19
            int rand = qrand() % 20;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        case adult: // 20 - 64
        default: {
            int rand = (qrand() % 45) + 20;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        case elder: { // 65 - 100
            int rand = (qrand() % 35) + 65;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        }
    }


};


#endif // PERSONCONSTANTS_H

