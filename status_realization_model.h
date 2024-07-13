#ifndef STATUS_REALIZATION_MODEL_H
#define STATUS_REALIZATION_MODEL_H

#include <QSqlTableModel>
#include "database.h"

class Status_realization_model
{
public:
    Status_realization_model();
    QSqlTableModel* getModel() const;

private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // STATUS_REALIZATION_MODEL_H
