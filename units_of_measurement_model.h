#ifndef UNITS_OF_MEASUREMENT_MODEL_H
#define UNITS_OF_MEASUREMENT_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Units_of_measurement_model
{
public:
    Units_of_measurement_model();
    QSqlTableModel* getModel() const;

private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // UNITS_OF_MEASUREMENT_MODEL_H
