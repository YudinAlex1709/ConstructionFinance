#include "settings.h"
#include "constants.h"
#include <QDebug>

Settings* Settings::s_instance = nullptr;

Settings* Settings::instance()
{
    if (!s_instance) {
        s_instance = new Settings();
    }
    return s_instance;
}

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_settings(Constants::APP_ORGANIZATION, Constants::APP_NAME)
{
}

QString Settings::language() const
{
    return m_settings.value("language", "ru").toString();
}

void Settings::setLanguage(const QString &language)
{
    if (this->language() != language) {
        m_settings.setValue("language", language);
        emit languageChanged();
        emit settingChanged("language");
    }
}

QString Settings::theme() const
{
    return m_settings.value("theme", "light").toString();
}

void Settings::setTheme(const QString &theme)
{
    if (this->theme() != theme) {
        m_settings.setValue("theme", theme);
        emit themeChanged();
        emit settingChanged("theme");
    }
}

QString Settings::currency() const
{
    return m_settings.value("currency", "RUB").toString();
}

void Settings::setCurrency(const QString &currency)
{
    if (this->currency() != currency) {
        m_settings.setValue("currency", currency);
        emit currencyChanged();
        emit settingChanged("currency");
    }
}

bool Settings::autoBackup() const
{
    return m_settings.value("autoBackup", true).toBool();
}

void Settings::setAutoBackup(bool enabled)
{
    if (this->autoBackup() != enabled) {
        m_settings.setValue("autoBackup", enabled);
        emit autoBackupChanged();
        emit settingChanged("autoBackup");
    }
}

int Settings::backupInterval() const
{
    return m_settings.value("backupInterval", 7).toInt();
}

void Settings::setBackupInterval(int days)
{
    if (this->backupInterval() != days) {
        m_settings.setValue("backupInterval", days);
        emit backupIntervalChanged();
        emit settingChanged("backupInterval");
    }
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return m_settings.value(key, defaultValue);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    m_settings.setValue(key, value);
    emit settingChanged(key);
}

void Settings::resetToDefaults()
{
    m_settings.clear();
    
    emit languageChanged();
    emit themeChanged();
    emit currencyChanged();
    emit autoBackupChanged();
    emit backupIntervalChanged();
}
