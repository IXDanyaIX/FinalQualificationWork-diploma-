#include "orders_goods_model.h"


Orders_goods_model::Orders_goods_model() : m_database(Database::instance()) {
    m_model = read_from_db();


}

QSqlTableModel* Orders_goods_model::getModel() const{
    return m_model;
}

QSqlTableModel* Orders_goods_model::read_from_db() const{
    QSqlTableModel* baseModel = m_database.getModel("orders_goods");

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel;
    model->setTable(baseModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // Присвоение отношений
    model->setRelation(0, QSqlRelation("goods", "id", "name"));

    delete baseModel;

    model->select();

    model->setHeaderData(0, Qt::Horizontal, "Название товара");
    model->setHeaderData(1, Qt::Horizontal, "Номер заказа");
    model->setHeaderData(2, Qt::Horizontal, "Количество");




    return model;
}



