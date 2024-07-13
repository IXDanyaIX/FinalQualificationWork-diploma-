#include "status_goods_model.h"

Status_goods_model::Status_goods_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Status_goods_model::getModel() const{
    return m_model;
}

QSqlTableModel* Status_goods_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("status_goods");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Название");
    return model;
}
