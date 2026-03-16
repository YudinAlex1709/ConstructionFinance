#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QString>

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(bool isDebugBuild READ isDebugBuild CONSTANT)

public:
    static Application* instance();
    
    QString version() const;
    bool isDebugBuild() const;
    
    Q_INVOKABLE QString formatFileSize(qint64 bytes);
    Q_INVOKABLE QString getDatabasePath();
    Q_INVOKABLE QString getBackupPath();

signals:
    void databasePathChanged(const QString &path);

private:
    explicit Application(QObject *parent = nullptr);
    static Application *s_instance;
};

#endif // APPLICATION_H
