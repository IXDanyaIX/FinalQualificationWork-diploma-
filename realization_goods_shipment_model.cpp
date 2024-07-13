#include "realization_goods_shipment_model.h"

Realization_goods_shipment_model::Realization_goods_shipment_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Realization_goods_shipment_model::getModel() const{
    return m_model;
}

QSqlTableModel* Realization_goods_shipment_model::read_from_db() const{
    QSqlTableModel* baseModel = m_database.getModel("realization_goods_shipment");

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel; // замените parent на соответствующий родительский объект
    model->setTable(baseModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // Присвоение отношений
    model->setRelation(2, QSqlRelation("goods", "id", "name"));

    // Удаление базовой модели, если она больше не нужна
    delete baseModel;

    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Номер покупки");
    model->setHeaderData(2, Qt::Horizontal, "Название товара");
    model->setHeaderData(3, Qt::Horizontal, "Номер поставки");
    model->setHeaderData(4, Qt::Horizontal, "Количество");
    return model;
}
