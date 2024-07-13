#ifndef GOODS_MODEL_H
#define GOODS_MODEL_H

#include "database.h"
#include <QSqlTableModel>

class Goods_model
{

public:
    Goods_model();
    QSqlTableModel *getModel() const;
private:
    QSqlTableModel* m_model;
    Database &m_database;

    QSqlTableModel* read_from_db() const;

};

#endif // GOODS_MODEL_H
