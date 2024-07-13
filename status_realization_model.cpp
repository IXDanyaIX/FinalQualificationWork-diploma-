#include "status_realization_model.h"

Status_realization_model::Status_realization_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Status_realization_model::getModel() const{
    return m_model;
}

QSqlTableModel* Status_realization_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("status_realization");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Название");
    return model;
}
