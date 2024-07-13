#include "information_goods.h"
#include "ui_information_goods.h"
#include <QMessageBox>

Information_goods::Information_goods(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Information_goods)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

}

Information_goods::~Information_goods()
{
    delete ui;
}


void Information_goods::setModel(QAbstractItemModel *model){
    currentModel = model;

    QSqlRelationalTableModel *relationalModel = static_cast<QSqlRelationalTableModel*>(model);

    mapper->setModel(relationalModel);

    mapper->setItemDelegate(new QSqlRelationalDelegate(this));



    mapper->addMapping(ui->id_goods, 0);

    mapper->addMapping(ui->contractors, 1);
    ui->contractors->setModel(relationalModel->relationModel(1));
    ui->contractors->setModelColumn(1);

    mapper->addMapping(ui->units, 2);
    ui->units->setModel(relationalModel->relationModel(2));
    ui->units->setModelColumn(1);

    mapper->addMapping(ui->category, 3);
    ui->category->setModel(relationalModel->relationModel(3));
    ui->category->setModelColumn(1);

    mapper->addMapping(ui->name_goods, 5);
    mapper->addMapping(ui->description, 6);
    mapper->addMapping(ui->purchase_price_spinBox, 8);
    mapper->addMapping(ui->sale_price_spinBox, 9);
    mapper->addMapping(ui->quntity, 10);



    // mapper->addMapping(ui->idOrderComboBox,1);
    // mapper->addMapping(ui->dateEdit, 2);

    // ui->idOrderComboBox->setModel(model);
    // ui->idOrderComboBox->setModelColumn(1);

    // proxyModel->setFilterKeyColumn(2);
    // ui->tableView->setModel(proxyModel);
    // ui->tableView->hideColumn(2);






    connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Information_goods::updateButtonsAvailability);
    updateButtonsAvailability();
}

void Information_goods::setCurrentIndex(const QModelIndex &index){
    if (index.isValid()){
        mapper->setCurrentModelIndex(index);
        currentIndex = index;
    }
}

void Information_goods::updateButtonsAvailability(){
    int currentIndex = mapper->currentIndex();
    int lastIndex = mapper->model()->rowCount() - 1; // Получаем индекс последней записи

    bool hasPrevious = (currentIndex > 0);
    bool hasNext = (currentIndex < lastIndex);

    ui->last_goods->setEnabled(hasPrevious);
    ui->next_goods->setEnabled(hasNext);
    Update_state();
}


void Information_goods::onModelUpdated(){
    mapper->setModel(currentModel); // Перезапуск модели в mapper
    mapper->toFirst(); // Установить на первый индекс или текущий актуальный индекс
    updateButtonsAvailability();
}

void Information_goods::on_save_pushButton_clicked()
{
    try {
        if (mapper->submit()) {
            \

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


void Information_goods::on_close_pushButton_clicked()
{
    close();
    emit closed();
}


void Information_goods::on_last_goods_triggered()
{
    mapper->toPrevious();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);

    Update_state();
}


void Information_goods::on_next_goods_triggered()
{
    mapper->toNext();
    int row = mapper->currentIndex();
    currentIndex = currentModel->index(row, 0);

    Update_state();
}

void Information_goods::Update_state(){
    if (ui->id_goods->value() == 0){
        state = State::create;
    }
    else state = State::update;
}


