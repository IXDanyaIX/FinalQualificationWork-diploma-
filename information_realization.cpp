#include "information_realization.h"
#include "ui_information_realization.h"
#include <QMessageBox>

Information_realization::Information_realization(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Information_realization)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    proxyModel = new QSortFilterProxyModel(this);
    Realization_goods_shipment_model *relGoodsShipModel = new Realization_goods_shipment_model;

    QAbstractItemModel* model = (QAbstractItemModel*) relGoodsShipModel->getModel();
    proxyModel->setSourceModel(model);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Information_realization::~Information_realization()
{
    delete ui;
}

void Information_realization::setModel(QAbstractItemModel *model){
    currentModel = model;
    mapper->setModel(model);

    mapper->setItemDelegate(new QSqlRelationalDelegate(this));


    mapper->addMapping(ui->id_spinBox, 0);
    mapper->addMapping(ui->dateEdit,1);
    mapper->addMapping(ui->seller_lineEdit, 2);

    proxyModel->setFilterKeyColumn(1);
    ui->tableView->setModel(proxyModel);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(0);
    Calculate_sum();

    connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Information_realization::updateButtonsAvailability);
    updateButtonsAvailability();
}

void Information_realization::setCurrentIndex(const QModelIndex &index){
    if (index.isValid()){
        mapper->setCurrentModelIndex(index);
        currentIndex = index;

        // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
        proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));

    }
}

void Information_realization::updateButtonsAvailability(){
    int currentIndex = mapper->currentIndex();
    int lastIndex = mapper->model()->rowCount() - 1; // Получаем индекс последней записи

    bool hasPrevious = (currentIndex > 0);
    bool hasNext = (currentIndex < lastIndex);

    ui->last_realization->setEnabled(hasPrevious);
    ui->next_realization->setEnabled(hasNext);
    Update_state();
}

void Information_realization::onModelUpdated(){
    mapper->setModel(currentModel); // Перезапуск модели в mapper
    mapper->toFirst(); // Установить на первый индекс или текущий актуальный индекс
    updateButtonsAvailability();
}


void Information_realization::on_save_pushButton_clicked()
{
    Calculate_sum();
    try {

        if (state == State::create and proxyModel->rowCount() == 0){
            QMessageBox::information(this, "Предупреждение", "Не добавлен товар!");

            return;
        }
        if (mapper->submit()) {

            Realization_goods_shipment_model* relGoodsShip = new Realization_goods_shipment_model;
            QSqlTableModel* model = relGoodsShip->getModel();
            model->setEditStrategy(QSqlTableModel::OnManualSubmit);
            model->submitAll();
            proxyModel->setSourceModel((QAbstractItemModel*)relGoodsShip->getModel());
            proxyModel->setFilterKeyColumn(1);
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


void Information_realization::on_close_pushButton_clicked()
{
    close();
    emit closed();
}


void Information_realization::on_last_realization_triggered()
{
    mapper->toPrevious();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));


    Update_state();
}


void Information_realization::on_next_realization_triggered()
{
    mapper->toNext();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);
    // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->id_spinBox->value()));
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->id_spinBox->value())));


    Update_state();
}

void Information_realization::Update_state(){
    if (ui->id_spinBox->value() == 0){
        state = State::create;
    }
    else state = State::update;
}

void Information_realization::on_add_record_triggered()
{
    QSqlTableModel *table_model = qobject_cast<QSqlTableModel*>(proxyModel->sourceModel());
    table_model->setEditStrategy(QSqlTableModel::OnRowChange);
    if (table_model) {
        int row = table_model->rowCount();
        table_model->insertRow(row);
        QModelIndex index = table_model->index(row, 1); // Индекс скрытой колонки
        table_model->setData(index, ui->id_spinBox->value());
        table_model->submit();
        proxyModel->invalidate(); // Перезагружаем прокси-модель

    }
}


void Information_realization::on_delete_record_triggered()
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
                model->setRelation(2, QSqlRelation("goods", "id", "name"));
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


void Information_realization::Calculate_sum() {
    int id = ui->id_spinBox->value(); // Получаем ID отгрузки из виджета

    QSqlQuery query;
    query.prepare("SELECT SUM(g.sale_price * rg.quantity) AS total_realization_price "
                  "FROM realization_goods_shipment rg "
                  "JOIN goods g ON rg.ID_goods = g.ID "
                  "WHERE rg.ID_shipment = :shipment_id");
    query.bindValue(":shipment_id", id); // Привязываем значение ID отгрузки к запросу

    if (query.exec()) { // Проверяем успешность выполнения запроса
        if (query.next()) { // Переходим к первой записи результата запроса
            double total_sum = query.value(0).toDouble(); // Извлекаем сумму из результата
            ui->SumLineEdit->setText(QString::number(total_sum)); // Устанавливаем значение в поле sumLineEdit
        } else {
            // Если запрос не вернул результатов
            qDebug() << "Отгрузка с ID" << id << "не найдена";
        }
    } else {
        // Если запрос не удалось выполнить
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
}

