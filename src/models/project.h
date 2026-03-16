#ifndef PROJECT_H
#define PROJECT_H

#include "basemodel.h"
#include <QDate>
#include <QColor>

class Project : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(ProjectStatus status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDatePlanned READ endDatePlanned WRITE setEndDatePlanned NOTIFY endDatePlannedChanged)
    Q_PROPERTY(QDate endDateActual READ endDateActual WRITE setEndDateActual NOTIFY endDateActualChanged)
    Q_PROPERTY(double budget READ budget WRITE setBudget NOTIFY budgetChanged)
    Q_PROPERTY(double budgetSpent READ budgetSpent WRITE setBudgetSpent NOTIFY budgetSpentChanged)
    Q_PROPERTY(double budgetRemaining READ budgetRemaining NOTIFY budgetRemainingChanged)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString companyId READ companyId WRITE setCompanyId NOTIFY companyIdChanged)
    Q_PROPERTY(QString statusLabel READ statusLabel NOTIFY statusChanged)
    Q_PROPERTY(QColor statusColor READ statusColor NOTIFY statusChanged)

public:
    enum ProjectStatus {
        Planning = 0,
        Active = 1,
        OnHold = 2,
        Completed = 3,
        Cancelled = 4
    };
    Q_ENUM(ProjectStatus)

    explicit Project(QObject *parent = nullptr);
    
    // Getters
    QString code() const { return m_code; }
    QString name() const { return m_name; }
    QString description() const { return m_description; }
    QString address() const { return m_address; }
    QString city() const { return m_city; }
    ProjectStatus status() const { return m_status; }
    QDate startDate() const { return m_startDate; }
    QDate endDatePlanned() const { return m_endDatePlanned; }
    QDate endDateActual() const { return m_endDateActual; }
    double budget() const { return m_budget; }
    double budgetSpent() const { return m_budgetSpent; }
    double budgetRemaining() const { return m_budget - m_budgetSpent; }
    double progress() const { return m_progress; }
    QString clientId() const { return m_clientId; }
    QString clientName() const { return m_clientName; }
    QString companyId() const { return m_companyId; }
    
    // Setters
    void setCode(const QString &code);
    void setName(const QString &name);
    void setDescription(const QString &desc);
    void setAddress(const QString &addr);
    void setCity(const QString &city);
    void setStatus(ProjectStatus status);
    void setStartDate(const QDate &date);
    void setEndDatePlanned(const QDate &date);
    void setEndDateActual(const QDate &date);
    void setBudget(double budget);
    void setBudgetSpent(double spent);
    void setProgress(double progress);
    void setClientId(const QString &id);
    void setClientName(const QString &name);
    void setCompanyId(const QString &id);
    
    // Helper methods
    QString statusLabel() const;
    QColor statusColor() const;
    Q_INVOKABLE QString formattedBudget() const;
    Q_INVOKABLE QString formattedSpent() const;
    Q_INVOKABLE QString formattedRemaining() const;
    Q_INVOKABLE double budgetUtilization() const;
    Q_INVOKABLE bool isOverBudget() const;
    Q_INVOKABLE int daysRemaining() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void codeChanged();
    void nameChanged();
    void descriptionChanged();
    void addressChanged();
    void cityChanged();
    void statusChanged();
    void startDateChanged();
    void endDatePlannedChanged();
    void endDateActualChanged();
    void budgetChanged();
    void budgetSpentChanged();
    void budgetRemainingChanged();
    void progressChanged();
    void clientIdChanged();
    void clientNameChanged();
    void companyIdChanged();

private:
    QString m_code;
    QString m_name;
    QString m_description;
    QString m_address;
    QString m_city;
    ProjectStatus m_status = Planning;
    QDate m_startDate;
    QDate m_endDatePlanned;
    QDate m_endDateActual;
    double m_budget = 0.0;
    double m_budgetSpent = 0.0;
    double m_progress = 0.0;
    QString m_clientId;
    QString m_clientName;
    QString m_companyId;
};

#endif // PROJECT_H
