#include "realization_model.h"

Realization_model::Realization_model() : m_database(Database::instance()){
    m_model = read_from_db();
}

QSqlTableModel* Realization_model::getModel() const{
    return m_model;
}

QSqlTableModel* Realization_model::read_from_db() const{

    QSqlTableModel* baseModel = m_database.getModel("realization");

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel; // замените parent на соответствующий родительский объект
    model->setTable(baseModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    // Присвоение отношений
    model->setRelation(3, QSqlRelation("status_realization", "id", "name"));
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);


    // Удаление базовой модели, если она больше не нужна
    delete baseModel;

    model->select();


    model->setHeaderData(0, Qt::Horizontal, "Номер реализации");
    model->setHeaderData(1, Qt::Horizontal, "Дата продажи");
    model->setHeaderData(2, Qt::Horizontal, "Продавец");
    model->setHeaderData(3, Qt::Horizontal, "Статус");


    return model;
}

