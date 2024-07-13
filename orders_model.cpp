#include "orders_model.h"

Orders_model::Orders_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Orders_model::getModel() const{
    return m_model;
}

QSqlTableModel* Orders_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("orders");


    model->select();
    model->setHeaderData(0, Qt::Horizontal, "Номер заказа");
    model->setHeaderData(1, Qt::Horizontal, "Дата заказа");


    return model;

}
