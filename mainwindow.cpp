#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

#include "category_model.h"
#include "goods_model.h"
#include "units_of_measurement_model.h"
#include "contractors_model.h"
#include "orders_model.h"
#include "realization_model.h"
#include "shipment_model.h"
#include "status_goods_model.h"
#include "status_realization_model.h"
#include <QFileDialog>
#include <QLocale>
#include "database.h"
#include "orders_goods_model.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addAction(ui->remove_tab);


    ctrlPressed = false;
    currentTableData = nullptr;
    infCategory = nullptr;
    infOrder = nullptr;
    infShipment = nullptr;
    infGoods = nullptr;
    infRealization = nullptr;
    searchForm = nullptr;
    searchTable = ActiveTable::None;
    isProxyModelActive = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Создание таблицы
QTableView* MainWindow::createTable(){


    if (currentTableData){
        currentTableData = nullptr;
    }

    // Освобождение занаятой памяти
    if (currentTable == ActiveTable::Category){
        delete infCategory;
        infCategory = nullptr;
    }
    if (currentTable == ActiveTable::Order){
        delete infOrder;
        infOrder = nullptr;
    }
    if (currentTable == ActiveTable::Shipment){
        delete infShipment;
        infShipment = nullptr;
    }
    if (currentTable == ActiveTable::Goods){
        delete infGoods;
        infGoods = nullptr;
    }
    if(currentTable == ActiveTable::Realization){
        delete infRealization;
        infRealization = nullptr;
    }


    // Создание таблицы
    QTableView* temp = createTableView();
    setupLayout(temp);
    enableSorting(temp);
    setHeaderResizeMode(temp);
    temp->setLocale(QLocale::Russian);
    return temp;
}

QTableView* MainWindow::createTableView(){
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QTableView* existingTableView = currentTab->findChild<QTableView*>();
    if (existingTableView) {
        return existingTableView;
    }
    return new QTableView(currentTab);
}

void MainWindow::setupLayout(QTableView *view){
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QVBoxLayout* layout = nullptr;
    if (currentTab->layout()) {
        layout = qobject_cast<QVBoxLayout*>(currentTab->layout());
    } else {
        layout = new QVBoxLayout(currentTab);
        currentTab->setLayout(layout);
    }
    layout->addWidget(view);
}

void MainWindow::setHeaderResizeMode(QTableView *view){
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::enableSorting(QTableView *view){
    view->setSortingEnabled(true);
}

// Обработка нажатий на action для просмотра таблиц
void MainWindow::on_contractors_triggered()
{
    Contractors_model contractors;
    currentTableData = createTable();
    currentTableData->setModel(contractors.getModel());
    currentTable = ActiveTable::Contractors;
    isProxyModelActive = false;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);

}

void MainWindow::on_goods_triggered()
{
    Goods_model goods;
    currentTableData = createTable();
    currentTableData->setModel(goods.getModel());

    if (currentTable != ActiveTable::Goods){
        currentTableData->horizontalHeader()->swapSections(1,5);

    }

    currentTableData->hideColumn(1);
    currentTableData->hideColumn(6);
    currentTableData->hideColumn(7);
    currentTableData->hideColumn(8);
    currentTableData->hideColumn(9);
    currentTableData->setItemDelegate(new QSqlRelationalDelegate(this));

    currentTable = ActiveTable::Goods;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
    isProxyModelActive = false;
}


void MainWindow::on_units_of_measurmant_triggered()
{
    Units_of_measurement_model uom;
    currentTableData = createTable();
    currentTableData->setModel(uom.getModel());
    currentTable = ActiveTable::Units;
    isProxyModelActive = false;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
}


void MainWindow::on_category_triggered()
{
    Category_model category;
    currentTableData = createTable();
    currentTable = ActiveTable::Category;
    currentTableData->setModel(category.getModel());
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
}


void MainWindow::on_orders_triggered()
{
    Orders_model orders;
    currentTableData = createTable();
    currentTableData->setModel(orders.getModel());
    currentTable = ActiveTable::Order;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
    isProxyModelActive = false;

}

void MainWindow::on_status_goods_triggered()
{
    Status_goods_model status_goods;
    currentTableData = createTable();
    currentTableData->setModel(status_goods.getModel());
    currentTable = ActiveTable::Status_goods;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
    isProxyModelActive = false;
}


void MainWindow::on_shipment_triggered()
{
    Shipment_model shipment;
    currentTableData = createTable();
    currentTableData->setModel(shipment.getModel());
    currentTable = ActiveTable::Shipment;
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
    isProxyModelActive = false;

}


void MainWindow::on_realization_triggered()
{
    Realization_model realization;
    currentTableData = createTable();
    currentTable = ActiveTable::Realization;
    currentTableData->setModel(realization.getModel());
    connect(currentTableData, &QTableView::clicked, this, &MainWindow::onTableClicked);
    isProxyModelActive = false;
}

void MainWindow::on_status_realization_triggered()
{
    Status_realization_model status;
    currentTableData = createTable();
    currentTableData->setModel(status.getModel());
    currentTable = ActiveTable::Status_realization;
    isProxyModelActive = false;


}
// конец реализаций для просмотра таблиц


void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    // Получение текущего название вкладки
    QString currentTabText = ui->tabWidget->tabText(index);

    // Отображение диалогового окно с запросом на новое название вкладки
    bool ok;
    QString newTabText = QInputDialog::getText(this, "Изменить название вкладки",
                                               "Введите новое название:", QLineEdit::Normal,
                                               currentTabText, &ok);
    // Если пользователь нажал ОК и ввел новое название, устанавливаем его
    if (ok && !newTabText.isEmpty()) {
        ui->tabWidget->setTabText(index, newTabText);
    }
}

// Добавление новой вкладки
void MainWindow::on_add_tab_triggered()
{
    QWidget *newTab = new QWidget();

    ui->tabWidget->addTab(newTab, "Новая вкладка");
    ui->tabWidget->setTabsClosable(true);
}

// Закрытие вкладки
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);

    if(ui->tabWidget->count() <= 2){
        ui->tabWidget->setTabsClosable(false);
    }
    else ui->tabWidget->setTabsClosable(true);
}

//  Обарботка событий
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
    QMainWindow::keyReleaseEvent(event);
}

// закрытие форм
void MainWindow::onInformationCategoryClosed() {
    disconnect(infCategory,  &Information_category::closed, this, &MainWindow::onInformationCategoryClosed);
    delete infCategory;
    infCategory = nullptr;
}

void MainWindow::onInformationOrderClosed(){
    disconnect(infOrder,  &Information_order::closed, this, &MainWindow::onInformationOrderClosed);
    delete infOrder;
    infOrder = nullptr;
}

void MainWindow::onInformationShipmentClosed(){
    disconnect(infShipment,  &Information_shipment::closed, this, &MainWindow::onInformationShipmentClosed);
    delete infShipment;
    infShipment = nullptr;
}

void MainWindow::onInformationGoodsClosed(){
    disconnect(infGoods,  &Information_goods::closed, this, &MainWindow::onInformationGoodsClosed);
    delete infGoods;
    infGoods = nullptr;
}

void MainWindow::onInformationRealizationClosed(){
    disconnect(infRealization,  &Information_realization::closed, this, &MainWindow::onInformationRealizationClosed);
    delete infRealization;
    infRealization = nullptr;
}

// Нажатие на таблицу
void MainWindow::onTableClicked(const QModelIndex &index){
    if (isProxyModelActive){
        currentRow = proxyModel->mapToSource(index).row();
    }
    else currentRow = index.row();
    // Отслеживание нажатия ctrl
    if (ctrlPressed) {
        if (index.isValid()) {
            int indexColumn = index.column();
            switch (currentTable) {
            case ActiveTable::Category:
                expandCategoryTable(index);
                break;
            case ActiveTable::Order:
                expandOrderTable(index);
                break;
            case ActiveTable::Shipment:
                expandShipmentTable(index);
                break;
            case ActiveTable::Goods:
                expandGoodsTable(index);
                break;
            case ActiveTable::Realization:
                expandRealizationTable(index);
                break;
            default:
                break;
            }
        }
    }
}

//  Открытие форм
void MainWindow::expandCategoryTable(const QModelIndex &index){
    if (index.column() == 0){
        if (!infCategory){
            infCategory = new Information_category(this);
        }


        if (isProxyModelActive){
            infCategory->setModel(proxyModel->sourceModel());
            infCategory->setCurrentIndex(proxyModel->mapToSource(index));
        }
        else{
            infCategory->setModel(currentTableData->model());
            infCategory->setCurrentIndex(index);
        }
        connect(infCategory, &Information_category::closed, this, &MainWindow::onInformationCategoryClosed);
        connect(this, &MainWindow::modelUpdated, infCategory, &Information_category::onModelUpdated);

        showForm(infCategory);
    }
}

void MainWindow::expandOrderTable(const QModelIndex &index){
    if (index.column() == 0){
        if(!infOrder){
            infOrder = new Information_order(this);
        }

        if (isProxyModelActive){
            infOrder->setModel(proxyModel->sourceModel());
            infOrder->setCurrentIndex(proxyModel->mapToSource(index));
        }
        else{
            infOrder->setModel(currentTableData->model());
            infOrder->setCurrentIndex(index);
        }

        connect(infOrder, &Information_order::closed, this, &MainWindow::onInformationOrderClosed);
        connect(this, &MainWindow::modelUpdated, infOrder, &Information_order::onModelUpdated);

        showForm(infOrder);

    }
}

void MainWindow::expandShipmentTable(const QModelIndex &index){
    if (index.column() == 0){
        if(!infShipment){
            infShipment = new Information_shipment(this);
        }

        if (isProxyModelActive){
            infShipment->setModel(proxyModel->sourceModel());
            infShipment->setCurrentIndex(proxyModel->mapToSource(index));
        }
        else{
            infShipment->setModel(currentTableData->model());
            infShipment->setCurrentIndex(index);
        }

        connect(infShipment, &Information_shipment::closed, this, &MainWindow::onInformationShipmentClosed);
        connect(this, &MainWindow::modelUpdated, infShipment, &Information_shipment::onModelUpdated);

        showForm(infShipment);

    }
}

void MainWindow::expandGoodsTable(const QModelIndex &index){
    if (index.column() == 0){
        if (!infGoods){
            infGoods = new Information_goods(this);
        }
        if (isProxyModelActive){
            infGoods->setModel(proxyModel->sourceModel());
            infGoods->setCurrentIndex(proxyModel->mapToSource(index));
        }
        else{
            infGoods->setModel(currentTableData->model());
            infGoods->setCurrentIndex(index);
        }


        connect(infGoods, &Information_goods::closed, this, &MainWindow::onInformationGoodsClosed);
        connect(this, &MainWindow::modelUpdated, infGoods, &Information_goods::onModelUpdated);

        showForm(infGoods);
    }
}

void MainWindow::expandRealizationTable(const QModelIndex &index){
    if (index.column() == 0){
        if (!infRealization){
            infRealization = new Information_realization(this);
        }

        if (isProxyModelActive){
            infRealization->setModel(proxyModel->sourceModel());
            infRealization->setCurrentIndex(proxyModel->mapToSource(index));
        }
        else{
            infRealization->setModel(currentTableData->model());
            infRealization->setCurrentIndex(index);
        }

        connect(infRealization, &Information_realization::closed, this, &MainWindow::onInformationRealizationClosed);
        connect(this, &MainWindow::modelUpdated, infRealization, &Information_realization::onModelUpdated);

        showForm(infRealization);
    }
}


// Отображение форм
void MainWindow::showForm(QWidget *form){
    QWidget *currentTabWidget = ui->tabWidget->currentWidget();
    QLayout *currentLayout = currentTabWidget->layout();

    if (!form){
        qDebug() << "Метод: showForm: Форма не существует, поэтому не может отобразиться!";
        return;
    }

    if (!currentLayout) {
        currentLayout = new QHBoxLayout(currentTabWidget);
        currentTabWidget->setLayout(currentLayout);
    }
    // Добавление содержимого в текущий layout
    currentLayout->addWidget(form);
    // Переключение на текущий виджет вкладки
    ui->tabWidget->setCurrentWidget(currentTabWidget);
}

// Вывод сообщений
void MainWindow::showInformationMessage(const QString &message) {
    QMessageBox::information(this, tr("Информация"), message);
}

void MainWindow::showErrorMessage(const QString &message){
    QMessageBox::warning(this, "Ошибка", message);
}

// Добавление записи в таблицу
void MainWindow::on_add_record_triggered()
{
    if (!currentTableData) {
        // Если текущая таблица не существует, выводим сообщение об ошибке
        showInformationMessage( "Таблица не выбрана или не загружена.");
        return;
    }

    QAbstractItemModel *absmodel = currentTableData->model();
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(absmodel);
    if (model) {
        model->insertRow(model->rowCount(QModelIndex()));
        if (currentTable == ActiveTable::Category){
            expandCategoryTable(model->index(model->rowCount() - 1,0));
        }
        if (currentTable == ActiveTable::Order){
            expandOrderTable(model->index(model->rowCount() - 1,0));
        }
        if (currentTable == ActiveTable::Shipment){
            expandShipmentTable(model->index(model->rowCount() - 1,0));
        }
        if (currentTable == ActiveTable::Goods){
            expandGoodsTable(model->index(model->rowCount() - 1, 0));
        }
        if (currentTable == ActiveTable::Realization){
            expandRealizationTable(model->index(model->rowCount() - 1, 0));
        }
    } else {
        // Если модель не является QSqlTableModel, также выводим сообщение об ошибке
        showErrorMessage("Текущая таблица не поддерживает добавление записей.");
    }
}

// Удаление записи в таблице
void MainWindow::on_delete_record_triggered()
{
    if (!currentTableData) {
        // Если текущая таблица не существует, выводим сообщение об ошибке
        showInformationMessage( "Таблица не выбрана или не загружена.");
        return;
    }
    QAbstractItemModel *absmodel;
    if (isProxyModelActive){
        absmodel = proxyModel->sourceModel();
    }
    else absmodel = currentTableData->model();

    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(absmodel);
    if (model) {
        model->removeRow(currentRow);
        model->select();
        emit modelUpdated(); // Сигнал обновления модели
    }
}


void MainWindow::on_remove_tab_triggered()
{
    if (ui->tabWidget->count() > 2) {
        // Проверка, есть ли вообще вкладки
        int currentIndex = ui->tabWidget->currentIndex(); // Получение индекс текущей вкладки
        ui->tabWidget->removeTab(currentIndex);
    }
    if (ui->tabWidget->count() == 2) ui->tabWidget->setTabsClosable(false);

}

// Экспорт данных в эксель
void MainWindow::on_exportExcel_triggered()
{
    if (!currentTableData || !currentTableData->model()) {
        // Если currentTableData или модель не установлены
        qWarning() << "No table data to export";
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Excel Files (*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".xlsx", Qt::CaseInsensitive)) {
        fileName += ".xlsx";
    }

    qDebug() << "Saving to file:" << fileName;

    lxw_workbook  *workbook  = workbook_new(fileName.toStdString().c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    QAbstractItemModel *model = currentTableData->model();

    // Экспорт названий столбцов
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        worksheet_write_string(worksheet, 0, col, header.toUtf8().constData(), NULL);
    }

    // Экспорт данных из модели в Excel
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QVariant data = model->data(index);
            worksheet_write_string(worksheet, row + 1, col, data.toString().toUtf8().constData(), NULL); // Сдвиг на 1 строку из-за заголовков
        }
    }
    workbook_close(workbook);
    qDebug() << "Файл успешно сохранен как" << fileName;
    // Проверка, что файл имеет расширение .xlsx

}



// Открытие поисковой формы
void MainWindow::on_search_triggered()
{
    if(!searchForm){
        searchForm = new SearhForm(this);
        connect(searchForm, &SearhForm::searchTextEntered, this, &MainWindow::searchInTable);
    }
    searchForm->show();
}

// Настройка поиска в табице
void MainWindow::searchInTable(const QString &searchText)
{
    proxyModel = new QSortFilterProxyModel(this);

    if(searchTable == currentTable){
        proxyModel->setSourceModel(originalModel);
    }
    else{
        searchTable = currentTable;
        originalModel = currentTableData->model();
        proxyModel->setSourceModel(originalModel);
    }

    if (currentTableData && currentTableData->model()) {

        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterKeyColumn(-1); // Поиск по всем колонкам
        proxyModel->setFilterFixedString(searchText);
        currentTableData->setModel(proxyModel);
        isProxyModelActive = true;
    }

}



// Открытие формы для отчета
void MainWindow::on_report_action_triggered()
{
    ReportDialog *repDialog = new  ReportDialog;
    repDialog->exec();
}


