#ifndef REALIZATION_MODEL_H
#define REALIZATION_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Realization_model
{
public:
    Realization_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;

};

#endif // REALIZATION_MODEL_H
