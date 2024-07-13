#include "category_model.h"

Category_model::Category_model() : m_database(Database::instance()) {
    m_model = read_from_db();
}

QSqlTableModel* Category_model::getModel() const{
    return m_model;
}

QSqlTableModel* Category_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("category");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Наименование");
    return model;
}

