#ifndef INFORMATION_REALIZATION_H
#define INFORMATION_REALIZATION_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include "realization_goods_shipment_model.h"

namespace Ui {
class Information_realization;
}

class Information_realization : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information_realization(QWidget *parent = nullptr);
    ~Information_realization();

    void setModel(QAbstractItemModel* model);
    void setCurrentIndex(const QModelIndex& index);

signals:
    void closed();




private slots:
    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_last_realization_triggered();

    void on_next_realization_triggered();

    //my slots
    void updateButtonsAvailability();


    void on_add_record_triggered();

    void on_delete_record_triggered();

public slots:
    void onModelUpdated();


private:

    void Update_state();
    void Calculate_sum();
    enum class State{
        create,
        update
    };

    State state;
    QModelIndex currentIndex;
    QDataWidgetMapper* mapper;
    QAbstractItemModel *currentModel;
    QSortFilterProxyModel *proxyModel;
    Ui::Information_realization *ui;
};

#endif // INFORMATION_REALIZATION_H
