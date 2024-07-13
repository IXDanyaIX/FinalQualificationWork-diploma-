#ifndef INFORMATION_ORDER_H
#define INFORMATION_ORDER_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include <orders_goods_model.h>

namespace Ui {
class Information_order;
}

class Information_order : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information_order(QWidget *parent = nullptr);
    ~Information_order();
    void setModel(QAbstractItemModel* model);
    void setCurrentIndex(const QModelIndex& index);

signals:
    void closed();

private slots:

    void on_close_pushButton_clicked();

    void on_save_pushButton_clicked();

    void on_last_order_triggered();

    void on_next_order_triggered();

    void on_add_record_triggered();

    void on_delete_record_triggered();

    void on_tableView_clicked(const QModelIndex &index);


    //my slots
    void updateButtonsAvailability();


public slots:
    void onModelUpdated();

private:
    void Update_state();
    // Подсчет суммы заказа
    void Calculate_sum();
    enum class State{
        create,
        update
    };

    int currentRow;
    State state;
    QModelIndex currentIndex;
    QDataWidgetMapper* mapper;
    QAbstractItemModel *currentModel;
    QSortFilterProxyModel *proxyModel;
    QSqlTableModel* tempModel;

    Ui::Information_order *ui;
};

#endif // INFORMATION_ORDER_H
