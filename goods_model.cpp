#include "goods_model.h"

Goods_model::Goods_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel *Goods_model::getModel() const
{
    return m_model;
}

QSqlTableModel* Goods_model::read_from_db() const{
    QSqlTableModel* baseModel = m_database.getModel("goods");

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel;
    model->setTable(baseModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    // Присвоение отношений
    model->setRelation(1, QSqlRelation("contractors", "id", "name"));
    model->setRelation(2, QSqlRelation("units_of_measurement", "id", "designation"));
    model->setRelation(3, QSqlRelation("category", "id", "name"));
    model->setRelation(4, QSqlRelation("status_goods", "id", "name"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    // Удаление базовой модели, так как она больше не нужна
    delete baseModel;

    model->select();

    model->setHeaderData(0, Qt::Horizontal, "Артикул");
    model->setHeaderData(1, Qt::Horizontal, "Контрагент");
    model->setHeaderData(2, Qt::Horizontal, "ЕИ");
    model->setHeaderData(3, Qt::Horizontal, "Категория");
    model->setHeaderData(4, Qt::Horizontal, "Статус");
    model->setHeaderData(5, Qt::Horizontal, "Название ");
    model->setHeaderData(6, Qt::Horizontal, "Описание");
    model->setHeaderData(8, Qt::Horizontal, "Цена покупки");
    model->setHeaderData(9, Qt::Horizontal, "Цена продажи");
    model->setHeaderData(10, Qt::Horizontal, "Количество");

    return model;
}

