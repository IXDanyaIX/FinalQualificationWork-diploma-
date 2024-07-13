#ifndef CONTRACTORS_MODEL_H
#define CONTRACTORS_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Contractors_model
{
public:
    Contractors_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // CONTRACTORS_MODEL_H
