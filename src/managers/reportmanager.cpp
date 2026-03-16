#include "reportmanager.h"
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/projectrepository.h"
#include <QDebug>

ReportManager* ReportManager::s_instance = nullptr;

ReportManager* ReportManager::instance()
{
    if (!s_instance) {
        s_instance = new ReportManager();
    }
    return s_instance;
}

ReportManager::ReportManager(QObject *parent)
    : QObject(parent)
{
}

QVariantMap ReportManager::generateReport(ReportType type, const QDate &from, const QDate &to, const QString &projectId)
{
    switch (type) {
        case ProfitLoss:
            return generateProfitLoss(from, to);
        case CashFlow:
            return generateCashFlow(from, to);
        case BudgetVsActual:
            return generateBudgetVsActual(projectId, from, to);
        case AccountsReceivable:
            return generateAR(from);
        case AccountsPayable:
            return generateAP(from);
        case ProjectSummary:
            return generateBudgetVsActual(projectId, from, to);
        default:
            return QVariantMap();
    }
}

QVariantMap ReportManager::generateProfitLoss(const QDate &from, const QDate &to)
{
    QVariantMap report;
    
    report["title"] = tr("Отчет о прибылях и убытках");
    report["period"] = QString("%1 - %2").arg(from.toString("dd.MM.yyyy"), to.toString("dd.MM.yyyy"));
    report["generatedAt"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    
    double income = TransactionRepository::instance()->getTotalIncome(from, to, "default");
    double expense = TransactionRepository::instance()->getTotalExpense(from, to, "default");
    double profit = income - expense;
    double margin = income > 0 ? (profit / income * 100.0) : 0.0;
    
    report["income"] = income;
    report["expense"] = expense;
    report["profit"] = profit;
    report["margin"] = margin;
    report["isProfitable"] = profit >= 0;
    
    // Income breakdown
    QVariantList incomeBreakdown;
    // TODO: Implement income by category
    report["incomeBreakdown"] = incomeBreakdown;
    
    // Expense breakdown
    report["expenseBreakdown"] = TransactionRepository::instance()->getByCategory(from, to, "default");
    
    return report;
}

QVariantMap ReportManager::generateCashFlow(const QDate &from, const QDate &to)
{
    QVariantMap report;
    
    report["title"] = tr("Отчет о движении денежных средств");
    report["period"] = QString("%1 - %2").arg(from.toString("dd.MM.yyyy"), to.toString("dd.MM.yyyy"));
    report["generatedAt"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    
    QVariantList cashFlow = TransactionRepository::instance()->getCashFlow(from.daysTo(to), "default");
    
    double totalIncome = 0;
    double totalExpense = 0;
    
    for (const auto &item : cashFlow) {
        QVariantMap map = item.toMap();
        totalIncome += map["income"].toDouble();
        totalExpense += map["expense"].toDouble();
    }
    
    report["cashFlow"] = cashFlow;
    report["totalIncome"] = totalIncome;
    report["totalExpense"] = totalExpense;
    report["netCashFlow"] = totalIncome - totalExpense;
    
    return report;
}

QVariantMap ReportManager::generateBudgetVsActual(const QString &projectId, const QDate &from, const QDate &to)
{
    QVariantMap report;
    
    report["title"] = tr("Бюджет vs Факт");
    report["period"] = QString("%1 - %2").arg(from.toString("dd.MM.yyyy"), to.toString("dd.MM.yyyy"));
    report["generatedAt"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    
    if (projectId.isEmpty()) {
        // All projects
        QVariantMap stats = ProjectRepository::instance()->getStatistics("default");
        report["totalBudget"] = stats["totalBudget"];
        report["totalSpent"] = stats["totalSpent"];
        report["totalRemaining"] = stats["totalRemaining"];
        report["utilization"] = stats["utilization"];
    } else {
        // Single project
        QVariantMap summary = TransactionRepository::instance()->getProjectFinanceSummary(projectId);
        report["projectId"] = projectId;
        report["income"] = summary["income"];
        report["expense"] = summary["expense"];
        report["balance"] = summary["balance"];
    }
    
    return report;
}

QVariantMap ReportManager::generateAR(const QDate &asOf)
{
    QVariantMap report;
    
    report["title"] = tr("Дебиторская задолженность");
    report["asOf"] = asOf.toString("dd.MM.yyyy");
    report["generatedAt"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    
    // TODO: Implement AR tracking
    report["totalAR"] = 0.0;
    report["items"] = QVariantList();
    
    return report;
}

QVariantMap ReportManager::generateAP(const QDate &asOf)
{
    QVariantMap report;
    
    report["title"] = tr("Кредиторская задолженность");
    report["asOf"] = asOf.toString("dd.MM.yyyy");
    report["generatedAt"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    
    // TODO: Implement AP tracking
    report["totalAP"] = 0.0;
    report["items"] = QVariantList();
    
    return report;
}

void ReportManager::exportToPDF(ReportType type, const QString &filePath, const QDate &from, const QDate &to, const QString &projectId)
{
    QVariantMap report = generateReport(type, from, to, projectId);
    
    // TODO: Implement PDF generation using Qt PDF or third-party library
    qDebug() << "Exporting to PDF:" << filePath;
    
    emit exportCompleted(filePath);
}

void ReportManager::exportToExcel(ReportType type, const QString &filePath, const QDate &from, const QDate &to, const QString &projectId)
{
    QVariantMap report = generateReport(type, from, to, projectId);
    
    // TODO: Implement Excel generation using QXlsx or similar
    qDebug() << "Exporting to Excel:" << filePath;
    
    emit exportCompleted(filePath);
}

QString ReportManager::generateReportTitle(ReportType type)
{
    switch (type) {
        case ProfitLoss:
            return tr("Отчет о прибылях и убытках");
        case CashFlow:
            return tr("Отчет о движении денежных средств");
        case BudgetVsActual:
            return tr("Бюджет vs Факт");
        case AccountsReceivable:
            return tr("Дебиторская задолженность");
        case AccountsPayable:
            return tr("Кредиторская задолженность");
        case ProjectSummary:
            return tr("Сводка по проекту");
        default:
            return tr("Отчет");
    }
}
