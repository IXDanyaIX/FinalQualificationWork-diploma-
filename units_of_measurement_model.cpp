#include "units_of_measurement_model.h"

Units_of_measurement_model::Units_of_measurement_model() : m_database(Database::instance()) {
    m_model = read_from_db();
}

QSqlTableModel* Units_of_measurement_model::getModel() const{
    return m_model;
}

QSqlTableModel* Units_of_measurement_model::read_from_db() const{
    QSqlTableModel* model = m_database.getModel("units_of_measurement");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Наименование");
    model->setHeaderData(2, Qt::Horizontal, "Обозначение");
    return model;
}

