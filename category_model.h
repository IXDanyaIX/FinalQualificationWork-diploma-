#ifndef CATEGORY_MODEL_H
#define CATEGORY_MODEL_H
#include <QSqlTableModel>
#include "database.h"

class Category_model
{
public:
    Category_model();
    QSqlTableModel* getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;
};

#endif // CATEGORY_MODEL_H
