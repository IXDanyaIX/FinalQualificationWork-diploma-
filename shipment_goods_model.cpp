#include "shipment_goods_model.h"

Shipment_goods_model::Shipment_goods_model() : m_database(Database::instance()) {
    m_model = read_from_db();
}

QSqlTableModel* Shipment_goods_model::getModel() const{
    return m_model;
}

QSqlTableModel* Shipment_goods_model::read_from_db() const{
    QSqlTableModel* baseModel = m_database.getModel("shipment_goods");

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel; // замените parent на соответствующий родительский объект
    model->setTable(baseModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // Присвоение отношений
    model->setRelation(1, QSqlRelation("goods", "id", "name"));

    // Удаление базовой модели, если она больше не нужна
    delete baseModel;

    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Название товара");
    model->setHeaderData(2, Qt::Horizontal, "Номер поставки");
    model->setHeaderData(3, Qt::Horizontal, "Количество");
    model->setHeaderData(4, Qt::Horizontal, "Дата производства");
    model->setHeaderData(5, Qt::Horizontal, "Срок годности");




    return model;
}

