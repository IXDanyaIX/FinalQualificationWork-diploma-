#ifndef ORDERS_GOODS_MODEL_H
#define ORDERS_GOODS_MODEL_H

#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include "database.h"


// class CustomProxyModel : public QSortFilterProxyModel {
// public:
//     CustomProxyModel(QObject *parent = nullptr);

//     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//     int columnCount(const QModelIndex &parent = QModelIndex()) const override;
// };

class Orders_goods_model
{
public:
    Orders_goods_model();
    QSqlTableModel* getModel() const;
private:

    //double getPriceForGood(int goodId) const;
    //void fill_price_and_sum();




    QSqlTableModel* m_model;
    //CustomProxyModel* m_proxyModel;

    
    
    Database &m_database;
    QSqlTableModel* read_from_db() const;



};

#endif // ORDERS_GOODS_MODEL_H
