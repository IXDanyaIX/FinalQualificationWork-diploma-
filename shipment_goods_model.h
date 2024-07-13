#ifndef SHIPMENT_GOODS_MODEL_H
#define SHIPMENT_GOODS_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Shipment_goods_model
{
public:
    Shipment_goods_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // SHIPMENT_GOODS_MODEL_H
