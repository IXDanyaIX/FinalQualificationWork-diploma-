#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}, m_db {new QSqlDatabase{QSqlDatabase::addDatabase("QPSQL")}}
{

    m_db->setConnectOptions();
    m_db->setHostName("localhost");
    m_db->setDatabaseName("account_product");
    m_db->setUserName("postgres");
    m_db->setPassword("1234");
    m_db->setPort(5432);
}

Database& Database::instance(){
    static Database instance;
    return instance;
}

Database::~Database(){
    disconnect();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

}
bool Database::connect(){
    if (!m_db->isOpen())
    {
        if(!m_db->open()){
            qDebug() << "Failed to open database:" << m_db->lastError().text();
            return false;
        }
        else qDebug() << "Conect_database\n";
    }
    return true;

}

void Database::disconnect(){
    if(m_db->isOpen()){
        m_db->close();
    }
}

QSqlTableModel* Database::getModel(const QString &tableName){
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database());
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    return model;
}



