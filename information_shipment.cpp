#include "information_shipment.h"
#include "ui_information_shipment.h"
#include <QSqlRelationalDelegate>
#include <QPushButton>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSortFilterProxyModel>
#include "shipment_goods_model.h"

Information_shipment::Information_shipment(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Information_shipment)
{
    ui->setupUi(this);



    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    proxyModel = new QSortFilterProxyModel(this);
    Shipment_goods_model *shipment_goods = new Shipment_goods_model;

    QAbstractItemModel* model = (QAbstractItemModel*) shipment_goods->getModel();
    proxyModel->setSourceModel(model);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

Information_shipment::~Information_shipment()
{
    delete ui;
}

void Information_shipment::setModel(QAbstractItemModel *model){

    currentModel = model;
    mapper->setModel(model);

    mapper->setItemDelegate(new QSqlRelationalDelegate(this));


    mapper->addMapping(ui->id_spinBox, 0);
    mapper->addMapping(ui->idOrderComboBox,1);
    mapper->addMapping(ui->dateEdit, 2);

    ui->idOrderComboBox->setModel(model);
    ui->idOrderComboBox->setModelColumn(1);

    proxyModel->setFilterKeyColumn(2);
    ui->tableView->setModel(proxyModel);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(0);


    connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Information_shipment::updateButtonsAvailability);
    updateButtonsAvailability();
}

void Information_shipment::setCurrentIndex(const QModelIndex &index){
    if (index.isValid()){
        mapper->setCurrentModelIndex(index);
        currentIndex = index;

        // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
        proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));

    }
}

void Information_shipment::updateButtonsAvailability(){
    int currentIndex = mapper->currentIndex();
    int lastIndex = mapper->model()->rowCount() - 1; // Получаем индекс последней записи

    bool hasPrevious = (currentIndex > 0);
    bool hasNext = (currentIndex < lastIndex);

    ui->last_shipment->setEnabled(hasPrevious);
    ui->next_shipment->setEnabled(hasNext);
    Update_state();
}

void Information_shipment::onModelUpdated(){
    mapper->setModel(currentModel); // Перезапуск модели в mapper
    mapper->toFirst(); // Установить на первый индекс или текущий актуальный индекс
    updateButtonsAvailability();
}


void Information_shipment::on_close_pushButton_clicked()
{
    close();
    emit closed();
}


void Information_shipment::on_save_pushButton_clicked()
{
    try {
        if (mapper->submit()) {
            Shipment_goods_model* shipment_model = new Shipment_goods_model;
            QSqlTableModel* model = shipment_model->getModel();
            model->setEditStrategy(QSqlTableModel::OnManualSubmit);
            model->submitAll();
            proxyModel->setSourceModel((QAbstractItemModel*)shipment_model->getModel());
            proxyModel->setFilterKeyColumn(2);
            // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
            proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));


        } else {
            QSqlTableModel *tempModel = qobject_cast<QSqlTableModel*>(currentModel);
            if (tempModel) {
                // Установка стратегии редактирования модели на OnRowChange
                tempModel->setEditStrategy(QSqlTableModel::OnRowChange);
            }
            qDebug() << "Submit failed: " << tempModel->lastError().text();
            tempModel->revertRow(currentIndex.row());

            if (state == State::create){
                tempModel->insertRow(tempModel->rowCount());

            }
            //tempModel->revert();
            mapper->revert();
            mapper->setCurrentModelIndex(currentIndex);

            //mapper->setCurrentModelIndex(currentIndex);
            QMessageBox::information(this, "Предупреждение", "Не удалось сохранить данные! Не все поля были заполнены или поставка с таким индексом уже существует!");
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught: " << e.what();
    }
}


void Information_shipment::on_last_shipment_triggered()
{
    mapper->toPrevious();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));


    Update_state();
}


void Information_shipment::on_next_shipment_triggered()
{
    mapper->toNext();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));


    Update_state();
}

void Information_shipment::Update_state(){
    if (ui->id_spinBox->value() == 0){
        state = State::create;
    }
    else state = State::update;
}


void Information_shipment::on_add_record_triggered()
{
    QSqlTableModel *table_model = qobject_cast<QSqlTableModel*>(proxyModel->sourceModel());
    table_model->setEditStrategy(QSqlTableModel::OnRowChange);
    if (table_model) {
        int row = table_model->rowCount();
        table_model->insertRow(row);
        QModelIndex index = table_model->index(row, 2); // Индекс скрытой колонки
        table_model->setData(index, ui->id_spinBox->value());
        table_model->submit();
        proxyModel->invalidate(); // Перезагружаем прокси-модель

    }
}


void Information_shipment::on_delete_record_triggered()
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для удаления.");
        return;
    }

    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    if (sourceIndex.isValid()) {
        QSqlRelationalTableModel *model = qobject_cast<QSqlRelationalTableModel *>(proxyModel->sourceModel());
        if (model) {
            // Отключаем отображение отношения
            model->setRelation(0, QSqlRelation());
            model->select(); // Перезагружаем модель без отношения
            \
                int row = sourceIndex.row();
            // Удаляем строку
            model->removeRow(row);
            qDebug() << "Удаление строки:" << sourceIndex.row();

            if (model->submitAll()) {
                // Восстанавливаем отображение отношения
                model->setRelation(1, QSqlRelation("goods", "id", "name"));
                model->select(); // Перезагружаем модель с восстановленным отношением
                proxyModel->invalidate(); // Перезагружаем прокси-модель
                qDebug() << "Изменения успешно применены.";
                QMessageBox::information(this, "Успех", "Запись успешно удалена.");
            } else {
                // Восстанавливаем отображение отношения
                model->setRelation(2, QSqlRelation("goods", "id", "name"));
                model->select(); // Перезагружаем модель с восстановленным отношением
                proxyModel->invalidate(); // Перезагружаем прокси-модель
                qDebug() << "Ошибка применения изменений:" << model->lastError().text();
                QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись: " + model->lastError().text());
            }


        } else {
            qDebug() << "Не удалось привести к QSqlRelationalTableModel.";
        }
    } else {
        qDebug() << "Неверный sourceIndex.";
    }
}

