#include "dateutils.h"
#include <QLocale>

QString DateUtils::format(const QDate &date)
{
    if (!date.isValid()) return QString();
    return date.toString("dd.MM.yyyy");
}

QString DateUtils::format(const QDateTime &dateTime)
{
    if (!dateTime.isValid()) return QString();
    return dateTime.toString("dd.MM.yyyy hh:mm");
}

QString DateUtils::formatShort(const QDate &date)
{
    if (!date.isValid()) return QString();
    
    QDate today = QDate::currentDate();
    if (date == today) {
        return QObject::tr("Сегодня");
    }
    if (date == today.addDays(-1)) {
        return QObject::tr("Вчера");
    }
    if (date == today.addDays(1)) {
        return QObject::tr("Завтра");
    }
    
    return date.toString("dd.MM.yy");
}

QString DateUtils::formatShort(const QDateTime &dateTime)
{
    if (!dateTime.isValid()) return QString();
    return formatShort(dateTime.date()) + " " + dateTime.toString("hh:mm");
}

QString DateUtils::relative(const QDate &date)
{
    if (!date.isValid()) return QString();
    
    int days = QDate::currentDate().daysTo(date);
    
    if (days == 0) return QObject::tr("Сегодня");
    if (days == -1) return QObject::tr("Вчера");
    if (days == 1) return QObject::tr("Завтра");
    if (days < -1 && days > -7) return QObject::tr("%1 дней назад").arg(-days);
    if (days > 1 && days < 7) return QObject::tr("Через %1 дней").arg(days);
    
    return format(date);
}

QString DateUtils::relative(const QDateTime &dateTime)
{
    if (!dateTime.isValid()) return QString();
    
    int secs = dateTime.secsTo(QDateTime::currentDateTime());
    
    if (secs < 60) return QObject::tr("Только что");
    if (secs < 3600) return QObject::tr("%1 минут назад").arg(secs / 60);
    if (secs < 86400) return QObject::tr("%1 часов назад").arg(secs / 3600);
    
    return relative(dateTime.date());
}

QDate DateUtils::parse(const QString &text)
{
    QStringList formats = {
        "dd.MM.yyyy",
        "dd/MM/yyyy",
        "dd-MM-yyyy",
        "yyyy-MM-dd",
        "yyyy.MM.dd",
        "ddMMyyyy"
    };
    
    for (const QString &format : formats) {
        QDate date = QDate::fromString(text, format);
        if (date.isValid()) return date;
    }
    
    return QDate();
}
