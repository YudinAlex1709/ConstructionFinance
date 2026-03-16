#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <QString>
#include <QDate>
#include <QDateTime>

class DateUtils
{
public:
    static QString format(const QDate &date);
    static QString format(const QDateTime &dateTime);
    static QString formatShort(const QDate &date);
    static QString formatShort(const QDateTime &dateTime);
    static QString relative(const QDate &date);
    static QString relative(const QDateTime &dateTime);
    static QDate parse(const QString &text);
    
private:
    DateUtils() = delete;
};

#endif // DATEUTILS_H
