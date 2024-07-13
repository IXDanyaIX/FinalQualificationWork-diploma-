#ifndef SHIPMENT_MODEL_H
#define SHIPMENT_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Shipment_model
{
public:
    Shipment_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // SHIPMENT_MODEL_H
