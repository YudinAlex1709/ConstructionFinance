#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(bool autoBackup READ autoBackup WRITE setAutoBackup NOTIFY autoBackupChanged)
    Q_PROPERTY(int backupInterval READ backupInterval WRITE setBackupInterval NOTIFY backupIntervalChanged)

public:
    static Settings* instance();
    
    QString language() const;
    void setLanguage(const QString &language);
    
    QString theme() const;
    void setTheme(const QString &theme);
    
    QString currency() const;
    void setCurrency(const QString &currency);
    
    bool autoBackup() const;
    void setAutoBackup(bool enabled);
    
    int backupInterval() const;
    void setBackupInterval(int days);
    
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
    
    Q_INVOKABLE void resetToDefaults();

signals:
    void languageChanged();
    void themeChanged();
    void currencyChanged();
    void autoBackupChanged();
    void backupIntervalChanged();
    void settingChanged(const QString &key);

private:
    explicit Settings(QObject *parent = nullptr);
    
    QSettings m_settings;
    static Settings *s_instance;
};

#endif // SETTINGS_H
