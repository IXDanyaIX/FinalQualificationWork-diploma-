#ifndef STATUS_GOODS_MODEL_H
#define STATUS_GOODS_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Status_goods_model
{
public:
    Status_goods_model();
    QSqlTableModel* getModel() const;

private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // STATUS_GOODS_MODEL_H
