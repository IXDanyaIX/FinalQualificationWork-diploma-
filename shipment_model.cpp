#include "shipment_model.h"

Shipment_model::Shipment_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Shipment_model::getModel() const{
    return m_model;
}

QSqlTableModel* Shipment_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("shipment");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ID заказа");
    model->setHeaderData(2, Qt::Horizontal, "Дата поставки");
    return model;
}
