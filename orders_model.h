#ifndef ORDERS_MODEL_H
#define ORDERS_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Orders_model
{
public:
    Orders_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;
    QSqlTableModel* read_from_db() const;
};

#endif // ORDERS_MODEL_H
