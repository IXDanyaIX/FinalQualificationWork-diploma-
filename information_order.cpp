#include "information_order.h"
#include "ui_information_order.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QSortFilterProxyModel>
#include <QLineEdit>

Information_order::Information_order(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Information_order)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    // Модель для поиска и фильтрации
    proxyModel = new QSortFilterProxyModel(this);
    Orders_goods_model *orders_goods = new Orders_goods_model;

    ui->tableView->setModel(orders_goods->getModel());

    proxyModel->setSourceModel(ui->tableView->model());

    // Настройка столбцов таблицы
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->setSortingEnabled(false); // Отключение сортировки

}

Information_order::~Information_order()
{
    delete ui;
}

// Установка модели
void Information_order::setModel(QAbstractItemModel *model){
    currentModel = model;
    mapper->setModel(model);
    mapper->addMapping(ui->id_spinBox, 0);
    mapper->addMapping(ui->dateEdit, 1);


    proxyModel->setFilterKeyColumn(1); // Устанавливка фильтра по первому столбцу
    ui->tableView->setModel(proxyModel);
    ui->tableView->hideColumn(1); // Скрытие первого столбца
    Calculate_sum(); // Вычисление суммы

    connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Information_order::updateButtonsAvailability);
    updateButtonsAvailability();
}

//  Установка индекса для формы
void Information_order::setCurrentIndex(const QModelIndex &index){
    currentIndex = index;

    if (index.isValid()){
        mapper->setCurrentModelIndex(index);
        proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    }
}

// Обновление кнопок для списка товаров
void Information_order::updateButtonsAvailability(){
    int currentIndex = mapper->currentIndex();
    int lastIndex = mapper->model()->rowCount() - 1; // Получение индекса последней записи

    bool hasPrevious = (currentIndex > 0);
    bool hasNext = (currentIndex < lastIndex);

    ui->last_order->setEnabled(hasPrevious);
    ui->next_order->setEnabled(hasNext);
    Update_state();
    proxyModel->invalidate(); // Перезагрузка прокси-модель после обновления состояния

}

// Обновление модели
void Information_order::onModelUpdated(){
    mapper->setModel(currentModel);
    mapper->toFirst();
    updateButtonsAvailability();
}

// Закрытие формы
void Information_order::on_close_pushButton_clicked()
{
    close();
    emit closed();
}

// Сохранение данных
void Information_order::on_save_pushButton_clicked()
{
    Calculate_sum();
    try {

        if (mapper->submit()) {
            // Сохранение данных и обновление модели
            Orders_goods_model* orders_goods = new Orders_goods_model;
            QSqlTableModel* model = orders_goods->getModel();
            model->setEditStrategy(QSqlTableModel::OnManualSubmit);
            model->submitAll();
            proxyModel->setSourceModel((QAbstractItemModel*)orders_goods->getModel());
            proxyModel->setFilterKeyColumn(1); // Устанавливаем фильтр по первому столбцу
            proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));


        } else {
            QSqlTableModel *tempModel = qobject_cast<QSqlTableModel*>(currentModel);
            if (tempModel) {
                // Установка стратегии редактирования модели на OnRowChange
                tempModel->setEditStrategy(QSqlTableModel::OnRowChange);
            }
            qDebug() << "Submit failed: " << tempModel->lastError().text();

            tempModel->revertRow(currentIndex.row());

            if (state == State::create) tempModel->insertRow(tempModel->rowCount());
            mapper->revert();
            mapper->setCurrentModelIndex(currentIndex);

            QMessageBox::information(this, "Предупреждение", "Не удалось сохранить данные! Не все поля были заполнены или заказ с таким индексом уже существует!");
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught: " << e.what();
    }
}

// Перемотка записей назад
void Information_order::on_last_order_triggered()
{
    mapper->toPrevious();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    Update_state();
}

// Перемотка записей вперед
void Information_order::on_next_order_triggered()
{
    mapper->toNext();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    Update_state();
}

// Обновление состояния
void Information_order::Update_state(){
    if (ui->id_spinBox->value() == 0){
        state = State::create;
    }
    else { state = State::update; }
}


// Добавление записей в таблицу
void Information_order::on_add_record_triggered()
{
    QSqlTableModel *table_model = qobject_cast<QSqlTableModel*>(proxyModel->sourceModel());
    table_model->setEditStrategy(QSqlTableModel::OnRowChange);
    if (table_model) {
        int row = table_model->rowCount();
        table_model->insertRow(row);
        QModelIndex index = table_model->index(row, 1); // Индекс скрытой колонки
        table_model->setData(index, ui->id_spinBox->value());
        table_model->submit();
        proxyModel->invalidate(); // Перезагрузка прокси-модель

    }
}


// Удаление записей
void Information_order::on_delete_record_triggered()
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
            // Отключение отображение отношения
            model->setRelation(0, QSqlRelation());
            model->select(); // Перезагрузка модели без отношения
\
            int row = sourceIndex.row();
            // Удаление строки
            model->removeRow(row);
            qDebug() << "Удаление строки:" << sourceIndex.row();

            if (model->submitAll()) {
                // Восстанавление отображения отношения
                model->setRelation(0, QSqlRelation("goods", "id", "name"));
                model->select(); // Перезагрузка модели с восстановленным отношением
                proxyModel->invalidate(); // Перезагрузка прокси-модель
                qDebug() << "Изменения успешно применены.";
                QMessageBox::information(this, "Успех", "Запись успешно удалена.");
            } else {
                // Восстанавление отображения отношения
                model->setRelation(0, QSqlRelation("goods", "id", "name"));
                model->select(); // Перезагрузка модели с восстановленным отношением
                proxyModel->invalidate(); // Перезагрузка прокси-модель
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


void Information_order::on_tableView_clicked(const QModelIndex &index)
{
    currentRow = proxyModel->mapToSource(index).row();
}


// Подсчет суммы заказа
void Information_order::Calculate_sum(){
    int id = ui->id_spinBox->value(); // Получение ID заказа из виджета

    QSqlQuery query;
    query.prepare("SELECT SUM(g.purchase_price * og.quantity) AS total_order_price "
                  "FROM orders_goods og "
                  "JOIN goods g ON og.ID_goods = g.ID "
                  "WHERE og.ID_orders = :order_id");

    query.bindValue(":order_id", id); // Привязка значения ID заказа к запросу

    if (query.exec()) { // Проверка на успешность выполнения запроса
        if (query.next()) { // Переход к первой записи результата запроса
            double total_sum = query.value(0).toDouble(); // Извлечение суммы из результата
            QString sum_str = QString::number(total_sum, 'f', 2); // Вывод с фиксированной точностью до 2 знаков после запятой
            ui->SumLineEdit->setText(sum_str);
        } else {
            // Если запрос не вернул результатов
            qDebug() << "Заказ с ID" << id << "не найден";
        }
    } else {
        // Если запрос не удалось выполнить
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
}




