#include "currencyutils.h"
#include <QLocale>

QString CurrencyUtils::format(double amount, const QString &currency)
{
    QLocale locale = QLocale::system();
    QString symbol = currencySymbol(currency);
    
    QString formatted = locale.toString(amount, 'f', 2);
    
    // Add thousand separators
    int dotPos = formatted.indexOf('.');
    if (dotPos == -1) dotPos = formatted.length();
    
    for (int i = dotPos - 3; i > 0; i -= 3) {
        formatted.insert(i, ' ');
    }
    
    return formatted + " " + symbol;
}

QString CurrencyUtils::formatShort(double amount, const QString &currency)
{
    QString suffix;
    double divisor = 1.0;
    
    if (qAbs(amount) >= 1000000000.0) {
        suffix = "B";
        divisor = 1000000000.0;
    } else if (qAbs(amount) >= 1000000.0) {
        suffix = "M";
        divisor = 1000000.0;
    } else if (qAbs(amount) >= 1000.0) {
        suffix = "K";
        divisor = 1000.0;
    }
    
    double shortAmount = amount / divisor;
    QString formatted = QString::number(shortAmount, 'f', suffix.isEmpty() ? 2 : 1);
    
    return formatted + suffix + " " + currencySymbol(currency);
}

QString CurrencyUtils::currencySymbol(const QString &currency)
{
    if (currency == "RUB" || currency == "RUR") return "₽";
    if (currency == "USD") return "$";
    if (currency == "EUR") return "€";
    if (currency == "GBP") return "£";
    if (currency == "JPY") return "¥";
    if (currency == "CNY") return "¥";
    return currency;
}

double CurrencyUtils::parse(const QString &text)
{
    QString cleaned = text;
    cleaned.remove(' ');
    cleaned.remove(currencySymbol("RUB"));
    cleaned.remove(currencySymbol("USD"));
    cleaned.remove(currencySymbol("EUR"));
    cleaned.remove('$');
    cleaned.remove('€');
    cleaned.remove('₽');
    cleaned.remove('¥');
    cleaned.remove('£');
    
    bool ok;
    double result = cleaned.toDouble(&ok);
    return ok ? result : 0.0;
}
