#include "information_category.h"
#include "ui_information_category.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include "goods_model.h"
#include <QSqlRelationalDelegate>


    Information_category::Information_category(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::Information_category)
    {
        ui->setupUi(this);

        mapper = new QDataWidgetMapper(this);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        //ui->units->setSizeAdjustPolicy(QComboBox::AdjustToContents);



        proxyModel = new QSortFilterProxyModel(this);
        Goods_model *goods = new Goods_model;

        QSqlRelationalTableModel* table_model = (QSqlRelationalTableModel*)goods->getModel();

        QAbstractItemModel* model = (QAbstractItemModel*)goods->getModel();
        proxyModel->setSourceModel(model);


        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }


    Information_category::~Information_category()
    {
        delete ui;
    }

    void Information_category::setModel(QAbstractItemModel *model){
        currentModel = model;
        mapper->setModel(model);
        mapper->addMapping(ui->id_spinBox, 0);
        mapper->addMapping(ui->category_textEdit, 1, "plainText");

        proxyModel->setFilterKeyColumn(3); // Устанавливаем фильтр по первому столбцу


        ui->tableView->setModel(proxyModel);


        ui->tableView->horizontalHeader()->swapSections(1,5);

        ui->tableView->hideColumn(1);
        ui->tableView->hideColumn(3);
        ui->tableView->hideColumn(4);
        ui->tableView->hideColumn(6);
        ui->tableView->hideColumn(7);






        connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &Information_category::updateButtonsAvailability);
        updateButtonsAvailability();
    }

    void Information_category::setCurrentIndex(const QModelIndex &index){
        currentIndex = index;
        if (index.isValid()){
            mapper->setCurrentModelIndex(index);
            // proxyModel->setFilterRegExp(QString("^%1$").arg(ui->category_textEdit->toPlainText()));
            proxyModel->setFilterRegularExpression(QRegularExpression(QString("^%1$").arg(ui->category_textEdit->toPlainText())));

        }



    }

    void Information_category::updateButtonsAvailability(){

        //mapper->toLast();
        int currentIndex = mapper->currentIndex();
        int lastIndex = mapper->model()->rowCount() - 1; // Получаем индекс последней записи

        bool hasPrevious = (currentIndex > 0);
        bool hasNext = (currentIndex < lastIndex);

        ui->last_category->setEnabled(hasPrevious);
        ui->next_category->setEnabled(hasNext);

        Update_state();
    }

    void Information_category::onModelUpdated(){
        mapper->setModel(currentModel); // Перезапуск модели в mapper
        mapper->toFirst(); // Установить на первый индекс или текущий актуальный индекс
        updateButtonsAvailability();
    }


    void Information_category::on_save_pushButton_clicked()
    {
        try {
            if (ui->category_textEdit->toPlainText() == ""){
                QMessageBox::information(this, "Предупреждение", "Не удалось сохранить данные! Не все поля были заполнены или категория с таким индексом уже существует!");
                return;
            }
            if (mapper->submit()) {
                Goods_model *goods = new Goods_model;
                QSqlTableModel* model = goods->getModel();
                model->setEditStrategy(QSqlTableModel::OnManualSubmit);
                model->submitAll();
                proxyModel->setSourceModel((QAbstractItemModel*)goods->getModel());
                proxyModel->setFilterRegExp(QString("^%1$").arg(ui->category_textEdit->toPlainText()));


            } else {
                QSqlTableModel *tempModel = qobject_cast<QSqlTableModel*>(currentModel);
                if (tempModel) {
                    // Установка стратегии редактирования модели на OnRowChange
                    tempModel->setEditStrategy(QSqlTableModel::OnRowChange);
                }

                qDebug() << "Submit failed: " << tempModel->lastError().text();
                tempModel->revertRow(currentIndex.row());
                if (state == State::create) tempModel->insertRow(tempModel->rowCount());
                //tempModel->revert();
                mapper->revert();
                mapper->setCurrentModelIndex(currentIndex);
                QMessageBox::information(this, "Предупреждение", "Не удалось сохранить данные! Не все поля были заполнены или категория с таким индексом уже существует!");


            }
        } catch (const std::exception& e) {
            qDebug() << "Exception caught: " << e.what();
        }
    }


    void Information_category::on_close_pushButton_clicked()
    {
        close();
        emit closed();
    }


    void Information_category::on_last_category_triggered()
    {
        mapper->toPrevious();
        int row = mapper->currentIndex();
        currentIndex = currentModel->index(row, 0);
        proxyModel->setFilterRegExp(QString("^%1$").arg(ui->category_textEdit->toPlainText()));
        Update_state();

    }


    void Information_category::on_next_category_triggered()
    {
        mapper->toNext();
        int row = mapper->currentIndex();
        currentIndex = currentModel->index(row, 0);
        proxyModel->setFilterRegExp(QString("^%1$").arg(ui->category_textEdit->toPlainText()));
        Update_state();

    }

    void Information_category::Update_state(){
        if (ui->category_textEdit->toPlainText() == ""){
            state = State::create;
        }
        else { state = State::update; }
    }

