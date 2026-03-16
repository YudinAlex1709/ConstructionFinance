#include "migration.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

const QList<QPair<int, QString>> Migration::MIGRATIONS = {
    // V1: Initial schema
    {1, R"(
        -- Companies table
        CREATE TABLE IF NOT EXISTS companies (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            inn TEXT UNIQUE,
            kpp TEXT,
            address TEXT,
            phone TEXT,
            email TEXT,
            logo TEXT,
            settings TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );

        -- Users table
        CREATE TABLE IF NOT EXISTS users (
            id TEXT PRIMARY KEY,
            email TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            phone TEXT,
            role INTEGER DEFAULT 4,
            is_active INTEGER DEFAULT 1,
            company_id TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (company_id) REFERENCES companies(id)
        );

        CREATE INDEX IF NOT EXISTS idx_users_company ON users(company_id);
        CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);

        -- Contractors table
        CREATE TABLE IF NOT EXISTS contractors (
            id TEXT PRIMARY KEY,
            company_id TEXT NOT NULL,
            type INTEGER DEFAULT 0,
            name TEXT NOT NULL,
            full_name TEXT,
            inn TEXT,
            kpp TEXT,
            ogrn TEXT,
            phone TEXT,
            email TEXT,
            address TEXT,
            city TEXT,
            bank_account TEXT,
            bank_name TEXT,
            bik TEXT,
            notes TEXT,
            is_active INTEGER DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (company_id) REFERENCES companies(id)
        );

        CREATE INDEX IF NOT EXISTS idx_contractors_company ON contractors(company_id);
        CREATE INDEX IF NOT EXISTS idx_contractors_type ON contractors(type);

        -- Projects table
        CREATE TABLE IF NOT EXISTS projects (
            id TEXT PRIMARY KEY,
            code TEXT NOT NULL,
            name TEXT NOT NULL,
            description TEXT,
            address TEXT,
            city TEXT,
            region TEXT,
            status INTEGER DEFAULT 0,
            start_date DATE,
            end_date_planned DATE,
            end_date_actual DATE,
            budget REAL DEFAULT 0,
            budget_spent REAL DEFAULT 0,
            progress REAL DEFAULT 0,
            company_id TEXT NOT NULL,
            client_id TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (company_id) REFERENCES companies(id),
            FOREIGN KEY (client_id) REFERENCES contractors(id)
        );

        CREATE INDEX IF NOT EXISTS idx_projects_company ON projects(company_id);
        CREATE INDEX IF NOT EXISTS idx_projects_status ON projects(status);
        CREATE INDEX IF NOT EXISTS idx_projects_client ON projects(client_id);

        -- Project stages table
        CREATE TABLE IF NOT EXISTS project_stages (
            id TEXT PRIMARY KEY,
            project_id TEXT NOT NULL,
            name TEXT NOT NULL,
            stage_order INTEGER,
            start_date DATE,
            end_date DATE,
            budget REAL DEFAULT 0,
            spent REAL DEFAULT 0,
            status INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE
        );

        CREATE INDEX IF NOT EXISTS idx_stages_project ON project_stages(project_id);

        -- Bank accounts table
        CREATE TABLE IF NOT EXISTS bank_accounts (
            id TEXT PRIMARY KEY,
            company_id TEXT NOT NULL,
            name TEXT NOT NULL,
            bank_name TEXT,
            account_number TEXT NOT NULL,
            bik TEXT,
            currency TEXT DEFAULT 'RUB',
            balance REAL DEFAULT 0,
            is_default INTEGER DEFAULT 0,
            is_active INTEGER DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (company_id) REFERENCES companies(id)
        );

        CREATE INDEX IF NOT EXISTS idx_accounts_company ON bank_accounts(company_id);

        -- Categories table
        CREATE TABLE IF NOT EXISTS categories (
            id TEXT PRIMARY KEY,
            company_id TEXT NOT NULL,
            name TEXT NOT NULL,
            type INTEGER NOT NULL,
            color TEXT DEFAULT '#3B82F6',
            icon TEXT,
            parent_id TEXT,
            is_system INTEGER DEFAULT 0,
            FOREIGN KEY (company_id) REFERENCES companies(id),
            FOREIGN KEY (parent_id) REFERENCES categories(id),
            UNIQUE(company_id, name, type)
        );

        CREATE INDEX IF NOT EXISTS idx_categories_company ON categories(company_id);

        -- Transactions table
        CREATE TABLE IF NOT EXISTS transactions (
            id TEXT PRIMARY KEY,
            type INTEGER NOT NULL,
            category TEXT,
            amount REAL NOT NULL,
            currency TEXT DEFAULT 'RUB',
            date DATE NOT NULL,
            project_id TEXT,
            company_id TEXT NOT NULL,
            contractor_id TEXT,
            bank_account_id TEXT,
            description TEXT,
            document_number TEXT,
            document_date DATE,
            created_by_id TEXT NOT NULL,
            status INTEGER DEFAULT 1,
            attachments TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (project_id) REFERENCES projects(id),
            FOREIGN KEY (contractor_id) REFERENCES contractors(id),
            FOREIGN KEY (bank_account_id) REFERENCES bank_accounts(id),
            FOREIGN KEY (created_by_id) REFERENCES users(id)
        );

        CREATE INDEX IF NOT EXISTS idx_transactions_date ON transactions(date);
        CREATE INDEX IF NOT EXISTS idx_transactions_project ON transactions(project_id);
        CREATE INDEX IF NOT EXISTS idx_transactions_type ON transactions(type);
        CREATE INDEX IF NOT EXISTS idx_transactions_company ON transactions(company_id);

        -- Materials table
        CREATE TABLE IF NOT EXISTS materials (
            id TEXT PRIMARY KEY,
            project_id TEXT NOT NULL,
            name TEXT NOT NULL,
            unit TEXT,
            quantity REAL,
            price REAL,
            total REAL,
            supplier_id TEXT,
            material_date DATE,
            notes TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE,
            FOREIGN KEY (supplier_id) REFERENCES contractors(id)
        );

        CREATE INDEX IF NOT EXISTS idx_materials_project ON materials(project_id);
        CREATE INDEX IF NOT EXISTS idx_materials_supplier ON materials(supplier_id);

        -- Documents table
        CREATE TABLE IF NOT EXISTS documents (
            id TEXT PRIMARY KEY,
            project_id TEXT,
            type INTEGER,
            name TEXT,
            file_path TEXT,
            file_size INTEGER,
            mime_type TEXT,
            description TEXT,
            tags TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE
        );

        CREATE INDEX IF NOT EXISTS idx_documents_project ON documents(project_id);

        -- Activity log table
        CREATE TABLE IF NOT EXISTS activity_log (
            id TEXT PRIMARY KEY,
            user_id TEXT NOT NULL,
            action TEXT,
            entity_type TEXT,
            entity_id TEXT,
            old_value TEXT,
            new_value TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id)
        );

        CREATE INDEX IF NOT EXISTS idx_activity_user ON activity_log(user_id);
        CREATE INDEX IF NOT EXISTS idx_activity_entity ON activity_log(entity_type, entity_id);

        -- Insert default categories
        INSERT INTO categories (id, company_id, name, type, color, is_system) VALUES
        ('cat_income_1', 'default', 'Оплата от заказчиков', 0, '#10B981', 1),
        ('cat_income_2', 'default', 'Авансы', 0, '#34D399', 1),
        ('cat_income_3', 'default', 'Дополнительные работы', 0, '#6EE7B7', 1),
        ('cat_expense_1', 'default', 'Материалы', 1, '#EF4444', 1),
        ('cat_expense_2', 'default', 'Зарплата', 1, '#F87171', 1),
        ('cat_expense_3', 'default', 'Аренда техники', 1, '#FCA5A5', 1),
        ('cat_expense_4', 'default', 'Субподряд', 1, '#FDA4AF', 1),
        ('cat_expense_5', 'default', 'Налоги и взносы', 1, '#FB7185', 1),
        ('cat_expense_6', 'default', 'Коммунальные услуги', 1, '#F43F5E', 1),
        ('cat_expense_7', 'default', 'Транспорт', 1, '#E11D48', 1),
        ('cat_expense_8', 'default', 'Прочие расходы', 1, '#9CA3AF', 1);
    )"},
    
    // V2: Add version tracking table
    {2, R"(
        CREATE TABLE IF NOT EXISTS schema_version (
            version INTEGER PRIMARY KEY,
            applied_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
        
        INSERT OR REPLACE INTO schema_version (version) VALUES (2);
    )"},
    
    // V3: Add indexes for performance
    {3, R"(
        CREATE INDEX IF NOT EXISTS idx_transactions_date_type ON transactions(date, type);
        CREATE INDEX IF NOT EXISTS idx_projects_status_budget ON projects(status, budget);
        CREATE INDEX IF NOT EXISTS idx_contractors_inn ON contractors(inn);
    )"}
};

Migration::Migration()
{
}

bool Migration::run()
{
    if (!createVersionTable()) {
        return false;
    }
    
    int currentVer = currentVersion();
    qDebug() << "Current database version:" << currentVer;
    
    for (const auto &migration : MIGRATIONS) {
        int version = migration.first;
        const QString &sql = migration.second;
        
        if (version > currentVer) {
            qDebug() << "Applying migration version" << version;
            if (!applyMigration(version, sql)) {
                return false;
            }
        }
    }
    
    return true;
}

bool Migration::createVersionTable()
{
    QSqlQuery query;
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS schema_version (
            version INTEGER PRIMARY KEY,
            applied_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )")) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

int Migration::currentVersion() const
{
    QSqlQuery query("SELECT MAX(version) FROM schema_version");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool Migration::applyMigration(int version, const QString &sql)
{
    Database *db = Database::instance();
    
    if (!db->transaction()) {
        m_lastError = "Failed to start transaction";
        return false;
    }
    
    // Split SQL by semicolon and execute each statement
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    
    for (QString statement : statements) {
        statement = statement.trimmed();
        if (statement.isEmpty()) continue;
        
        QSqlQuery query = db->execute(statement);
        if (query.lastError().isValid()) {
            m_lastError = query.lastError().text();
            db->rollback();
            return false;
        }
    }
    
    if (!setVersion(version)) {
        db->rollback();
        return false;
    }
    
    if (!db->commit()) {
        m_lastError = "Failed to commit transaction";
        return false;
    }
    
    qDebug() << "Migration version" << version << "applied successfully";
    return true;
}

bool Migration::setVersion(int version)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO schema_version (version) VALUES (:version)");
    query.bindValue(":version", version);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}
