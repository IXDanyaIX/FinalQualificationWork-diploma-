#ifndef INFORMATION_SHIPMENT_H
#define INFORMATION_SHIPMENT_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include "shipment_goods_model.h"

namespace Ui {
class Information_shipment;
}

class Information_shipment : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information_shipment(QWidget *parent = nullptr);
    ~Information_shipment();

    void setModel(QAbstractItemModel* model);
    void setCurrentIndex(const QModelIndex& index);

signals:
    void closed();

private slots:
    void on_close_pushButton_clicked();

    void on_save_pushButton_clicked();

    void on_last_shipment_triggered();

    void on_next_shipment_triggered();

    //my slots
    void updateButtonsAvailability();

    void on_add_record_triggered();

    void on_delete_record_triggered();

public slots:
    void onModelUpdated();


private:
    void Update_state();
    enum class State{
        create,
        update
    };

    State state;
    QModelIndex currentIndex;
    QDataWidgetMapper* mapper;
    QAbstractItemModel *currentModel;
    QSortFilterProxyModel *proxyModel;
    Ui::Information_shipment *ui;
};

#endif // INFORMATION_SHIPMENT_H
