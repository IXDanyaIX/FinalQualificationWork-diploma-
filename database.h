#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
public:
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    static Database& instance();


    ~Database();

    bool connect();
    void disconnect();


    QSqlTableModel* getModel(const QString &tableName);


private:
    explicit Database(QObject *parent = nullptr);
    std::unique_ptr<QSqlDatabase> m_db;
};

#endif // DATABASE_H
