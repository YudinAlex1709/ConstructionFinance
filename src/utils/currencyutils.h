#ifndef CURRENCYUTILS_H
#define CURRENCYUTILS_H

#include <QString>
#include <QLocale>

class CurrencyUtils
{
public:
    static QString format(double amount, const QString &currency = "RUB");
    static QString formatShort(double amount, const QString &currency = "RUB");
    static QString currencySymbol(const QString &currency);
    static double parse(const QString &text);
    
private:
    CurrencyUtils() = delete;
};

#endif // CURRENCYUTILS_H
