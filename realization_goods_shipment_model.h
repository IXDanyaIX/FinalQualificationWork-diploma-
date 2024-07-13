#ifndef REALIZATION_GOODS_SHIPMENT_MODEL_H
#define REALIZATION_GOODS_SHIPMENT_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Realization_goods_shipment_model
{
public:
    Realization_goods_shipment_model();

    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // REALIZATION_GOODS_SHIPMENT_MODEL_H
