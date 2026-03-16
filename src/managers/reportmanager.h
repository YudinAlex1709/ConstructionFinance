#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QObject>
#include <QDate>
#include <QVariantMap>
#include <QVariantList>

class ReportManager : public QObject
{
    Q_OBJECT

public:
    enum ReportType {
        ProfitLoss,         // P&L
        CashFlow,           // ДДС
        BudgetVsActual,     // Бюджет vs Факт
        AccountsReceivable, // Дебиторка
        AccountsPayable,    // Кредиторка
        ProjectSummary      // Сводка по проекту
    };
    Q_ENUM(ReportType)
    
    static ReportManager* instance();
    
    Q_INVOKABLE QVariantMap generateReport(ReportType type, 
                                           const QDate &from, 
                                           const QDate &to,
                                           const QString &projectId = QString());
    
    Q_INVOKABLE void exportToPDF(ReportType type, 
                                  const QString &filePath,
                                  const QDate &from, 
                                  const QDate &to,
                                  const QString &projectId = QString());
    
    Q_INVOKABLE void exportToExcel(ReportType type, 
                                    const QString &filePath,
                                    const QDate &from, 
                                    const QDate &to,
                                    const QString &projectId = QString());
    
    Q_INVOKABLE QString generateReportTitle(ReportType type);

signals:
    void reportGenerated(const QVariantMap &data);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &message);

private:
    explicit ReportManager(QObject *parent = nullptr);
    
    QVariantMap generateProfitLoss(const QDate &from, const QDate &to);
    QVariantMap generateCashFlow(const QDate &from, const QDate &to);
    QVariantMap generateBudgetVsActual(const QString &projectId, const QDate &from, const QDate &to);
    QVariantMap generateAR(const QDate &asOf);
    QVariantMap generateAP(const QDate &asOf);
    
    static ReportManager *s_instance;
};

#endif // REPORTMANAGER_H
