#include "contractors_model.h"

Contractors_model::Contractors_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Contractors_model::getModel() const{
    return m_model;
}

QSqlTableModel* Contractors_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("contractors");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Название");
    model->setHeaderData(2, Qt::Horizontal, "Телефон");
    model->setHeaderData(3, Qt::Horizontal, "Адресс");
    return model;
}
